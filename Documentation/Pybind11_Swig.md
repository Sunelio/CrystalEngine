# Choice of Python binding API

### Pybind11

Pros:
- Installation is straightforward, just a simple addition of includes and libs.
- Module/binding definitions are clearer.
- Easier to grasp, most problems often arise from Python.
- Better online documentation (more information, cleaner official website, search bar).

Cons:
- Manually defining the module and binding of each function from C++ is a time-consuming task.
<br>


### Swig

Pros:
- Automatically generates C++ code in Python (supports class definitions, constructor/destructor, inheritance, virtual functions, etc...), which theoretically makes the work faster.
- Passes complex/class types by reference, no need to redefine them/no "unrecognized type".

Cons:
- Construction or generation of an interface file for each library.
- Requires corrections for each module according to the library/header, for various errors. For example:
- More complex installation/configuration on Windows (adding an environment variable and redefining the path of libraries).
- Errors are highly specific to the project, which makes the search longer, as the issues solved online are also very specific.

<br>


### Conclusion

Based on the pros and cons listed for each library, we chose to use pybind11 instead of swig.

The straightforward installation process, clearer module/binding definitions, and better online documentation of pybind11 made it easier for us to learn and use. Additionally, while manually defining the module and binding of each function from C++ can be a time-consuming task, the other benefits of pybind11 outweigh this con.

On the other hand, while swig can automatically generate C++ code in Python and pass complex/class types by reference, the cons of constructing or generating an interface file for each library, requiring corrections for each module according to the library/header, and more complex installation/configuration on Windows have made it less appealing to us. Additionally, the highly specific nature of errors and issues with swig may have made it more difficult to find solutions online.

<br>

Go back to [DesignDocument](DesignDocument.md)
