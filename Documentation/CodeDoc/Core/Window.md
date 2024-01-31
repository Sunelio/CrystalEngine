# WindowParams: holds window parameters

```cpp
const char* name = ""
```

```cpp
int width = -1
```

```cpp
int height = -1
```

```cpp
int posX = 50
```

```cpp
int posY = 70
```

# WindowInputs: holds values of user inputs

```cpp
Maths::Vector3 dirMovement
```

```cpp
Maths::Vector2 mousePos
```

```cpp
Maths::Vector2 mouseDelta
```

```cpp
bool mouseLeftClick = false
```

```cpp
bool mouseMiddleClick = false
```

```cpp
bool mouseRightClick = false
```

# InputKeys: holds keybindings for user inputs

```cpp
int exit
```

Key used to exit the engine.

<br>

```cpp
int movement[6]
```

Right, Left, Up, Down, Fwd, Back.

<br>

# Window

## Constructor

```cpp
Window(const WindowParams& windowParams)
```

Create a window with the specified window parameters.

<br>

## Miscellaneous

```cpp
void Update()
```

Retrieves inputs from the user.

<br>

```cpp
void SwapBuffers() const
```

Sends the current framebuffer to the monitor for display.

<br>

```cpp
void Close() const
```

Closes the window.

<br>

## Setters

```cpp
void SetName(const char* windowName)
```

```cpp
void SetWidth(const int& windowWidth) const
```

```cpp
void SetHeight(const int& windowHeight) const
```

```cpp
void SetSize(const int& windowWidth, const int& windowHeight) const
```

```cpp
void SetPosX(const int& windowPosX) const
```

```cpp
void SetPosY(const int& windowPosY) const
```

```cpp
void SetPos(const int& windowPosX, const int& windowPosY) const
```

```cpp
void SetVsync(const bool& windowVsync) const
```

```cpp
void SetExitKey(const int& windowExitKey)
```

```cpp
void RemoveExitKey()
```

## Getters

```cpp
const char* GetName() const
```

```cpp
int GetWidth() const
```

```cpp
int GetHeight() const
```

```cpp
int GetPosX() const
```

```cpp
int GetPosY() const
```

```cpp
WindowInputs GetInputs() const
```

```cpp
GLFWwindow* GetWindowContext() const
```

```cpp
bool ShouldClose() const
```

