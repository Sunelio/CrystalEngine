# CameraManager

## Handling Cameras

```cpp
Camera* Create(const CameraParams& params)
```

Creates a new camera from the given parameters.

<br>

```cpp
Camera* Get(const size_t& index) const
```

Returns the camera at the given index.

<br>

```cpp
void Delete(const size_t& index)
```

Deletes the camera at the given index.

<br>

```cpp
void Delete(const Camera* camera)
```

Deletes the given camera.

<br>

## Miscellaneous

```cpp
void UpdateEngineCamera(const float& deltaTime, const Core::WindowInputs& inputs) const
```

Updates the transform of the edit mode camera based on user input.

<br>

```cpp
void SetSceneCamera(Camera* camera)
```

Changes which camera is used to render the scene in play mode.

<br>

```cpp
void ResizeFullscreenCameras() const
```

Resizes all fullscreen cameras to the current screen size.

<br>

## Getters

```cpp
Camera* GetEngineCamera() const
```

Returns the camera from which the scene is rendered in edit mode.

<br>

```cpp
Camera* GetSceneCamera() const
```

Returns the camera from which the scene is rendered in play mode.

<br>

