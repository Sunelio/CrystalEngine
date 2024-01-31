# Scene

## Constructor

```cpp
Scene(std::string _name)
```

Creates a scene with the given name (has to be a filename).

<br>

## Methods

```cpp
void Load()
```

Loads this scene using its name.

<br>

```cpp
void Save()
```

Save this scene to the file corresponding to its name.

<br>

```cpp
void Unload()
```

Unloads this scene from memory.

<br>

```cpp
void Start()
```

Calls every game object's Start method.

<br>

```cpp
void Update(const float& deltaTime, const Core::WindowInputs& inputs) const
```

Updates every game object.

<br>

```cpp
void Render(const Render::Renderer* renderer) const
```

Renders every game object.

<br>

## Getters

```cpp
bool IsLoaded() const
```

```cpp
Render::CameraManager* GetCameraManager() const
```

```cpp
Render::LightManager* GetLightManager() const
```

```cpp
SceneGraph* GetSceneGraph() const
```

