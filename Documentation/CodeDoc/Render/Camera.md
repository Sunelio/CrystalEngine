# CameraParams: holds camera parameters

```cpp
int width = 0, height = 0
```

```cpp
float near = 0.01f, far = 100.f, fov = 60.f
```

# Camera

## Attributes

```cpp
Maths::Transform* transform = nullptr
```

Pointer to the parent CameraObject's transform.

<br>

## Constructors

```cpp
Camera(const CameraParams& _params)
```

Create a camera from the given parameters.

<br>

```cpp
Camera(const int& height, const int& width)
```

Create a camera from the given width and height.

<br>

## Setters

```cpp
void SetParams(const CameraParams& _params)
```

```cpp
void SetFullscreen(const bool& _fullscreen)
```

Toggles camera fullscreen (if true, the camera's size matches the window size).

<br>

## Getters

```cpp
CameraParams GetParams() const
```

```cpp
bool IsFullscreen() const
```

```cpp
Maths::Mat4 GetWorldTransform() const
```

Returns the camera's world transform.

<br>

```cpp
Maths::Mat4 GetProjectionMat() const
```

Returns the camera's perspective projection matrix.

<br>

```cpp
Maths::Mat4 GetViewMat() const
```

Returns the camera's view transformation matrix.

<br>

