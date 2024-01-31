# RenderTexture (inherits from Resource)

## Attributes

```cpp
Maths::RGBA clearColor
```

The color to fill the texture to.

<br>

## Constructor

```cpp
RenderTexture(const std::string& _name)
```

Creates a render texture with the given name.

<br>

## Methods

```cpp
void Load() override
```

Loads the render texture.

<br>

```cpp
void SendToPipeline() override
```

Sends the render texture to GPU memory.

<br>

```cpp
void BeginRender() const
```

Starts rendering onto the texture.

<br>

```cpp
void EndRender() const
```

Stops rendering to the texture and starts drawing objects directly to the screen texture.

<br>

## Setters

```cpp
void SetWidth(const int& _width)
```

```cpp
void SetHeight(const int& _height)
```

```cpp
void SetSize(const int& _width, const int& _height)
```

```cpp
void SetPipelineData(const RenderTexturePipelineData& data)
```

## Getters

```cpp
int GetWidth() const
```

```cpp
int GetHeight() const
```

```cpp
RenderTexturePipelineData GetPipelineData() const
```

