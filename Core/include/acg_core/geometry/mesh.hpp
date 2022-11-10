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
template <typename T = F32> class Mesh {
public:
  using VerticeType = Vec3<T>;

  using StateType = AttrVec<T, 3>;

  using FaceType = Vec3<Idx>;

  using FaceListType = AttrVec<Idx, 3>;
  /**
   * @brief Construct a new Triangle Mesh object, the vertices and indices will be checked.
   *
   * @param vert
   * @param indi
   */
  Mesh(const StateType& vert, const FaceListType& indi);

  /**
   * @brief Construct a triangle mesh object, with no vertices and indices initialized. Will return
   * false when call `IsGood` right after.
   *
   * @warning Double Check if you really need this constructor.
   */
  Mesh() = default;

  /**
   * @brief Enable Copy Constructor
   *
   */
  Mesh(const Mesh&) = default;

  inline const StateType& GetVertices() const;

  inline const FaceListType& GetFaces() const;

  template<typename T2>
  inline Mesh<T2> Cast() const;

  /**
   * @brief Get the #Vertices
   *
   * @return int
   */
  inline Idx GetNumVertices() const {return GetVertices().cols();}

private:
  // Mesh should be column major, AoS layout.
  StateType vertices_;

  // the same for indice.
  FaceListType faces_;
};

template <typename T> const typename Mesh<T>::StateType& Mesh<T>::GetVertices() const {
  return vertices_;
}

template <typename T> const typename Mesh<T>::FaceListType& Mesh<T>::GetFaces() const {
  return faces_;
}

template <typename T> Mesh<T>::Mesh(const StateType& vertices, const FaceListType& indices)
    : vertices_(vertices), faces_(indices) {}


template<typename T>
template<typename T2>
Mesh<T2> Mesh<T>::Cast() const{
  return {vertices_.template cast<T2>(), faces_};
}
}  // namespace acg::geometry
