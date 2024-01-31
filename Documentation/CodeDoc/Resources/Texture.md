# Textures (inherits from Resource)

## Constructor

```cpp
Texture(const std::string& _name)
```

Creates a texture with the given name (has to be a filename).

<br>

## Methods

```cpp
void Load() override
```

Loads the texture.

<br>

```cpp
void SendToPipeline() override
```

Sends the texture to GPU memory.

<br>

```cpp
void SetFlippedVertivally(const bool& flipped)
```

Toggles vertical flip of the texture (triggers a total reload).

<br>

## Getters

```cpp
uint GetId() const
```

```cpp
int GetWidth() const
```

```cpp
int GetHeight() const
```

```cpp
uchar* GetData() const
```

Returns the texture's pixel data.

<br>

```cpp
int GetChannels() const
```

Returns the number of color channels in the image.

<br>

