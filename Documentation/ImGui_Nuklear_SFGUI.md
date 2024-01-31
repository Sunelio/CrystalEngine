# Choice of User Interface API

### ImGui

Pros:
- Easy to use: ImGui provides a simple and intuitive API for creating user interfaces.
- Lightweight: ImGui is a relatively lightweight library that can be easily integrated into existing codebases.
- High-performance: ImGui is designed to be fast and efficient, and can render complex user interfaces quickly.
- Customizable: ImGui provides a wide range of customization options for widgets, including colors, fonts, and sizes.

Cons:
- Limited functionality: ImGui is primarily focused on creating user interfaces and doesn't provide as many features as some other libraries.
- C++ only: ImGui is written in C++ and may not be suitable for projects written in other programming languages.
- Not suitable for complex applications: ImGui is designed to be lightweight and may not be suitable for complex applications with a large number of widgets.

<br>

### Nuklear

Pros:
- Lightweight: Nuklear is a small, embeddable library that can be easily integrated into existing codebases.
- High-performance: Nuklear is designed to be fast and efficient, and can render complex user interfaces quickly.
- Easy to use: Nuklear provides a simple API for creating user interfaces.

Cons:
- Limited customization: Nuklear provides a limited number of customization options for widgets.
- Limited functionality: Nuklear is primarily focused on creating user interfaces and doesn't provide as many features as some other libraries.
- C only: Nuklear is written in C and may not be suitable for projects written in other programming languages.

<br>

### SFGUI

Pros:
- Comprehensive: SFGUI provides a wide range of widgets for creating complex user interfaces.
- Cross-platform: SFGUI is available on a wide variety of platforms, including Windows, macOS, and Linux.
- Customizable: SFGUI provides a wide range of customization options for widgets, including colors, fonts, and sizes.

Cons:
- Complex API: SFGUI's API can be somewhat complex, especially for beginners.
- Large codebase: SFGUI is a relatively large library that can add a significant amount of code to your project.
- Limited performance: SFGUI is designed to be flexible and customizable, which can sometimes come at the cost of performance.

<br>

### Conclusion

Compared to Nuklear and SFGUI, ImGui is focused on providing a simple and intuitive API for creating user interfaces. It is also relatively lightweight and provides a high level of customization for widgets. Additionally, ImGui is designed to be high-performance, which can be beneficial for complex applications. Since we are all very familliar with it, implementing user interface will be a fast and efficient process. 

<br>

Go back to [DesignDocument](DesignDocument.md)
