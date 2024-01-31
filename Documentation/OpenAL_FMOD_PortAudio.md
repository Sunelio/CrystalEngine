# Choice of Audio API

### FMOD

Pros:
- Advanced features such as 3D sound and DSP effects
- Supports a wide variety of platforms
- Well-documented with a large community of users
- Commercial license with dedicated support

Cons:
- Commercial license can be costly for some projects
- Limited access to source code
- Can be complex to set up and use, especially for beginners

<br>

### PortAudio

Pros:
- Cross-platform support
- Simple and lightweight library
- Low-latency audio input and output
- Open source license

Cons:
- Limited features compared to other libraries
- No built-in support for 3D sound or spatial audio
- Can require additional libraries or extensions for certain features

<br>

### OpenAL

Pros:
- Cross-platform 3D audio API
- Provides spatial sound effects for immersive audio experiences
- Supports multiple sound sources and playback options
- Open source license

Cons:
- Can be complex to set up and use, especially for beginners
- Limited support for non-3D audio effects
- Smaller community compared to other libraries, with fewer resources and support available

<br>

### Conclusion

We chose OpenAL over PortAudio and FMOD because our project required spatial audio and 3D sound effects to create an immersive audio experience for the user. OpenAL provides these features, allowing us to create a more engaging and realistic sound environment. Additionally, OpenAL is cross-platform, meaning it can be used on a variety of operating systems, and has an open source license, making it a great option for our project.

<br>

Go back to [DesignDocument](DesignDocument.md)
