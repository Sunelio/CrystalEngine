# ResourceTypes

```cpp
Undefined,
Texture,
RenderTexture,
Cubemap,
Material,
Mesh,
VertexShader,
FragmentShader,
ShaderProgram
```



<br>

# Resource: Parent class for all engine resources

## Methods

```cpp
virtual void Load() = 0
```

Overriden by child resources to be loaded.

<br>

```cpp
virtual void SendToPipeline() = 0
```

Overriden by child resources to be sent to GPU memory.

<br>

## Getters

```cpp
std::string GetName() const
```

```cpp
ResourceTypes GetType() const
```

```cpp
bool IsLoaded() const
```

```cpp
bool WasSentToPipeline() const
```

## Setters

```cpp
void SetLoadingDone()
```

```cpp
void SetPipelineTransferDone()
```

# Non-member functions

```cpp
inline std::string Resources::ResourceTypeToString(const ResourceTypes& type)
```

Returns the name of the given resource type as a string.

<br>

