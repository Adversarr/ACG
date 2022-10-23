#pragma once
#include "../core.hpp"
#include <Eigen/Eigen>



namespace acg::geometry {

/**
 * @brief Triangle Mesh Geometry.
 *
 * @tparam T the float point number in use. default = float (32-bit)
 */
template <typename T = F32> class TriangleMesh {
public:
  using VerticeType = Eigen::Vector3<T>;
  using VerticeListType = Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor>;
  using FaceIndexType = Eigen::Vector3<Idx>;
  using IndexListType = Eigen::Matrix<
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
  TriangleMesh(const VerticeListType& vert, const IndexListType& indi);

  /**
   * @brief Construct a triangle mesh object, with no vertices and indices initialized. Will return
   * false when call `IsGood` right after.
   *
   * @warning Double Check if you really need this constructor.
   */
  __attribute__((warning("The Default Constructor may be not safe."))) TriangleMesh() = default;

  /**
   * @brief Enable Copy Constructor
   * 
   */
  TriangleMesh(const TriangleMesh&) = default;

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

  inline bool IsGood() const;

  inline const VerticeListType& GetVertices() const;

  inline const IndexListType& GetIndices() const;

private:
  Idx num_vertices_{0};

  Idx num_edges_{0};

  // Mesh should be column major, AoS layout.
  VerticeListType vertices_;

  // the same for indice.
  IndexListType indices_;

  bool is_good_{false};
};

template<typename T> 
const typename TriangleMesh<T>::VerticeListType& TriangleMesh<T>::GetVertices() const {
  return vertices_;
}


template<typename T> 
const typename TriangleMesh<T>::IndexListType& TriangleMesh<T>::GetIndices() const {
  return indices_;
}

template<typename T>
TriangleMesh<T>::TriangleMesh(const VerticeListType& vertices, const IndexListType& indices) :
  vertices_(vertices),
  indices_(indices) { }



}  // namespace acg::geometry
