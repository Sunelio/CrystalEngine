# Choice of Parsing Library

### Assimp

Pros:
- Support for a wide range of 3D file formats, including many popular formats like OBJ, FBX, Collada, 3DS, and more.
- Simple and uniform API for loading 3D models, regardless of the original file format.
- Advanced features like collision detection, camera detection, light detection, and scene hierarchy management.
- Ability to convert data from different file formats into a common internal format, which can improve performance and simplify the code needed to load models from different sources.
- Open-source and actively maintained, with a large community of contributors and users.

Cons:
- The library is written in C++, which may not be ideal for developers working in other languages.
- The library may have a steep learning curve for developers who are not familiar with 3D graphics programming or file formats.
- The library does not provide support for editing or creating 3D models; it is only designed for importing existing models.
- Some file formats may not be fully supported or may have limited functionality compared to other libraries or tools that are specifically designed for that format.
- The library may add some overhead to the application, especially when loading large models or using advanced features.

<br>

### Autodesk FBX SDK

Pros:
- Provides support for importing and exporting 3D models in the FBX file format, which is widely used in the game development, film, and animation industries.
- Provides a wide range of features for manipulating and optimizing 3D models, including tools for merging, splitting, scaling, and transforming meshes and objects.
- Provides support for animations, including blending, looping, and keyframe interpolation.
- Can be used in conjunction with other Autodesk tools like Maya and 3ds Max for seamless integration into a production pipeline.
- Provides a robust and well-documented API, with extensive documentation and support available from Autodesk.

Cons:
- The Autodesk FBX SDK is a proprietary library, which may not be ideal for developers who prefer open-source software.
- The library is relatively complex and may have a steep learning curve for developers who are not familiar with 3D graphics programming or file formats.
- The library is primarily focused on the FBX file format and may not provide the same level of support for other file formats.
- The library may have some licensing restrictions or fees associated with it, depending on the specific use case and usage requirements.
- The library may add some overhead to the application, especially when loading or manipulating large models or using advanced features.

<br>

### Conclusion

After careful consideration, we chose Assimp over Autodesk FBX SDK. The main reason was that Assimp is a versatile and widely-used library for importing and processing 3D models in a variety of file formats, and the facts that Autodesk is closed-source and has less file extensions supported made our choice easier.

Go back to [DesignDocument](DesignDocument.md)
