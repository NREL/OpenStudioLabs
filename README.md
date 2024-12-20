# OpenStudio Labs

This is a place to experiment with ideas that might apply to OpenStudio.
At the moment the main thing is to develop a prototype of supporting Python and Ruby in
a single OpenStudio Workflow.

Run the example program located in the build directory `<build>/Products/openstudio`.

I built Ruby as shared via rbenv, so Ruby_LIBRARIES=/home/julien/.rbenv/versions/3.2.2/lib/libruby.so

```
conan install . --output-folder=build --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release
cmake --preset conan-release
cmake --build --preset conan-release
```
