#include "Core/Window.h"
#include "Core/Engine.h"
#include "Debug/Logger.h"
#include "Render/CameraManager.h"
#include "Render/PostProcessor.h"
#include "Render/Renderer.h"
#include "Resources/Texture.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Core/EventDispatcher.h"

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/glfw3native.h>

using namespace Core;

constexpr int OPENGL_MAJOR = 4;
constexpr int OPENGL_MINOR = 5;

InputKeys::InputKeys()
{
    exit         = 0;
    movement[0]  = GLFW_KEY_D;
    movement[1]  = GLFW_KEY_A;
    movement[2]  = GLFW_KEY_E;
    movement[3]  = GLFW_KEY_Q;
    movement[4]  = GLFW_KEY_W;
    movement[5]  = GLFW_KEY_S;
    specialKey[0] = GLFW_KEY_LEFT_SHIFT;
    specialKey[1] = GLFW_KEY_SPACE;
    specialKey[2] = GLFW_KEY_F;
}

Window::Window(const WindowParams& windowParams)
    : params(windowParams)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Set the window size to the monitor size if windowWidth and windowHeight are not positive.
    if (params.width < 0 && params.height < 0)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        params.width = vidMode->width; params.height = vidMode->height - 30; // TODO: check this on different monitor resolutions.
        params.posX = 0; params.posY = 30;
    }
    
    // Create a new glfw window with the given parameters.
    glfwWindow = glfwCreateWindow(params.width, params.height, params.name, NULL, NULL);
    if (!glfwWindow) {
        DebugLogError("Unable to create a window.");
        return;
    }

    // Set the window's icon.
    Resources::Texture* logoTexture      = new Resources::Texture("Resources/Textures/Logos/logo.png"     ); logoTexture     ->SetFlippedVertically(false); logoTexture     ->Load();
    Resources::Texture* logoTextureSmall = new Resources::Texture("Resources/Textures/Logos/logoSmall.png"); logoTextureSmall->SetFlippedVertically(false); logoTextureSmall->Load();
    const GLFWimage icons[2] = {
        { logoTexture     ->GetWidth(), logoTexture     ->GetHeight(), logoTexture     ->GetData() },
        { logoTextureSmall->GetWidth(), logoTextureSmall->GetHeight(), logoTextureSmall->GetData() }
    };
    glfwSetWindowIcon(glfwWindow, 2, icons);
    delete logoTexture; delete logoTextureSmall;

    // Set the window's position and minimum size.
    glfwMakeContextCurrent(glfwWindow);
    SetPos(params.posX, params.posY);
    glfwSetWindowSizeLimits(glfwWindow, 1, 1, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Set window user pointer and callbacks.
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* _glfwWindow, int _width, int _height)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_glfwWindow);
        window->params.width = _width; window->params.height = _height;
        
        while (window->params.width == 0 || window->params.height == 0) {
            glfwGetFramebufferSize(_glfwWindow, &window->params.width, &window->params.height);
            glfwWaitEvents();
        }
        Engine::Get()->GetEventDispatcher()->Dispatch(new ScreenResizeEvent(window->params.width, window->params.height));
    });
    glfwSetWindowPosCallback(glfwWindow, [](GLFWwindow* _glfwWindow, int _posX, int _posY)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_glfwWindow);
        window->params.posX = _posX; window->params.posY = _posY;
    });
    /*
    glfwSetDropCallback(glfwWindow, [](GLFWwindow* window, int count, const char** paths)
    {
        if (const Application* app = Application::Get()) {
            for (int i = 0; i < count; i++) {
                // Load resource files.
            }
        }
    });
    */

    DebugLogInfo(std::string("Created a new window (title: ") + params.name + ", size: " + std::to_string(params.width) + "x" + std::to_string(params.height) + ").");
}

Window::~Window()
{
    glfwDestroyWindow(glfwWindow);
    DebugLogInfo(std::string("Closed window titled ") + params.name + ".");
}

void Window::InitGlfw()
{
    Assert(glfwInit(), "Unable to initialize GLFW");
    DebugLogInfo("Initialized GLFW.");
}

void Window::TerminateGlfw()
{
    glfwTerminate();
    DebugLogInfo("Terminated GLFW.");
}

ProcAddressFunc Window::GetProcAddressFuncPointer()
{
    return (ProcAddressFunc)glfwGetProcAddress;
}

void Window::LockInputs(const bool& locked)
{
    inputsLocked = locked;
}

void Window::LockHideCursor(const bool& locked)
{
    cursorLockedHidden = locked;
}

void Window::Update(const float& deltaTime)
{
    // Make sure the current context is the window.
    if (glfwWindow != glfwGetCurrentContext())
        glfwMakeContextCurrent(glfwWindow);

    // Update glfw events and update inputs.
    glfwPollEvents();
    UpdateInputs(deltaTime);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(glfwWindow);
    
    // Hide the mouse cursor and disable its movement when right clicking.
    if (inputsLocked) return;
    if (cursorLockedHidden) glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else                    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::Close() const
{
    glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}


void Window::SetName(const char* windowName)
{
    params.name = windowName;
    glfwSetWindowTitle(glfwWindow, params.name);
}

void Window::SetWidth (const int& windowWidth)                          const { glfwSetWindowSize(glfwWindow, windowWidth, params.height); }
void Window::SetHeight(                        const int& windowHeight) const { glfwSetWindowSize(glfwWindow, params.width, windowHeight); }
void Window::SetSize  (const int& windowWidth, const int& windowHeight) const { glfwSetWindowSize(glfwWindow, windowWidth, windowHeight); }
void Window::SetPosX  (const int& windowPosX)                           const { glfwSetWindowPos(glfwWindow, windowPosX, params.posY); }
void Window::SetPosY  (                       const int& windowPosY)    const { glfwSetWindowPos(glfwWindow, params.posX, windowPosY); }
void Window::SetPos   (const int& windowPosX, const int& windowPosY)    const { glfwSetWindowPos(glfwWindow, windowPosX, windowPosY); }

void Window::SetVsync(const bool& windowVsync) const
{
    if (glfwWindow != glfwGetCurrentContext())
        glfwMakeContextCurrent(glfwWindow);
    
    glfwSwapInterval(windowVsync);
}

HWND Window::GetWindow32Context() const
{
    return glfwGetWin32Window(glfwWindow);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(glfwWindow);
}

//HWND Window::GetWindow32Context() {
//    return glfwGetWin32Window(glfwWindow);
//}

void Window::UpdateInputs(const float& deltaTime)
{
    // Close the window if the exit key was pressed.
    if (inputKeys.exit > 0 && glfwGetKey(glfwWindow, inputKeys.exit) == GLFW_PRESS)
        Close();

    // Disable inputs if they are locked.
    if (inputsLocked)
    {
        inputs = WindowInputs();
        double mouseX; double mouseY;
        glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
        inputs.mousePos = { (float)mouseX, (float)mouseY };
        return;
    }
    
    // Get movement inputs.
    inputs.movement = {};
    if (inputKeys.movement[0] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[0])) inputs.movement.x += 1;
    if (inputKeys.movement[1] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[1])) inputs.movement.x -= 1;
    if (inputKeys.movement[2] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[2])) inputs.movement.y += 1;
    if (inputKeys.movement[3] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[3])) inputs.movement.y -= 1;
    if (inputKeys.movement[4] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[4])) inputs.movement.z -= 1;
    if (inputKeys.movement[5] > 0 && glfwGetKey(glfwWindow, inputKeys.movement[5])) inputs.movement.z += 1;
    
    if (inputKeys.specialKey[0] > 0 && glfwGetKey(glfwWindow, inputKeys.specialKey[0])) inputs.shiftpress = true;
    else inputs.shiftpress = false;
    if (inputKeys.specialKey[1] > 0 && glfwGetKey(glfwWindow, inputKeys.specialKey[1])) inputs.spaceBarPress = true;
    else inputs.spaceBarPress = false;
    if (inputKeys.specialKey[2] > 0 && glfwGetKey(glfwWindow, inputKeys.specialKey[2])) inputs.interactionPress = true;
    else inputs.interactionPress = false;

    // Get mouse button inputs.
    inputs.mouseLeftClick   = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT);
    inputs.mouseMiddleClick = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE);
    inputs.mouseRightClick  = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT);

    // Get mouse position and mouse delta.
    double mouseX; double mouseY;
    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
    const Maths::Vector2 prevMousePos = inputs.mousePos;
    const Maths::Vector2 mousePos     = { (float)mouseX, (float)mouseY };
    inputs.mouseDelta = mousePos - prevMousePos;
    inputs.mousePos   = mousePos;

    // Hide the mouse cursor and disable its movement when necessary.
    if (cursorLockedHidden)
    {
        glfwSetCursorPos(glfwWindow, (double)prevMousePos.x, (double)prevMousePos.y);
        inputs.mousePos = prevMousePos;
    }
}
