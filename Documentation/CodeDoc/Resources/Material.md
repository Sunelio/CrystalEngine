# Material (inherits from Resource)

## Attributes

```cpp
Maths::RGB ambient, diffuse, specular
```

Values that influence incoming light color.

<br>

```cpp
Maths::RGB emission
```

The color of light emitted by the object.

<br>

```cpp
float shininess = 32
```

Defines the intensity of highlights on the object.

<br>

```cpp
float transparency = 1
```

Defines how see-through the object is.

<br>

```cpp
Texture* ambientTexture = nullptr
```

Texture used for the color of ambient light.

<br>

```cpp
Texture* diffuseTexture = nullptr
```

Texture used for the color of diffuse light.

<br>

```cpp
Texture* specularTexture = nullptr
```

Texture used for the color of specular light.

<br>

```cpp
Texture* emissionTexture = nullptr
```

Texture used for the color of light emitted by the object.

<br>

```cpp
Texture* shininessMap = nullptr
```

Texture used to modify the object's shininess.

<br>

```cpp
Texture* alphaMap = nullptr
```

Texture used to modify the object's transparency.

<br>

```cpp
Texture* normalMap = nullptr
```

Texture used to modify the object's normals.

<br>

## Constructor

```cpp
Material(const std::string& _name)
```

Creates a material with the given name (usually a filename).

<br>

## Methods

```cpp
void Load() override
```

Loads the material.

<br>

```cpp
void SendToPipeline() override
```

Sends the material to GPU memory.

<br>

```cpp
void SetParams(const Maths::RGB& _ambient, const Maths::RGB& _diffuse, const Maths::RGB& _specular, const Maths::RGB& _emission, const float& _shininess, const float& _transparency)
```

Sets the material's parameters to the given ones.

<br>

