# Renderer: Interface to the rendering API

## Sending resources to GPU memory

```cpp
template <typename T> void SendResourceToPipeline(T* resource) const
```

Stores the given resource into GPU memory.

<br>

```cpp
template <typename T> void DeleteResource(T* resource) const
```

Removes the given resource's data from GPU memory.

<br>

## Handling Shaders

```cpp
void UseShaderProgram(const Resources::ShaderProgram* shaderProgram) const
```

Starts using the given shader program.

<br>

```cpp
template <typename T> void SetShaderVariable(const Resources::ShaderProgram* shaderProgram, const char* name, const T& val) const
```

Sets the value of the specified shader variable.

<br>

## RenderTextures

```cpp
void BindRenderTexture(const Resources::RenderTexture* renderTexture) const
```

Starts using the given render texture to draw objects.

<br>

```cpp
void UnbindRenderTexture() const
```

Starts drawing objects directly to the screen texture.

<br>

```cpp
void ClearCurRenderTexture(const Maths::RGBA& color) const
```

Fills the current render texture with the given color.

<br>

## Rendering meshes

```cpp
void DrawSubMesh(const Resources::SubMesh* subMesh, const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager) const
```

Renders the given sub-mesh using the specified model transform matrix, camera and the light manager.

<br>

## Miscellaneous

```cpp
uint GetTextureSamplerId() const
```

Returns the index of the texture sampler (necessary to sample textures within shaders).

<br>

