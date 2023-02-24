#include "aport/triangle/ele_loader.hpp"
#include <doctest/doctest.h>

#include <sstream>

TEST_CASE("Ele Loader") {
  std::stringstream input(
      R"(1 4 0
         0 1 2 3 4
        )");
  acg::port::triangle::EleLoader loader(input);

  CHECK_EQ(acg::Status::kFailedPrecondition, loader.GetStatus());
  loader.Load();

  CHECK_EQ(acg::Status::kOk, loader.GetStatus());
  auto data = loader.GetData();
  CHECK_EQ(data.cols(), 1);
  CHECK_EQ(data.rows(), 4);
  CHECK_EQ(data(0, 0), 1);
  CHECK_EQ(data(2, 0), 3);
}
