# Choice of Window and Context API

### SDL

Pros:
- Comprehensive: SDL provides a comprehensive API for window and context management, as well as support for audio, input, networking, and other features.
- Cross-platform: SDL is available on a wide variety of platforms, including Windows, macOS, Linux, and mobile platforms.
- Mature ecosystem: SDL has been around for a long time and has a large community and a vast number of resources available.
- Good performance: SDL is designed to be fast and efficient.

Cons:
- Complex API: SDL's API can be somewhat complex, especially for beginners.
- Large codebase: SDL is a relatively large library that can add a significant amount of code to your project.

<br>

### Win32 API

Pros:
- High level of control: The Win32 API provides a high level of control over window and context management, which can be useful for advanced applications.
- Integration with Windows: The Win32 API is tightly integrated with the Windows operating system and provides access to a wide variety of Windows-specific features.
- Low-level: The Win32 API is a low-level API that provides fine-grained control over the Windows operating system.

Cons:
- Steep learning curve: The Win32 API can be difficult to learn, especially for beginners.
- Windows-specific: The Win32 API is specific to the Windows operating system and is not cross-platform.
- Requires a lot of code: Using the Win32 API requires a significant amount of code, which can be time-consuming and error-prone.

<br>

### GLFW

Pros:
- Easy to use: GLFW provides a simple and straightforward API for creating windows, handling user input, and creating OpenGL or Vulkan contexts.
- Cross-platform: GLFW is available on a wide variety of platforms, including Windows, macOS, and Linux.
- Lightweight: GLFW is a relatively lightweight library that doesn't require a lot of additional dependencies.
- Good documentation: GLFW has well-written documentation and a helpful community.

Cons:
- Limited functionality: GLFW is primarily focused on window and context management and doesn't provide as many features as some other libraries.
- No support for DirectX: GLFW only supports OpenGL and Vulkan, so it may not be suitable for projects that require DirectX.

<br>

### Conclusion

We have decided to go for GLFW because it is a lightweight, easy-to-use library with cross-platform support. It is only focused on window, context and input management, with an emphasis on being user-friendly. Using it over other APIs will speed up development time since the amount of code it requires is quite small.

<br>

Go back to [DesignDocument](DesignDocument.md)
