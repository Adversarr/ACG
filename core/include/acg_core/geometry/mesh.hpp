#pragma once
#include <Eigen/Eigen>
#include <type_traits>

#include "../common.hpp"
#include "../math/common.hpp"

namespace acg::geometry {

/**
 * @brief Triangle Mesh Geometry.
 * TODO: Do not use this class any more!
 *
 * @tparam T the float point number in use. default = float (32-bit)
 */
template <typename T = F32> class SimpleMesh {
public:
  using VertexType = Vec3<T>;

  using VerticesType = Field<T, 3>;

  using FaceType = Vec3<Idx>;

  using FacesType = Field<Idx, 3>;

private:
  // Mesh should be column major, AoS layout.
  VerticesType vertices_;

  // the same for indice.
  FacesType faces_;

public:
  /**
   * @brief Construct a new Triangle Mesh object, the vertices and indices will be checked.
   *
   * @param vert
   * @param indi
   */
  SimpleMesh(const VerticesType& vert, const FacesType& indi);

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

  inline const VerticesType& GetVertices() const;

  inline const FacesType& GetFaces() const;

  inline SimpleMesh<T>& SetVertices(const Field<T, 3>& vertices) {
    assert(vertices_.cols() == vertices.cols());
    vertices_ = vertices;
    return *this;
  }

  inline SimpleMesh<T>& SetFaces(const Field<Idx, 3>& faces) {
    assert(faces_.cols() == faces.cols());
    faces_ = faces;
    return *this;
  }

  inline SimpleMesh<T>& Set(const Field<T, 3>& vertices, const Field<Idx, 3>& faces) {
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

template <typename T>
const typename SimpleMesh<T>::VerticesType& SimpleMesh<T>::GetVertices() const {
  return vertices_;
}

template <typename T> const typename SimpleMesh<T>::FacesType& SimpleMesh<T>::GetFaces() const {
  return faces_;
}

template <typename T>
SimpleMesh<T>::SimpleMesh(const VerticesType& vertices, const FacesType& indices)
    : vertices_(vertices), faces_(indices) {}

template <typename T> template <typename T2> SimpleMesh<T2> SimpleMesh<T>::Cast() const {
  return SimpleMesh<T2>{vertices_.template cast<T2>(), faces_};
}
}  // namespace acg::geometry
