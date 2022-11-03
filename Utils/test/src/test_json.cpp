#include <acg_utils/json_serialize/all.hpp>
#include <co/unitest.h>
#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
namespace test {

DEF_test(basic) {
  DEF_case(c) {
    {
      auto ret = acg::utils::to_json(std::vector<int>{1, 2, 3});
      std::cout << ret.dbg() << std::endl;
    }

    {
      auto ret = acg::utils::to_json(std::map<std::string, int>{{"123", 2}});
      std::cout << ret.dbg() << std::endl;
    }

    std::vector<int> vec = {1, 3, 2};
    auto length = vec.size();
  }
}
}  // namespace test
