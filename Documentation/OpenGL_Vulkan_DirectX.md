# Choice of Rendering API

### Vulkan

Pros:
- Cross-platform: Vulkan is available on a wide variety of platforms, including Windows, macOS, Linux, and mobile platforms.
- High performance: Vulkan is designed to be extremely low-overhead and high-performance, making it a good choice for demanding applications.
- Multi-threading support: Vulkan is designed to take advantage of multi-core CPUs and can be used to offload work to the GPU, making it well-suited to modern hardware.
- Low-level control: Vulkan provides fine-grained control over the rendering pipeline, making it possible to achieve high levels of performance and efficiency.

Cons:
- Steep learning curve: Vulkan has a relatively steep learning curve compared to some other rendering APIs, due to its low-level nature and complex architecture.
- More work: Because Vulkan provides such low-level control, it requires more work to implement basic functionality compared to some other rendering APIs.
- Limited ecosystem: Vulkan is a relatively new API and doesn't have the same level of community support and resources as some other rendering APIs.

<br>

### DirectX

Pros:
- Tight integration with Windows: DirectX is tightly integrated with the Windows operating system and is often the preferred choice for Windows-specific applications.
- Large feature set: DirectX supports a wide variety of features, from basic 2D rendering to advanced 3D effects.
- Comprehensive documentation: Microsoft provides comprehensive documentation and support for DirectX, making it relatively easy to get started with.

Cons:
- Windows-only: DirectX is only available on the Windows operating system, making it less suitable for cross-platform applications.
- Limited multi-threading support: DirectX doesn't support multi-threading as well as some other rendering APIs, which can make it more difficult to take full advantage of modern hardware.
- Closed-source: DirectX is a proprietary API and isn't as open as some other rendering APIs.

<br>

### OpenGL

Pros:
- Cross-platform: OpenGL is available on a wide variety of platforms, including Windows, macOS, Linux, and mobile platforms.
- Mature ecosystem: OpenGL has been around for a long time and has a large community and a vast number of resources available.
- Flexible pipeline: OpenGL allows for a lot of customization and flexibility in the rendering pipeline.
- Large feature set: OpenGL supports a wide variety of features, from basic 2D rendering to advanced 3D effects.

Cons:
- Performance overhead: OpenGL has a relatively high performance overhead compared to some other rendering APIs, which can be a bottleneck in some applications.
- Limited multi-threading support: OpenGL doesn't support multi-threading as well as some other rendering APIs, which can make it more difficult to take full advantage of modern hardware.
- Limited low-level control: OpenGL is a relatively high-level API, which can make it more difficult to achieve fine-grained control over the rendering process.

<br>

### Conclusion

Compared to Vulkan or DirectX, OpenGL is less complex, which, on the good side, means handling it will cause us significantly less headaches. This lower complexity does mean we have less control over GPU memory and optimizations, which means our project will run a bit slower than it would have been with even lower-level graphics libraries. This is a disadvantage we are willing to take since the familliarity we have with OpenGL and its relative ease of use compared to Vulkan and DirectX is worth the performance difference. The fact that we don't have much experience with Vulkan and DirectX means that we probably wouldn't be able to optimize our rendering significantly better than OpenGL.

<br>

Go back to [DesignDocument](DesignDocument.md)
