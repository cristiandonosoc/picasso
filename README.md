# Picasso

Picasso is simply a software renderer in order to play around with simulating OpenGL, doing ray-tracing, multi-threading, etc.

## Building

For now, I'm trying CMake (cannot say I'm thrilled). Remember to do an out-of-source build

```
mkdir build
cd build
cmake ..
cmake --build .
```

So that CMake doesn't polute your main directory with garbage. To clean should be an easy `rm -rf build`

#### Windows

Windows needs a little more help finding the SDL2 libraries. How the build system is setup now is that it will look for the `SDL2` environment variable that should have the path to the base directory of the extracted SDL2 development libraries.

What I did is to create a `.set_env.bat` that the only thing it does is set that environment variable. Of course, as that path is only relevant to me, I didn't check in that file.
