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
};

// Lights.
#define MAX_LIGHTS 30
struct Light
{
    bool assigned;

    vec3  ambient, diffuse, specular;
    float constant, linear, quadratic;
    float outerCutoff, innerCutoff;
    vec3  position, direction;
};


// Output color.
out vec4 fragColor;

// Input pos, uv, normal and matrix.
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormal;
in mat3 tbnMatrix;

// Material to apply to the model.
uniform Material material;

// Render unlit instead of lit.
uniform bool unlit;

// Use plain color instead of lit material.
uniform bool usePlainColor;
uniform vec4 plainColor;

// Lights array.
uniform Light lights[MAX_LIGHTS];

// Camera position.
uniform vec3 viewPos;

// Gamma correction.
vec3 gamma = vec3(2.2);


// Function forward declarations.
void ComputeLightingParams(vec3 normal, vec3 viewDir, vec3 fragToLight, float shininess, inout float diff, inout vec3 reflectDir, inout float spec);
vec3 ComputeLight(Light light, vec3 normal, vec3 viewDir, float shininess, vec3 ambientTexVal, vec3 diffuseTexVal, vec3 specularTexVal);


void main()
{
    // Render mesh in plain color if specified.
    if (usePlainColor)
    {
        fragColor = plainColor;
        return;
    }

    // Render unlit mesh if specified.
    if (unlit)
    {
        fragColor = vec4(material.diffuse, 1.0);
        if (material.diffuseTex.assigned)
            fragColor *= vec4(pow(texture(material.diffuseTex .texture, fragTexCoord).rgb, gamma), 1.0);
        return;
    }

    // Sample all textures only once.
	vec3 ambientTexVal  = vec3(1);
    vec3 diffuseTexVal  = vec3(1);
    vec3 specularTexVal = vec3(1);
    vec3 emissionTexVal = material.emission;
	if (material.ambientTex .assigned) { ambientTexVal   = texture(material.ambientTex .texture, fragTexCoord).rgb; }
	if (material.diffuseTex. assigned) { diffuseTexVal   = texture(material.diffuseTex .texture, fragTexCoord).rgb; }
	if (material.specularTex.assigned) { specularTexVal  = texture(material.specularTex.texture, fragTexCoord).rgb; }
	if (material.emissionTex.assigned) { emissionTexVal *= texture(material.emissionTex.texture, fragTexCoord).rgb; }

	// Compute view dir and initialize light sum.
    vec3 viewDir  = normalize(viewPos - fragPos);
    vec3 lightSum = vec3(0.0);

    // Get normal from normal map.
	vec3 normal = fragNormal;
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

    // Compute lighting.
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        if (!lights[i].assigned)
            continue;
        lightSum += ComputeLight(lights[i], normal, viewDir, shininess, ambientTexVal, diffuseTexVal, specularTexVal);
    }

    fragColor     = vec4(lightSum + emissionTexVal, transparency);
    fragColor.rgb = clamp(pow(fragColor.rgb, 1.0/gamma), 0.0, 1.0);
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