# ShaderScript: Class from which all shader scripts inherit

## Attributes

**All attributes are set upon script creation.**

```cpp
const Resources::SubMesh* mesh = nullptr
```

```cpp
const Resources::ShaderProgram* shader = nullptr
```

```cpp
const Resources::Material* material = nullptr
```

```cpp
const Maths::Mat4* modelMat = nullptr
```

```cpp
const Render::Camera* camera = nullptr
```

```cpp
const Render::LightManager* lightManager = nullptr
```

```cpp
const Render::Renderer* renderer = nullptr
```

```cpp
const Core::Engine* engine = nullptr
```

## Methods

```cpp
virtual void SetShaderVariables()
```

Overriden by children to set their shader's variable values.

<br>

