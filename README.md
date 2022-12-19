# Adversarr's Computer Graphics toolbox

**Project Structure**: 

1. Core: Utilities/Mesh/Math/Data Structures/...
3. Gui:
   - Scene visualization
   - Scene Editing/Storaging/...
   - ImGUI & Vulkan based. high performance, no block when rendering.(WIP)
4. Plugins (not implemented)
   - libigl/cgal/... integration.
5. Examples.
6. ... (WIP)


## Example Usage

See `examples/nbody`.

## Todo Lists

### Tops

[x] remove coost. (doctest/nloh.._json)

### Build System

- [x] (Important!) Transfer everything to `xmake`

### Core

- [ ] import some function from libigl
- [ ] Half Edge data structure for high performance neighbour query.
- [ ] Spatial Data structure
- [ ] Math libraries
- [ ] SAD: working in progress, static-auto-diff
- [ ] Tensor Iterator Support

### Gui

- [ ] MT-Gui: Non blocking when performing compute.

