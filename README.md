1. `git clone --recursive git@github.com:themark147/cmake-gl-game.git`

2. #include <chrono> - to DefaultLogger in reactphysics3d


### Add new submodule to lib (only if submodule contains CMakefile)

- `git submodule add https://github.com/ocornut/imgui`

### ImGui added as part of project (https://github.com/ocornut/imgui/wiki/Getting-Started#compilinglinking)

- Recommended by ImGui this approach

Add files to your project or build system of your choice, so they get compiled and linked into your app.
Add all source files in the root folder: imgui/{*.cpp,*.h}.
Add selected imgui/backends/imgui_impl_xxxx{.cpp,.h} files corresponding to the technology you use from the imgui/backends/ folder
(e.g. if your app uses SDL2 + DirectX11, add imgui_impl_sdl2.cpp, imgui_impl_dx11.cpp etc.).
If your engine uses multiple APIs you may include multiple backends.

problem with physics3D - https://github.com/DanielChappuis/reactphysics3d/pull/421
