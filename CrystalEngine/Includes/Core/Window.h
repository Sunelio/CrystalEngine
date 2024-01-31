#pragma once
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"

typedef struct GLFWwindow GLFWwindow;               // Forward GLFW window context.
typedef void* (*ProcAddressFunc)(const char *name); // Forward OpenGL proc address function pointer.
struct HWND__; typedef HWND__* HWND;                // Forward Windows32 window context.

namespace Core
{
    // - WindowParams: holds window parameters - //
	struct WindowParams
    {
        const char* name = "";
        int  width       = -1;
        int  height      = -1;
        int  posX        = 50;
        int  posY        = 70;
    };

    // - WindowInputs: holds values of user inputs - //
    struct WindowInputs
    {
        Maths::Vector3 movement;
        Maths::Vector2 mousePos;
        Maths::Vector2 mouseDelta;
        bool mouseLeftClick   = false;
        bool mouseMiddleClick = false;
        bool mouseRightClick  = false;
        bool shiftpress       = false;
        bool spaceBarPress    = false;
        bool interactionPress = false;
    };

    // - InputKeys: holds keybindings for user inputs - //
    struct InputKeys
    {
        int exit;           // Key used to exit the engine.
        int movement[6]{};  // Right, Left, Up, Down, Fwd, Back.
        int specialKey[3]{}; // shift, space.

        InputKeys();
    };

    // - Window - //
    class Window
    {
    private:
        GLFWwindow*    glfwWindow;
        WindowParams   params;
        WindowInputs   inputs;
        InputKeys      inputKeys;
        Maths::Vector2 viewportSize;
        Maths::Vector2 viewportPos;
        bool           inputsLocked       = false;
        bool           cursorLockedHidden = false;
    
    public:
        // -- Constructor -- //
        Window(const WindowParams& windowParams); // Create a window with the specified window parameters.
        ~Window();
        Window(const Window&)            = delete;
        Window(Window&&)                 = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&)      = delete;

        static void InitGlfw();
        static void TerminateGlfw();
        static ProcAddressFunc GetProcAddressFuncPointer();

        // -- Miscellaneous -- //
        void LockInputs    (const bool&  locked);    // Prevents any inputs from registering until turned back on.
        void LockHideCursor(const bool& locked);     // Hides the mouse cursor and locks it in place.
        void Update        (const float& deltaTime); // Retrieves inputs from the user.
        void SwapBuffers   () const;                 // Sends the current framebuffer to the monitor for display.
        void Close         () const;                 // Closes the window.

        // -- Setters -- //
        void SetName   (const char* windowName);         // Sets the window's name.
        void SetWidth  (const int&  windowWidth ) const; // Sets the window's width.
        void SetHeight (const int&  windowHeight) const; // Sets the window's height.
        void SetSize   (const int&  windowWidth,  const int& windowHeight) const; // Sets the window's size.
        void SetPosX   (const int&  windowPosX)   const; // Sets the window's X position.
        void SetPosY   (const int&  windowPosY)   const; // Sets the window's Y position.
        void SetPos    (const int&  windowPosX,   const int& windowPosY) const; // Sets the window's position.
        void SetVsync  (const bool& windowVsync)  const; // Toggles vertical synchronization.
        void SetExitKey(const int& windowExitKey) { inputKeys.exit = windowExitKey; } // Sets the window's exit key.
        void RemoveExitKey()                      { inputKeys.exit = -1; } // Removes the window's exit key.
        void SetViewportSize(const Maths::Vector2 _viewportSize) { viewportSize = _viewportSize; }
        void SetViewportPos(const Maths::Vector2 _viewportPos) { viewportPos = _viewportPos; }

        // -- Getters -- //
        const char*    GetName()            const { return params.name;   } // Returns the window's name.
        int            GetWidth()           const { return params.width;  } // Returns the window's width.
        int            GetHeight()          const { return params.height; } // Returns the window's height.
        Maths::Vector2 GetSize()            const { return { (float)params.width,  (float)params.height }; } // Returns the window's size.
        float          GetAspect()          const { return   (float)params.width / (float)params.height;   } // Returns the window's aspect ratio.
        int            GetPosX()            const { return params.posX;   } // Returns the window's X position.
        int            GetPosY()            const { return params.posY;   } // Returns the window's Y position.
        Maths::Vector2 GetPos()             const { return { (float)params.posX, (float)params.posY }; } // Returns the window's position.
        Maths::Vector2 GetViewportSize()    const { return viewportSize;  } 
        Maths::Vector2 GetViewportPos()     const { return viewportPos;   } 
        WindowInputs   GetInputs()          const { return inputs;        } // Returns the window's inputs.
        GLFWwindow*    GetWindowContext()   const { return glfwWindow;    }
        HWND           GetWindow32Context() const;
        bool           ShouldClose()        const; // Returns true if the window sould close."

    private:
        void UpdateInputs(const float& deltaTime);
    };
}