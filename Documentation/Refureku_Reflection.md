## Choice of Refureku


## Refureku

Pros:
 - Is a powerful C++17 runtime refection dynamic library.
 - It  compiles with "msvc" or "clang++" or even "g++".
 - The library doesn't impose its own macro syntax: the user is free to define it.
 - Highly flexible and customizable code generation
 - Reflect namespaces, classes, (static) methods, (static) fields, variables, functions, enums and enum values.
 - It has a dynamic library containing the refelection API that is linked to the target program.
 - Using Refureku x Kodgen, the user has full control over the generated files / macros names to make the reflection integrate well in any project. 


Cons:
 - (MSVC only) Issues when reflecting class templates with only template template parameters, see this issue.
 - (MSVC only) Compilation error when reflect protected/private nested class templates.
 - If a reflected namespace is shared among different dynamic libraries, specific properties bound to each
    of those libraries are not removed when they are unloaded.


<br>


<br>

### Conclusion


<br>

## BIBLIOGRAPHIE
- https://github.com/jsoysouvanh/Refureku



Go back to [DesignDocument](DesignDocument.md)