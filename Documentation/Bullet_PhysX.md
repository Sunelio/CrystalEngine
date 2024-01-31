# Choice of Physics API

### PhysX

Pros:
- High-performance: PhysX is designed for high-performance physics simulation and can be used for complex simulations with large numbers of objects.
- GPU acceleration: PhysX has strong support for GPU acceleration, which can greatly improve performance in certain situations.
- Integrated with popular game engines: PhysX is integrated with popular game engines like Unreal Engine and Unity, which can make it easier to use for game development.

Cons:
- Proprietary: PhysX is a proprietary library owned by NVIDIA, which means it is not open source and may have restrictions on how it can be used.
- Limited platform support: PhysX is primarily designed for Windows and consoles, which can limit its use on other platforms.
- Limited flexibility: PhysX is primarily focused on rigid body dynamics and may not be suitable for more complex physics simulations like fluid simulation.

<br>

### Bullet

Pros:
- Open source: Bullet is an open-source library that can be used and modified freely.
- Cross-platform: Bullet is available on a wide variety of platforms, including Windows, macOS, Linux, and Android.
- Flexible: Bullet can be used for a variety of physics simulations, including rigid body dynamics, soft body dynamics, and fluid simulation.
- Well-documented: Bullet is well-documented and has a large community of users, which can make it easier to find help and resources when working with the library.

Cons:
- Steep learning curve: Bullet can be difficult to learn, especially for beginners without much experience in physics simulation.
- Limited support for GPU acceleration: Bullet's GPU acceleration support is limited compared to other physics libraries, which can affect performance in certain situations.
- Limited integration with game engines: Bullet has limited integration with popular game engines, which can make it difficult to use for game development.

<br>

### Conclusion

After careful consideration, we chose to implement physics ourselves, running on the CPU. It is a bad choice in terms of performance and precision, but the choice was more about this being a learning experience on how to do advanced physics with rigid body dynamics, raycasts, physically simulated particules and more.

<br>

Go back to [DesignDocument](DesignDocument.md)
