# GameObjectTypes

```cpp
Empty,
Model,
Light,
Camera
```



<br>

# GameObject

## Attributes

```cpp
std::string name
```

```cpp
Maths::Transform transform
```

## Constructor

```cpp
GameObject(const size_t& _id, const std::string& _name, GameObject* _parent = nullptr, const GameObjectTypes& _type = GameObjectTypes::Empty)
```

Creates a game object with the given ID, name, parent and type.

<br>

## Methods

```cpp
void PlayModeStart() const
```

Calls every object script's Start method.

<br>

```cpp
void PlayModeLateStart() const
```

Calls every object script's LateStart method.

<br>

```cpp
void UpdateChildren(const float& deltaTime, const Core::WindowInputs& inputs) const
```

Calls every object script's Update method and updates children.

<br>

```cpp
void DrawChildren(const Render::Renderer* renderer, const Render::Camera* camera, const Render::LightManager* lightManager) const
```

Renders this game object and its children.

<br>

```cpp
void AddScript(Scripting::ObjectScript* script)
```

Adds the given script to this game object's scrpits.

<br>

```cpp
Scripting::ObjectScript* GetScript(const size_t& idx) const
```

Returns the script at the given index.

<br>

```cpp
void DeleteScript(Scripting::ObjectScript* script)
```

Deletes the given script and removes it from this game object's scripts.

<br>

```cpp
void DeleteScript(const size_t& idx)
```

Deletes the script at the given index and removes it from this game object's scripts.

<br>

```cpp
void AddChild(GameObject* child)
```

Adds the given game object to this game object's childrent.

<br>

```cpp
void RemoveChild(const size_t& childId)
```

Remomves the child at the given id from this game object's children.

<br>

```cpp
bool SetParent(GameObject* newParent)
```

Sets this game object's parent to the given game object.

<br>

```cpp
GameObject* FindInChildrenId(const size_t& searchId, const bool& isRootOfSearch = true) const
```

Searches recursively in children to find and return a game object with the given ID.

<br>

```cpp
GameObject* FindInChildren(const std::string& searchName, const bool& isRootOfSearch = true) const
```

Searched recursively in children to find and return a game object with the given name.

<br>

# ModelObject (inherits from GameObject)

## Attributes

```cpp
Resources::Mesh* mesh = nullptr
```

## Constructor

```cpp
ModelObject(const size_t& _id, const std::string& _name, Resources::Mesh* _mesh, GameObject* _parent = nullptr)
```

Creates a model object with the given ID, name, mesh and parent.

<br>

## Methods

```cpp
void Draw(const Render::Renderer* renderer, const Render::Camera *camera, const Render::LightManager* lightManager) const
```

Renders this model object with the given camera and light manager.

<br>

# LightObject (inherits from GameObject)

## Attributes

```cpp
Render::Light* light = nullptr
```

## Constructor

```cpp
LightObject(const size_t& _id, const std::string& _name, Render::Light* _light, GameObject* _parent = nullptr)
```

Creates a light object with the given ID, name, light and parent.

<br>

# CameraObject (inherits from GameObject)

## Attributes

```cpp
Render::Camera* camera = nullptr
```

## Constructor

```cpp
CameraObject(const size_t& _id, const std::string& _name, Render::Camera* _camera, GameObject* _parent = nullptr)
```

Creates a camera object with the given ID, name, camera and parent.

<br>

