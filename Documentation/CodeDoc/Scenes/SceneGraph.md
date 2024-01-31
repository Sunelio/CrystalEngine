# SceneGraph

## Creating game objects

```cpp
GameObject* AddEmpty(const std::string& name, GameObject* parent = nullptr)
```

Creates a new empty game object with the given name.

<br>

```cpp
ModelObject* AddModel(const std::string& name, Resources::Mesh* mesh, GameObject* parent = nullptr)
```

Creates a new model object with the given name and mesh.

<br>

```cpp
LightObject* AddLight(const std::string& name, Render::Light* light, GameObject* parent = nullptr)
```

Creates a new light object with the given name and light.

<br>

```cpp
CameraObject* AddCamera(const std::string& name, Render::Camera* camera, GameObject* parent = nullptr)
```

Creates a new camera object with the given name and camera.

<br>

## Handling game objects

```cpp
void UpdateAll(const float& deltaTime, const Core::WindowInputs& inputs) const
```

Updates all game objects.

<br>

```cpp
void DrawAll(const Render::Renderer* renderer, const Render::Camera* camera, const Render::LightManager* lightManager) const
```

Renders all game objects with the given camera and light manager.

<br>

```cpp
void ClearAll()
```

Deletes all game objects.

<br>

## Searching for game objects

```cpp
GameObject* FindId(const size_t& searchId) const
```

Tries to find a game object with the given ID.

<br>

```cpp
GameObject* Find(const std::string& searchName) const
```

Tries to find a game object with the given name.

<br>

