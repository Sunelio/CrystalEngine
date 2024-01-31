# WavefrontParser: Custom OBJ and MTL loader

## Static Methods

```cpp
static std::vector<Resource*> ParseMtl(const std::string& filename)
```

Loads materials from the specified MTL file.

<br>

```cpp
static std::vector<Resource*> ParseObj(const std::string& filename)
```

Loads meshes, sub-meshes and materials from the specified MTL file.

<br>

