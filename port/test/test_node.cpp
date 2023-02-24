#include "aport/triangle/node_loader.hpp"
#include <doctest/doctest.h>

#include <sstream>

TEST_CASE("Ele Loader") {
  std::stringstream input(
      R"(1 3 0
         0 1.0 2.0 3.0
        )");
  acg::port::triangle::NodeLoader loader(input);

  CHECK_EQ(acg::Status::kFailedPrecondition, loader.GetStatus());
  loader.Load();

  CHECK_EQ(acg::Status::kOk, loader.GetStatus());
  auto data = loader.GetData();
  CHECK_EQ(data.cols(), 1);
  CHECK_EQ(data.rows(), 3);
  CHECK_EQ(data(0, 0), 1);
  CHECK_EQ(data(2, 0), 3);
}
