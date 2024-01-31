# Mesh (inherits from Resource)

## Attributes

```cpp
std::vector<SubMesh*> subMeshes
```

## Constructor

```cpp
Mesh(const std::string& _name)
```

Creates a mesh with the given name.

<br>

## Methods

```cpp
void Load() override
```

Loads the mesh if all its sub-meshes are loaded.

<br>

```cpp
void SendToPipeline() override
```

Sends all sub-meshes to GPU memory, then sends mesh to GPU memory.

<br>

```cpp
bool AreSubMeshesLoaded() const
```

Returns true if all sub-meshes are loaded.

<br>

```cpp
bool WereSubMeshesSentToPipeline() const
```

Returns true if all sub-meshes have been sent to GPU memory.

<br>

