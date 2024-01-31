# ShaderProgram (inherits from Resource)

## Constructor

```cpp
ShaderProgram(const std::string& _name)
```

Creates a shader program with the given name.

<br>

## Methods

```cpp
void AttachShader(Resource* shader)
```

Attaches the given shader to be linked with the shader program.

<br>

```cpp
void AttachShaders(size_t count, ...)
```

Attaches the given shaders to be linked with the shader program.

<br>

```cpp
void Load() override
```

Loads the shader program if all attached shaders have been loaded.

<br>

```cpp
void SendToPipeline() override
```

Links and sends the shader program to GPU memory is all attached shaders have been sent to GPU memory.

<br>

## Getters

```cpp
uint GetId() const
```

Returns the shader program's ID.

<br>

```cpp
std::vector<Resource*> GetAttachedShaders() const
```

Returns all of the shaders attached to the shader program.

<br>

