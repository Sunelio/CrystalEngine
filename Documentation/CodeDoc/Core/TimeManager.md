# TimeManager: handles FPS, delta time and vertical synchronization

## Constructor

```cpp
TimeManager(const int& _targetFPS, const bool& _vsync)
```

Parameters for maximum frames per second and state of vertical synchronization.

<br>

## Setters

```cpp
void SetVsyncEnabled(const bool& _vsync)
```

Enables/disables vertical synchronization.

<br>

```cpp
void SetTargetFPS(const int& _targetFPS)
```

Modifies the maximum frames per second.

<br>

## Getters

```cpp
bool IsVsyncEnabled() const
```

Returns true if vertical synchronization is enabled.

<br>

```cpp
int GetTargetFPS() const
```

Returns the maximum frames per second.

<br>

```cpp
float FPS() const
```

Return the current frames per second.

<br>

```cpp
float DeltaTime() const
```

Return the current delta time.

<br>

## Miscellaneous

```cpp
void WaitUntilNextFrame() const
```

Stops executing code until the next frame can start.

<br>

```cpp
void NewFrame()
```

Updates FPS and delta time for the current frame.

<br>

