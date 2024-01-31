# SubMesh

## Constructor

```cpp
SubMesh(const std::string& _name)
```

Creates a sub-mesh with the given name.

<br>

## Methods

```cpp
void LoadVertices(const std::array<std::vector<float>, 3>& vertexData, std::array<std::vector<uint32_t>, 3> vertexIndices)
```

Loads vertices from arrays of vertex data and indices.

<br>

```cpp
bool SendVerticesToPipeline(size_t& totalVertexCount)
```

Sends the loaded vertices and indices to GPU memory.

<br>

## Getters

```cpp
std::string GetName() const
```

```cpp
uint GetVertexCount() const
```

```cpp
SubmeshPipelineData GetPipelineData() const
```

```cpp
bool IsLoaded() const
```

```cpp
bool WasSentToPipeline() const
```

```cpp
const ShaderProgram* GetShaderProgram() const
```

```cpp
Material* GetMaterial() const
```

```cpp
Scripting::ShaderScript* GetShaderScript() const
```

```cpp
const std::vector<Maths::TangentVertex>& GetVertices() const
```

```cpp
const std::vector<uint>& GetIndices() const
```

## Setters

```cpp
void SetLoadingDone()
```

```cpp
void SetPipelineTransferDone()
```

```cpp
void SetPipelineData(const SubmeshPipelineData& data)
```

```cpp
void SetShaderProgram(ShaderProgram* _shaderProgram)
```

```cpp
void SetMaterial(Material* _material)
```

```cpp
void SetShaderScript(Scripting::ShaderScript* _shaderScript)
```

