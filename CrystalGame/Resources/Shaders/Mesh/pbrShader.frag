#version 330 core
#define PI 3.14159265358979323846

// Textures.
struct Texture
{
    bool      assigned;
    sampler2D texture;
};

// Materials.
struct Material 
{
    vec3    ambient, diffuse, specular, emission;
    float   shininess, transparency;
    Texture ambientTex, diffuseTex, specularTex, emissionTex;
    Texture shininessMap, alphaMap, normalMap;
    Texture metallicMap, roughnessMap, aoMap;
};

// Lights.
#define MAX_LIGHTS 30
struct Light
{
    bool assigned;

    vec3  ambient, diffuse, specular;
    float constant, linear, quadratic;
    float innerCutoff, outerCutoff;
    vec3  position, direction;
};

uniform sampler2D albedoMap;
uniform sampler2D normalMap;


// Output color.
out vec4 fragColor;

// Input pos, uv, normal and matrix.
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormal;
in mat3 tbnMatrix;

// Material to apply to the model.
uniform Material material;
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// Lights array.
uniform Light lights[MAX_LIGHTS];

// Camera position.
uniform vec3 viewPos;

// Gamma correction.
vec3 gamma = vec3(2.2);


// Function forward declarations.
void ComputeLightingParams(vec3 normal, vec3 viewDir, vec3 fragToLight, float shininess, inout float diff, inout vec3 reflectDir, inout float spec);
vec3 ComputeLight(Light light, vec3 normal, vec3 viewDir, float shininess, vec3 ambientTexVal, vec3 diffuseTexVal, vec3 specularTexVal);
void InitPBRValues();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main()
{

	// Compute view dir and initialize light sum.
    vec3 viewDir  = normalize(viewPos - fragPos);
    vec3 lightSum = vec3(0.0);


	vec3 normal = fragNormal;

    // Get normal from normal map.
	if (material.normalMap.assigned)
	{
		normal = texture(material.normalMap.texture, fragTexCoord).rgb * 2.0 - 1.0;
		normal = normalize(tbnMatrix * normal);
	}

	// Get shininess from shininess map.
	float shininess = material.shininess;
	if (material.shininessMap.assigned)
    {
        shininess = dot(texture(material.shininessMap.texture, fragTexCoord), vec4(1)) / 4;
    }

	// Get transparency from alpha map.
	float transparency = material.transparency;
	if (material.alphaMap.assigned)
    {
        transparency = dot(texture(material.alphaMap.texture, fragTexCoord), vec4(1)) / 4;
    }
    
    InitPBRValues();

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // Sample all textures only once.
	vec3 ambientTexVal  = vec3(1);
    vec3 diffuseTexVal  = vec3(1);
    vec3 specularTexVal = vec3(1);
    vec3 emissionTexVal = material.emission;
	if (material.ambientTex .assigned) { ambientTexVal   = texture(material.ambientTex .texture, fragTexCoord).rgb; }
	if (material.diffuseTex. assigned) { diffuseTexVal   = texture(material.diffuseTex .texture, fragTexCoord).rgb; }
	if (material.specularTex.assigned) { specularTexVal  = texture(material.specularTex.texture, fragTexCoord).rgb; }
	if (material.emissionTex.assigned) { emissionTexVal *= texture(material.emissionTex.texture, fragTexCoord).rgb; }

    // Compute lighting.
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        if (!lights[i].assigned)
            continue;

        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - fragPos);
        vec3 H = normalize(viewDir + L);
        float distance    = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lights[i].diffuse * attenuation;        
        
        if()
        // cook-torrance brdf
        float NDF = DistributionGGX(normal, H, roughness);        
        float G   = GeometrySmith(normal, viewDir, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 

        lightSum += ComputeLight(lights[i], normal, viewDir, shininess, ambientTexVal, diffuseTexVal, specularTexVal);

        lo += lightSum;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    fragColor = vec4(color, 1.0);
}


void ComputeLightingParams(vec3 normal, vec3 viewDir, vec3 fragToLight, float shininess, inout float diff, inout vec3 reflectDir, inout float spec)
{
    diff       = max(dot(normal, fragToLight), 0.0);
    reflectDir = reflect(fragToLight, normal);
    spec       = pow(max(dot(-viewDir, reflectDir), 0.0), shininess);
}


vec3 ComputeLight(Light light, vec3 normal, vec3 viewDir, float shininess, vec3 ambientTexVal, vec3 diffuseTexVal, vec3 specularTexVal)
{
    vec3  fragToLight = vec3(0.0);
    float cutoff      = 0.0;
    float diff        = 0.0;
    vec3  reflectDir  = vec3(0.0);
    float spec        = 0.0;
    float distance    = 0.0;
    float attenuation = 1.0;

    // If there is a cutoff, treat this as a spot light.
    if ((0 < light.outerCutoff && light.outerCutoff < PI*2) || (0 < light.innerCutoff && light.innerCutoff < PI*2))
    {
        fragToLight = normalize(light.position - fragPos);

        // Stop if the fragment isn't in the light cone.
        float cutoff = dot(fragToLight, -1 * light.direction);
        if (cutoff < light.outerCutoff)
            return vec3(0.0);

        ComputeLightingParams(normal, viewDir, fragToLight, shininess, diff, reflectDir, spec);

        // Compute attenuation.
        distance    = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance))
                    * clamp((cutoff - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
    }

    // If there is no attenuation, treat this as a directional light.
    else if (light.constant == 0.0 && light.linear == 0.0 && light.quadratic == 0.0)
    {
        fragToLight = normalize(-light.direction);
        ComputeLightingParams(normal, viewDir, fragToLight, shininess, diff, reflectDir, spec);
    }

	// In other cases, treat this as a point light.
    else
    {
        fragToLight = normalize(light.position - fragPos);
        ComputeLightingParams(normal, viewDir, fragToLight, shininess, diff, reflectDir, spec);

        // Compute attenuation.
        distance    = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    }

    // Combine all computations.
    vec3 ambient  = light.ambient  * material.ambient  * pow(ambientTexVal,  gamma);
    vec3 diffuse  = light.diffuse  * material.diffuse  * pow(diffuseTexVal,  gamma) * attenuation * diff;
    vec3 specular = light.specular * material.specular * pow(specularTexVal, gamma) * attenuation * spec;
    return ambient + diffuse + specular;
}

void InitPBRValues()
{
    if(material.albedoMap.assigned)
        albedo = pow(texture(material.albedoMap, fragTexCoord).rgb, 2.2);
    else 
        albedo = pow(texture(vec3(1.0), fragTexCoord).rgb, 2.2);

    if(material.metallicMap.assigned)
        metallic = texture(material.metallicMap, fragTexCoord).r;
    else
        metallic = texture(vec3(1.0), fragTexCoord).r;

    if(material.roughnessMap.assigned)
        roughness = texture(material.roughnessMap, fragTexCoord).r;
    else
         roughness = texture(vec3(1.0), fragTexCoord).r;
         
    if(material.aoMap.assigned)
        ao = texture(material.aoMap, fragTexCoord).r;
    else
        ao = texture(vec3(1.0), fragTexCoord).r;

}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}