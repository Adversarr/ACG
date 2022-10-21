#pragma once
#include "acg_core.hpp"
#include <Eigen/Eigen>
#include <spdlog/spdlog.h>
#include <memory>


namespace acg::geometry {

namespace details {


template <typename T = float> class TriangleMesh {
public:
  using VerticesT = Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor>;
  using IndicesT = Eigen::Matrix<int, 3, Eigen::Dynamic, Eigen::ColMajor>;

  // Mesh should be column major, AoS layout.
  VerticesT vertices_;

  // the same for indice.
  IndicesT indices_;

public:
  inline TriangleMesh(VerticesT vert, IndicesT indi): 
    vertices_(std::move(vert)),
    indices_(std::move(indi)) {
  }

  __attribute__((warning("The Default Constructor may be not safe.")))
  TriangleMesh() = default;
};

}

// always use shared_ptr for data transfering.
template<typename T = float>
using TriangleMesh = std::shared_ptr<details::TriangleMesh<T>>;

} // namespace acg::geometry
