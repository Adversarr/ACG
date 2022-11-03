#include <ostream>
#include <acg_utils/log.hpp>
#include <acg_utils/serializer/all.hpp>


#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// TEST_CASE("Serialize Numerics") {
//   CHECK(acg::utils::serialize(10) == "10");
//   CHECK(acg::utils::deserialize<int>(std::string("10")) == 10);
// }
// TEST_CASE("Serialize Strings.") {
//   CHECK(acg::utils::serialize("") == "\"\"");
//   std::string s = "123 \"";
//   CHECK(acg::utils::deserialize<std::string>(acg::utils::serialize(s)) == s);
// }

// TEST_CASE("Serialize Vector") {
//   CHECK(acg::utils::serialize(std::vector<int>{10}) == "[10]");
//   CHECK(acg::utils::serialize(std::vector<int>{10, 20}) == "[10, 20]");
//   CHECK(acg::utils::serialize(std::vector<int>{}) == "[]");

// }

// TEST_CASE("Serialize array") {
//   CHECK(acg::utils::serialize(std::array<int, 1>{10}) == "[10]");
//   CHECK(acg::utils::serialize(std::array<int, 2>{10, 20}) == "[10, 20]");
//   CHECK(acg::utils::serialize(std::array<int, 0>{}) == "[]");
// }

// TEST_CASE("Serialize Deque") {
//   CHECK(acg::utils::serialize(std::deque<int>{10}) == "[10]");
//   CHECK(acg::utils::serialize(std::deque<int>{10, 20}) == "[10, 20]");
//   CHECK(acg::utils::serialize(std::deque<int>{}) == "[]");
// }

// TEST_CASE("Serialize Map") {
//   CHECK(acg::utils::serialize(std::map<int, int>{{10, 10}}) == "{10: 10}");
//   CHECK(acg::utils::serialize(std::map<int, int>{{10, 10}, {20, 20}}) ==
//         "{10: 10, 20: 20}");
//   CHECK(acg::utils::serialize(std::map<int, int>{}) == "{}");
// }

// TEST_CASE("Serialize Deque") {
//   CHECK(acg::utils::serialize(std::set<int>{10}) == "[10]");
//   CHECK(acg::utils::serialize(std::set<int>{10, 20}) == "[10, 20]");
//   CHECK(acg::utils::serialize(std::set<int>{}) == "[]");
// }

// TEST_CASE("Serialize Class") {
//   Complicated c;
//   auto d = acg::utils::deserialize<Complicated>(acg::utils::serialize(c));
//   CHECK(d.has_value());
//   if (!d.has_value()) {
//     return;
//   }
//   std::cout << acg::utils::serialize(*d) << std::endl;
//   CHECK(acg::utils::serialize(*d) == acg::utils::serialize(c));
// }

// TEST_CASE("Deserialize") {
//   auto x = acg::utils::deserialize<std::vector<int>>("[1,2,3]");
//   CHECK(x.has_value());
//   if (x.has_value()) {
//     std::cout << acg::utils::serialize(*x) << std::endl;
//     auto y = acg::utils::deserialize<std::map<std::string, int>>(
//         R"({"1":1,"another":2})");
//     std::cout << acg::utils::serialize(*y) << std::endl;
//   }
// }
