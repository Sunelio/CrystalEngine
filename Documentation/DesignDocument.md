# Crystal Engine Design Document

### C++ Norms

To maintain consistent and organized code, we will follow the following naming conventions:

- #defined values, global constants, and global variables must be in ```UPPER_SNAKE_CASE``` to differentiate them from other variable types.
- Namespaces, classes, structs, enums, macros, functions, and methods must be in ```PascalCase```, with the first letter of each word capitalized.
- Class members, function/method parameters, and local variables must be in ```camelCase```, with the first letter of the first word in lower case and the first letter of each subsequent word capitalized.
- Function parameters that directly set class members should be named with an underscore prefix and the name of the member, such as ```_memberName```.
- Header (.h) files should be stored in an "Includes" folder, and source code (.cpp) files in a "Sources" folder.
- Both header and source code files should be stored in folders named according to their namespace(s).
- #includes in header files should be avoided by using forward declarations wherever possible.
- External library files should be stored in an "Externals" folder, with library header files in "Externals/Includes", library source files in "Externals/Sources", and library object files (.lib) in "Externals/Libs".

For more details on the project architecture, please refer to our [UML diagram](https://lucid.app/lucidchart/94085685-6d0d-4bb8-b546-9acec4b61f89/edit?view_items=dH1O2Q~ybK6f&invitationId=inv_96804935-6cd8-4687-b92e-bcf70612a535).


<br>

### Libraries

For this project, we have chosen to use the following libraries:

- ```GLFW```: an Open Source library for creating windows, contexts, and receiving input and events. Its API is simple, consistent and well known by every member of the group. GLFW also provides support a range of input devices, making it an ideal choice for a game engine project. See [this document](GLFW_SDL_Win32API.md) for details about our choice.

- ```OpenGL 4.5``` with ```glad```: an industry-standard graphics API that provides a low-level interface to the graphics hardware, allowing us to create and render 3D graphics for our game engine. We will be using ```glad``` to generate OpenGL loading code in a header file, since it is a foolproof quick and easy way to do it. See [this document](OpenGL_Vulkan_DirectX.md) for details about our choice.

- We have chosen not to use a physics library to learn to code advanced physics ourselves. See [this document](Bullet_PhysX.md) for details about our choice.

- ```STB Image```: a library for loading and manipulating image files that supports a wide range of file formats. It is incredibly lightweight (with a single header file), easy to use, and provides an efficient API for working with image data. We will probably only be using one of its functions to load texture data into the engine: ```stbi_load```.

- ```ImGui``` with Docking: a very user-friendly graphical user interface library that is widely used in game development. It provides an easy-to-use, fully customizable API for creating graphical user interfaces, and its docking addon allows for the user workspace to be easily rearranged. ImGui will enable us to create custom editor windows and debug tools, making it an essential component of our development process. An interactive manual for it can be found [here](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html). See [this document](ImGui_Nuklear_SFGUI.md) for details about our choice.

- ```Assimp```: Assimp (*Acquisition of Scene Data and Intelligence to the Importer*) is a C++ library that provides a simple and uniform interface for importing 3D models from various file formats into an application. It was created to solve the problem of importing 3D models from different file formats, which can be a complex and time-consuming task for developers. Assimp can import 40
different types of formats, including [OBJ](https://fr.wikipedia.org/wiki/Objet_3D_(format_de_fichier)), [FBX](https://fr.wikipedia.org/wiki/FBX) or [Collada](https://en.wikipedia.org/wiki/COLLADA). See [this document](Assimp_Autodesk.md) for details about our choice.

- ```pybind11``` and ```sol```: libraries for interfacing c++ code to python/lua so we can use them as a simple entry points to scripting in our engine. By using these libraries in particular, binding is very simple and everyone can understand the code supposed to bind each class of the engine. The design is very human and can be easily modified/maintained for long term project. Moreover we also have the luac api provided by the lua project to be able to create a lua library ```sol```. See [this document for python](Pybind11_Swig.md) and [this document for lua](Sol_tolua++_luabind.md) for details about our choice.

- ```MiniAudio```: a lightweight C++ library designed for audio playback and recording, offering a simple and intuitive API, cross-platform compatibility, and efficient performance. See [this document](MiniAudio_OpenAL_FMOD.md) for details about our choice.

In summary, the libraries we have selected for our project have been chosen for their compatibility, performance, ease of use, and powerful graphics capabilities. These libraries will help us to create a game engine that is both efficient and offers an easy game-making experience.
<br>

### Library Wrappers

In order to make our game engine more flexible and tidy, we will wrap external libraries in custom classes. This will allow us to abstract away the low-level details of the libraries and provide a more intuitive interface for game developers to work with. The following classes will be created to wrap the libraries:

- ```Window```: will be a wrapper to the ```GLFW``` library and provide methods to handle the window context and receive inputs.

- ```UserInterface```: ```ImGui``` will be wrapped within it. All user interface windows and widgets will be built, configured and rendered through this class.

- ```Texture```: though not a wrapper by design, it is the only class that will use ```STB Image``` and will only use one function from it. By extension, it wouldn't be worth it to create a whole wrapper for STBI and doesn't make the engine less flexible in any way.

- ```Renderer```: will wrap the ```OpenGL``` library and provide a simplified interface for sending all relevant resource data to the GPU, as well as handling the rendering of game objects. This will make interaction between our custom engine systems and OpenGL way cleaner, and will make working with the engine easier overall.

- ```AssimpParser```: will be wrapping the ```Assimp``` library and provide a single method to load a model file. 

- ```PythonWrapper``` and ```LuaWrapper```: will respectively wrap ```pybind11``` and ```sol``` to enable quick and easy binding of the engine in separate visual studio projects.

In summary, wrapping the chosen libraries in custom classes will make our C++ game engine project more flexible and easier to use. The classes will provide a simplified and intuitive interface for game developers, which will make it easier to create and manage window contexts, game assets, scene objects, user interfaces, and rendering.

<br>

### Render Hardware Interface

The render hardware interface (named Renderer in our project) is the only class that can directly call functions from the rendering API. Therefore, it acts as an interface for the rest of the codebase. Here are its methods:

- ```template <typename T> void SendResourceToPipeline(T* resource)```: <br>
    Stores the given resource (texture, shader, shader program...) into GPU memory.

- ```template <typename T> void DeleteResource(T* resource)```: <br>
    Removes the given resource's data from GPU memory.

- ```void UseShaderProgram(const Resources::ShaderProgram* shaderProgram)```: <br>
    Starts using the given shader program.

- ```template <typename T> void SetShaderVariable(const Resources::ShaderProgram* shaderProgram, const char* name, const T& val)```: <br>
    Sets the value of the specified shader variable.

- ```void BindRenderTexture(const Resources::RenderTexture* renderTexture)```: <br>
    Starts using the given render texture to draw objects.

- ```void UnbindRenderTexture()```: <br>
    Starts drawing objects directly to the screen texture.

- ```void ClearCurRenderTexture(const Maths::RGBA& color)```: <br>
    Fills the current render texture with the given color.

- ```void DrawSubMesh(const Resources::SubMesh* subMesh, const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager)```: <br>
    Renders the given sub-mesh using the specified model transform matrix, camera and light manager.

- ```uint GetTextureSamplerId()```: <br>
    Returns the index of the texture sampler (necessary to sample textures within shaders).

- ```void ResizeViewport(const int& width, const int & height)```: <br>
    Resizes the OpenGL vieweport to the given width and heiht.

- ```void CompileShader(uint id, ResourceTypes type, char* curFile, char* curFunction, long curLine)```: <br>
    Private method. Compiles any shader using the given ID. The resource type is used to determine what type of shader it should be compiled as (vertex or fragment). The other parameters are used for debug and assertions.

<br>

### Update & render loop

During execution of the engine program, the main thread performs scene updates and renders in a loop. Here is the pseudocode for it:
```cpp
// Call start methods of object scripts.
sceneManager.StartActiveScene();

while (not window.ShouldClose())
{
    // Don't go faster than the target FPS.
    WaitUntilNextFrame();

    // Update window events and inputs.
    window.Update();

    // Whenever a resource is loaded, send it to GPU memory (this is useful with multithreaded loading).
    resourceManager.SendLoadedResources();

    // Update the currently active scene (the scene manager handles edit/play modes).
    sceneManager.UpdateActiveScene(deltaTime, inputs);

    // Begin rendering.
    renderer.ClearFramebuffer();
    ui.NewFrame();

    // Render the currently active scene and the user interface.
    sceneManager.RenderActiveScene(renderer);
    ui.Render();

    // End rendering.
    ui.EndFrame();
    window.SwapBuffers();
}
```
