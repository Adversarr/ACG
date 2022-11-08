#pragma once
#include "../core.hpp"
#include <Eigen/Eigen>
#include <type_traits>


namespace acg::geometry {

/**
 * @brief Triangle Mesh Geometry.
 *
 * @tparam T the float point number in use. default = float (32-bit)
 */
template <typename T = F32> class Mesh {
public:
  using VerticeType = Eigen::Vector3<T>;

  using StateType = Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor>;

  using FaceIndexType = Eigen::Vector3<Idx>;

  using FaceListType = Eigen::Matrix<
      /* ElemType */ Idx,
      /* #Rows */ 3,
      /* #Cols */ Eigen::Dynamic,
      /* Layout */ Eigen::ColMajor>;
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

  /**
   * @brief Get the #Vertices
   *
   * @return int
   */
  inline Idx GetNumVertices() const;

  /**
   * @brief Get the #Edges
   *
   * @return int
   */
  inline Idx GetNumEdges() const;

  inline const StateType& GetVertices() const;

  inline const FaceListType& GetIndices() const;

private:
  Idx num_vertices_{0};

  Idx num_edges_{0};

  // Mesh should be column major, AoS layout.
  StateType vertices_;

  // the same for indice.
  FaceListType faces_;

};

template<typename T> 
const typename Mesh<T>::StateType& Mesh<T>::GetVertices() const {
  return vertices_;
}


template<typename T> 
const typename Mesh<T>::FaceListType& Mesh<T>::GetIndices() const {
  return faces_;
}

template<typename T>
Mesh<T>::Mesh(const StateType& vertices, const FaceListType& indices) :
  vertices_(vertices),
  faces_(indices) { }



}  // namespace acg::geometry
