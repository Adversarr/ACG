NOTE: This project is highly expremental.

# Adversarr's Computer Graphics toolbox

ACG is the toolbox designed for study use (mainly about Physical Simulation, not Rendering).



**Project Structure**: 

1. Core:
   1. General Utilities:
      1. `log`: based on SPDLOG
      2. `god`: a meta programming library for Compile Time Computing
      3. `json`: import nlohmann json.
      4. lots of tiny staff: such as `Result<T>`, unified & cross-platform time module, etc.

   2. Init: Hookers for global variables designed for shared/static libraries.
   3. Mathematics Tools: Import `Eigen`, and defines some types, such as `Field`, `TriangleList`.
   4. Parallel computing tools: Import `Taskflow`
   5. SAD(Static Auto Diff): a high performance and highly optimized **compile-time** AutoDiff Library.
   6. …

2. Gui:
   - (Finished) Scene visualization: Support Mesh(including instancing), Particles, and Wireframe.
   - (Finished) ImGUI & Vulkan based. high performance
   - (Finished) ImGUI based User Interface, and User Callback.
   - Scene Editing/Storaging/...
3. Port: Data import module.
4. Plugins (not implemented)
   - libigl/cgal/... integration.
5. Examples, see below.


## Examples

This repo is designed for study use. Some implemented (just demos, not optimized, debugged) algorithms are listed below:

### mass_spring

mass-spring system based on Projective Dynamics. Refer to "Fast mass spring" by Tiantian Liu.

![image-20230224132002368](./media//image-20230224132002368.png)

### Other Stuffs

1. `fem-explict`: first-order explicit integrator. StVK FEM Soft Body Simulation.
2. `mpm-explicit`: PIC-Fluid simulation, toy P2G and G2P process, weakly-compressible.



## Todo Lists

### Build System

- [x] Transfer everything to `xmake`

### Core

- [ ] import some function from libigl
- [ ] Half Edge data structure for high performance neighbour query.
- [ ] Spatial Data structure
- [ ] Math libraries
- [ ] SAD: working in progress, static-auto-diff
- [x] Tensor Iterator Support

### Gui

- [ ] Gui: Non blocking when performing compute – almost impossible (everything of glfw should be processed in MAIN thread.)

