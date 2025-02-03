# [![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&pause=1000&random=false&width=600&lines=Yadro+Winter+School+2025)](https://git.io/typing-svg)
![image](https://github.com/De-Par/Yadro_RISC-V_winter_2025/blob/main/img/risc-v-school.png)

This repository contains some pet-projects and material from Yadro Winter School 2025: optimizations for dense matrices on GPU. It supports cross-platform building with meson. 

# Prerequisites
This is a list of modules required to launch the project:
* Meson
* OpenCL
* C/C++

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
