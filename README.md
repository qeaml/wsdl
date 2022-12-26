# WSDL

**WSDL** is a header-only C++ wrapper for SDL2 which uses object lifetimes to
automatically deinitialize everything allocated.

*Note: This header assumes `<SDL2/SDL.h>` is the SDL2 header. You must manually
       change it if it's located elsewhere.*

## Example

An example of how WSDL can be used is found in the [`example.cpp`](example.cpp)
file. Note that the user does not have to manually destroy the render and window
or quit SDL.

You can compile this to an executable by running one of the `build_example`
scripts: e.g. [`build_example.bat`](build_example.bat) for MSVC.

## Compiling to an object file

If you wish to link the implementation as a separate object file, simply compile
the [`wsdl.cpp`](wsdl.cpp) file and link it.
