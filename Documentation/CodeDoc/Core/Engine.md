# Engine (singleton)

## Static methods

```cpp
static Engine* Create()
```

Creates a unique instance of the engine.

<br>

```cpp
static Engine* Get()
```

Returns the engine's unique instance.

<br>

```cpp
static void Destroy()
```

Destroys the engine's instance.

<br>

## Methods

```cpp
void Init(const WindowParams& windowParams)
```

Initialized the engine and all its components.

<br>

```cpp
void Run() const
```

Performs engine update and render in an infinite loop.

<br>

```cpp
void Quit() const
```

Quits the engine and closes the window.

<br>

```cpp
void Release() const
```

Releases all engine components and resources.

<br>

## Getters

```cpp
Debug::Logger* GetLogger() const
```

Returns the logger.

<br>

```cpp
Window* GetWindow() const
```

Returns the window.

<br>

```cpp
TimeManager* GetTimeManager() const
```

Returns the time manager.

<br>

```cpp
Render::Renderer* GetRenderer() const
```

Returns the renderer.

<br>

```cpp
Resources::ResourceManager* GetResourceManager() const
```

Returns the resource manager.

<br>

```cpp
Scenes::SceneManager* GetSceneManager() const
```

Returns the scene manager.

<br>

```cpp
Render::UserInterface* GetUi() const
```

Returns the user interface.

<br>

