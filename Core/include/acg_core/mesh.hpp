#pragma once
#include "acg_core.hpp"
#include <Eigen/Eigen>

namespace acg::geometry {

template <typename T = float> class TriangleMesh {
public:
  using FVec3 = Eigen::Vector<T, 3>;
  using IVec3 = Eigen::Vector<int, 3>;
  using VerticesT = Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor>;
  using IndicesT = Eigen::Matrix<int, 3, Eigen::Dynamic, Eigen::ColMajor>;

  // Mesh should be column major, AoS layout.
  VerticesT vertices_;


  IndicesT indices_;

public:
  inline TriangleMesh(VerticesT vert, IndicesT indi): 
    vertices_(std::move(vert)),
    indices_(std::move(indi)) {
    
  }

  __attribute__((warning("The Default Constructor may be not safe.")))
  TriangleMesh() = default;
};

} // namespace acg::geometry