# [![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&pause=1000&random=false&width=600&lines=Yadro+Winter+Hack+2025)](https://git.io/typing-svg)
This repository contains some pet-projects and material from the Yadro Winter School 2025: optimizations for dense matrices on GPU. It supports cross-platform building with meson. 

# Building
To configure and build the project, do the following from the root directory of the project:
```bash
meson setup build
cd build
ninja
```
This will configure the Meson and will generate platform-specific build files. To execute a specific module (e.g. 'vec_sum') just run from within the build directory such command:
```bash
./vecsum
```
