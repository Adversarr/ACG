#include <doctest/doctest.h>

#include <ostream>

#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#include <acg_utils/json_serialize/all.hpp>


DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("basic") {
  {
    auto ret = acg::utils::to_json(std::vector<int>{1, 2, 3});
    std::cout << ret.dbg() << std::endl;
  }

  {
    auto ret = acg::utils::to_json(std::map<std::string, int> {{"123", 2}});
    std::cout << ret.dbg() << std::endl;
  }
}


class Cls {
public:
  int x; float y; std::string c;
  ENABLE_JSON_SERIALIZE_FOR(Cls, x, y, c);
};