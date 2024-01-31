# VertexShader (inherits from Resource)

## Constructor

```cpp
VertexShader(const std::string& _name)
```

Creates a vertex shader with the given name (has to be a filename).

<br>

## Methods

```cpp
void Load() override
```

Loads the vertex shader source code.

<br>

```cpp
void SendToPipeline() override
```

Compiles and sends the vertex shader to GPU memory.

<br>

## Getters

```cpp
uint GetId() const
```

Returns the vertex shader's ID.

<br>

```cpp
std::string GetSourceCode() const
```

Returns the vertex shader's source code as a string.

<br>

# FragmentShader

## Constructor

```cpp
FragmentShader(const std::string& _name)
```

Creates a fragment shader with the given name (has to be a filename).

<br>

## Methods

```cpp
void Load() override
```

Loads the fragment shader source code.

<br>

```cpp
void SendToPipeline() override
```

Compiles and sends the fragment shader to GPU memory.

<br>

## Getters

```cpp
uint GetId() const
```

Returns the fragment shader's ID.

<br>

```cpp
std::string GetSourceCode() const
```

Returns the fragment shader's source code as a string.

<br>

