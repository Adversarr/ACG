## Core Features to Implement

Reference: Taichi Lang.

### Camera

Set the frustum Clipping Bottom Plane.

```
Camera::SetBottom(float bottom)
```

Set the fov:

```cpp
Camera::SetFov(float angle);
```

Get the Projection Matrix:

```cpp
Matrix3x3 Camera::GetProjection(float aspect_ratio) const;
```

Get view matrix:

```cpp
Camera::GetViewMatrix() const;
```

Set the left clipping plane:

```cpp
Camera::SetLeft(float );
```

Set Look at point:

```cpp
Camera::SetLookAt(Vec3f p);
```

Set Position:

```cpp
Camera::SetPosition(Vec3f p);
```

Set projection mode:

```cpp
Camera::SetProjectionMode(bool enable_perspective=true);
```

Set top clip:

```cpp
Camera::SetTop(float );
```

Set right clip

```cpp
Camera::SetRight(float);
```


Set Up:

```cpp
Camera::SetUp(Vec3f p);
```

Set Z-Far:

```cpp
Camera::SetZFar(float far);
```

Set Z-Near

```cpp
Camera::SetZNear(float near);
```

### Canvas

> This is the context manager for managing drawing commands on a window. You should not instantiate this class directly via `__init__`, instead please call the `GetCanvas()` method of Window.

```cpp
// Actually, The renderer takes the canvas for rendering.
class Canvas {
public:
  // if colors.cols() == 1, then per_vertex_color will be applied.
  void DrawCircles(AttrVec<float, 3> centers, AttrVec<float, 3> radius, AttrVec<float, 3> colors);
  // if colors.cols() == 1, then per_vertex_color will be applied.
  void DrawLines(AttrVec<float, 3> vertices, AttrVec<float, 1> width,
    AttrVec<Idx, 2> indices, AttrVec<float, 3> colors);
  // Each canvas has zero or one unique scene for rendering.
  unique_ptr<Scene>& GetScene();
};
```

### Scene

has light, and some physical objects

### World, Window, GUI

- World: the class for deriving, has one (or at least one in future?) window. 
- Window: 
- The ui interface class, can draw.


## Todo List

- [x] class `Camera`
- [x] class `WorldController`
- [x] class `ParticleSystem` `MeshGrid`, and corresponding port from `acg::core`
- [ ] Impl the core features

## Future

- [ ] Vertex buffer may be synced between CPU hand GPU every frame.
- [ ] Add marching cube support.
