#include <doctest/doctest.h>

#include <acg_utils/emum_iter.hpp>
#include <acg_utils/god/algorithms.hpp>
#include <acg_utils/god/god.hpp>
#include <cassert>
#include <iostream>
#include <vector>

#include "Eigen/Eigen"

using namespace acg::utils::god::details;

template <typename L, typename R> struct Right {
  using type = R;
};

template <typename L, typename R> struct Left {
  using type = L;
};
struct X {
  static constexpr int value = 1;  // NOLINT
};

struct Y {
  static constexpr int value = 2;  // NOLINT
};

struct Z {
  static constexpr int value = 3;  // NOLINT
};
template <typename X, typename Y> struct Pred {
  static constexpr bool value = X::value < Y::value;  // NOLINT
};

TEST_CASE("god") {
  SUBCASE("TList") {
    CHECK((std::is_same_v<List<int>::extend<float>, List<int, float>>));
    CHECK((std::is_same_v<ConcatType<List<int>, List<float, long>>, List<int, float, long>>));
    CHECK((std::is_same_v<Map<std::remove_pointer, List<int*, float*>>::type, List<int, float>>));
    CHECK((std::is_same_v<Car<List<int, float, char>>::type, int>));
    CHECK((std::is_same_v<Cdr<List<int, float, char>>::type, List<float, char>>));
    CHECK((std::is_same_v<GetElem<0, List<int, float, char>>::type, int>));
    CHECK((!std::is_same_v<GetElem<1, List<int, float, char>>::type, int>));
    CHECK((std::is_same_v<GetElem<2, List<int, float, char>>::type, char>));
    CHECK((std::is_same_v<GetElem<3, List<int, float, char>>::type, Empty>));
    CHECK((std::is_same_v<Reduce<Right, List<int, float, char>>::type, char>));
    CHECK((std::is_same_v<Reduce<Left, List<int, float, char>>::type, int>));
    CHECK((All<std::is_integral, List<int, long>>::value));
    CHECK((!All<std::is_floating_point, List<int, long>>::value));
  }
  SUBCASE("algorithms") {
    CHECK((std::is_same_v<Partition<2, List<int, float, double>>::left, List<int, float>>));
    CHECK((std::is_same_v<Merge<Pred, List<Y>, List<X, Z>>::type, List<X, Y, Z>>));
    CHECK((std::is_same_v<Merge<Pred, List<X, Z>, List<Y>>::type, List<X, Y, Z>>));
    CHECK((std::is_same_v<MergeSort<Pred, List<X, Z, Y>>::type, List<X, Y, Z>>));
    CHECK((std::is_same_v<MergeSort<Pred, List<Z, X, Y>>::type, List<X, Y, Z>>));
  }
}

TEST_CASE("enum_iter") {
  SUBCASE("vector") {
    std::vector<int> a = {1, 2, 3};
    for (const auto& v : Enumerate(a)) {
      std::cout << v.first << ": " << v.second << std::endl;
    }
  }
  SUBCASE("eigen") {
    Eigen::Matrix3f a;
    a.reshaped().setLinSpaced(0, 1);
    std::cout << a << std::endl;
    auto b = a.colwise();
    auto ret = Enumerate(b);
    std::cout << std::distance(a.colwise().begin(), a.colwise().end()) << std::endl;
    for (auto it = begin(ret); it != end(ret); ++it) {
      auto val = *(it.mItr);
      std::cout << val << std::endl;
    }
  }
}
