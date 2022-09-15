#include <Utilities.h>
#include <doctest/doctest.h>

#include <ostream>

#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#include <Serializer.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

class vec3 {
public:
  float x_;
  int y_;

public:
  explicit vec3(const float &x, const int &y) noexcept : x_(x), y_(y) {}

  vec3() = default;

public:
  ENABLE_SERIALIZE_FOR(vec3, x_, y_);
};

class Complicated {
public:
  std::map<int, float> e1_ = {{1, 2.4}, {2, 24.4}};
  std::string e2_{"213"};
  int e3_{12};
  ENABLE_SERIALIZE_FOR(Complicated, e1_, e2_, e3_);
};

TEST_CASE("Serialize Numerics") {
  CHECK(AUtilities::serialize(10) == "10");
  CHECK(AUtilities::deserialize<int>(std::string("10")) == 10);
}
TEST_CASE("Serialize Strings.") {
  CHECK(AUtilities::serialize("") == "\"\"");
  std::string s = "123 \"";
  CHECK(AUtilities::deserialize<std::string>(AUtilities::serialize(s)) == s);
}

TEST_CASE("Serialize Vector") {
  CHECK(AUtilities::serialize(std::vector<int>{10}) == "[10]");
  CHECK(AUtilities::serialize(std::vector<int>{10, 20}) == "[10, 20]");
  CHECK(AUtilities::serialize(std::vector<int>{}) == "[]");
}

TEST_CASE("Serialize array") {
  CHECK(AUtilities::serialize(std::array<int, 1>{10}) == "[10]");
  CHECK(AUtilities::serialize(std::array<int, 2>{10, 20}) == "[10, 20]");
  CHECK(AUtilities::serialize(std::array<int, 0>{}) == "[]");
}

TEST_CASE("Serialize Deque") {
  CHECK(AUtilities::serialize(std::deque<int>{10}) == "[10]");
  CHECK(AUtilities::serialize(std::deque<int>{10, 20}) == "[10, 20]");
  CHECK(AUtilities::serialize(std::deque<int>{}) == "[]");
}

TEST_CASE("Serialize Map") {
  CHECK(AUtilities::serialize(std::map<int, int>{{10, 10}}) == "{10: 10}");
  CHECK(AUtilities::serialize(std::map<int, int>{{10, 10}, {20, 20}}) ==
        "{10: 10, 20: 20}");
  CHECK(AUtilities::serialize(std::map<int, int>{}) == "{}");
}

TEST_CASE("Serialize Deque") {
  CHECK(AUtilities::serialize(std::set<int>{10}) == "{10}");
  CHECK(AUtilities::serialize(std::set<int>{10, 20}) == "{10, 20}");
  CHECK(AUtilities::serialize(std::set<int>{}) == "{}");
}

TEST_CASE("Serialize Class") {
  Complicated c;
  Complicated d{AUtilities::deserialize<Complicated>(AUtilities::serialize(c))};
  std::cout << AUtilities::serialize(d) << std::endl;
  CHECK(AUtilities::serialize(d) == AUtilities::serialize(c));
}

TEST_CASE("Deserialize") {
  auto x = AUtilities::deserialize<std::vector<int>>("[1,2,3]");
  std::cout << AUtilities::serialize(x) << std::endl;
  auto y = AUtilities::deserialize<std::map<std::string, int>>(
      R"({"1":1,"another":2})");
  std::cout << AUtilities::serialize(y) << std::endl;
}
