#pragma once
#include <Eigen/Eigen>
#include <type_traits>

#include "../core.hpp"
#include "../math.hpp"

namespace acg::geometry {

/**
 * @brief Triangle Mesh Geometry.
 *
 * @tparam T the float point number in use. default = float (32-bit)
 */
template <typename T = F32> class SimpleMesh {
public:
  using VerticeType = Vec3<T>;

  using StateType = AttrVec<T, 3>;

  using FaceType = Vec3<Idx>;

  using FaceListType = AttrVec<Idx, 3>;

private:
  // Mesh should be column major, AoS layout.
  StateType vertices_;

  // the same for indice.
  FaceListType faces_;

public:
  /**
   * @brief Construct a new Triangle Mesh object, the vertices and indices will be checked.
   *
   * @param vert
   * @param indi
   */
  SimpleMesh(const StateType& vert, const FaceListType& indi);

  /**
   * @brief Construct a triangle mesh object, with no vertices and indices initialized. Will return
   * false when call `IsGood` right after.
   *
   * @warning Double Check if you really need this constructor.
   */
  SimpleMesh() = default;

  /**
   * @brief Enable Copy Constructor
   *
   */
  SimpleMesh(const SimpleMesh&) = default;

  SimpleMesh(SimpleMesh&&) = default;

  SimpleMesh& operator=(const SimpleMesh&) = default;

  inline const StateType& GetVertices() const;

  inline const FaceListType& GetFaces() const;

  inline SimpleMesh<T>& SetVertices(const AttrVec<T, 3>& vertices) {
    assert(vertices_.cols() == vertices.cols());
    vertices_ = vertices;
    return *this;
  }

  inline SimpleMesh<T>& SetFaces(const AttrVec<Idx, 3>& faces) {
    assert(faces_.cols() == faces.cols());
    faces_ = faces;
    return *this;
  }

  inline SimpleMesh<T>& Set(const AttrVec<T, 3>& vertices, const AttrVec<Idx, 3>& faces) {
    vertices_ = vertices;
    faces_ = faces;
    return *this;
  }

  template <typename T2> inline SimpleMesh<T2> Cast() const;

  /**
   * @brief Get the #Vertices
   *
   * @return int
   */
  inline Idx GetNumVertices() const { return GetVertices().cols(); }

  inline Idx GetNumFaces() const { return faces_.cols(); }
};

template <typename T> const typename SimpleMesh<T>::StateType& SimpleMesh<T>::GetVertices() const {
  return vertices_;
}

template <typename T> const typename SimpleMesh<T>::FaceListType& SimpleMesh<T>::GetFaces() const {
  return faces_;
}

template <typename T> SimpleMesh<T>::SimpleMesh(const StateType& vertices, const FaceListType& indices)
    : vertices_(vertices), faces_(indices) {}

template <typename T> template <typename T2> SimpleMesh<T2> SimpleMesh<T>::Cast() const {
  return {vertices_.template cast<T2>(), faces_};
}
}  // namespace acg::geometry
