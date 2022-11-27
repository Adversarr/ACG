#include <Eigen/Eigen>
#include <acg_core/math.hpp>

#include "spdlog/spdlog.h"

int main(int argc, char** argv) {
  Eigen::initParallel();
  Eigen::setNbThreads(8);
  spdlog::info("Eigen Parallel: #threads = {}", Eigen::nbThreads());
  int n = 10000;
  Eigen::MatrixXf mat(n, n);
  mat.setRandom();

  mat = mat * mat;
  return 0;
}
