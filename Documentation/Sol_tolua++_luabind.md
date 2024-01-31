# Choice of Lua binding API

### Sol

Pros:
- Simple and easy-to-use interface for C++ developers
- Lightweight design makes it easy to integrate with existing C++ projects
- Supports multiple Lua versions
- Extensive documentation available
- Automatic garbage collection and type checking

Cons:
- Limited additional features compared to other libraries
- Some users have reported issues with compatibility with certain compilers

<br>


### tolua++

Pros:
- Provides additional features, such as class inheritance and automatic conversion of C++ objects to Lua types
- Large community and established user base
- Active development and maintenance

Cons:
- More complex to use and has a steeper learning curve than Sol
- Heavier weight than Sol, which can impact integration with existing C++ projects
- Not as extensive documentation as Sol

<br>


### luabind
Pros:
- Supports automatic binding of C++ functions to Lua
- Allows for binding of C++ classes to Lua tables

Cons:
- No longer under active development
- Documentation is limited
- Less feature-rich than Sol or Luabind

<br>


### Conclusion

Overall, Sol is a great choice for use looking for a lightweight and easy-to-use library for binding Lua. Luabind provides additional features but comes with a steeper learning curve and more complex integration. Lastly, while tolua++ has some useful features, it is no longer under active development and may not be the best choice for long-term projects.

<br>

Go back to [DesignDocument](DesignDocument.md)
