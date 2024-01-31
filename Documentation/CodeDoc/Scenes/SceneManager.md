# SceneManager

## Methods

```cpp
bool Exists(const std::string& name) const
```

Returns true if a scene with the given name has been created.

<br>

```cpp
bool Loaded(const std::string& name) const
```

Returns true if a scene with the given name is loaded.

<br>

```cpp
Scene* Create(const std::string& name)
```

Creates a new scene with the given name and returns it.

<br>

```cpp
void Delete(const std::string& name)
```

Deletes the scene with the given name if it exists.

<br>

```cpp
Scene* Get(const std::string& name) const
```

Returns the scene with the given name if it exists.

<br>

```cpp
Scene* GetActiveScene() const
```

Returns the currently active scene.

<br>

```cpp
void SetActiveScene(const std::string& name)
```

Changes the currently active scene to the scene with the given name.

<br>

```cpp
void StartActiveScene() const
```

Starts the currently active scene.

<br>

```cpp
void UpdateActiveScene(const float& deltaTime, const Core::WindowInputs& inputs) const
```

Updates the currently active scene.

<br>

```cpp
void RenderActiveScene(const Render::Renderer* renderer) const
```

Renders the currently active scene.

<br>

