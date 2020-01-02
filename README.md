# odin-assimp

This is a sanitized C wrapper around basic Assimp functionality in addition to Odin bindings to this wrapper. These bindings accept Odin slices, but the data will be allocated by the wrapper library and need to be manually freed by it.

With a dev console (x64 Native Tools Command Prompt for VS 2017), with `git` and `cmake` in your system PATH
```
cd scripts
get_assimp.bat
build_lib.bat
```

This will build Assimp in Release mode with all importers and exporters explicitly disabled, and only OBJ and FBX importers enabled. For other setups and formats, simply modify `get_assimp.bat`.

Run `meshloader.odin` to test when built successfully.

**Warning:** Only loads vertices and vertex indices from either an .obj or .fbx file. If you need more it should be simple to modify the wrapper in `src/meshloaded_lib.cpp` to account for it during mesh traversal.

Also see https://github.com/DrProfesor/odin-assimp for an incomplete Odin binding to the C-API of Assimp.
