#include <doctest/doctest.h>

#include <autils/god/algorithms.hpp>
#include <autils/god/god.hpp>
#include <autils/result.hpp>
#include <cassert>
#include <iostream>
#include <vector>

#include <autils/record/roting_record.hpp>
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

acg::Result<int> check_is_zero(int x) {
  if (x == 0) {
    return acg::make_result<int>(1);
  } else {
    return acg::make_error(acg::Status::kInvalidArgument);
  }
}

struct NoCopy{
  NoCopy() = default;
  NoCopy(const NoCopy &) = delete;
  NoCopy(NoCopy&& ) = default;
};

TEST_CASE("Result") { 
  auto has_result = check_is_zero(0);
  CHECK(has_result.HasValue());
  CHECK(static_cast<bool>(has_result));
  CHECK(has_result.Value() == 1);

  auto nohas_result = check_is_zero(1);
  CHECK(!nohas_result.HasValue());
  CHECK(!static_cast<bool>(nohas_result));
  CHECK(nohas_result.Error() == acg::Status::kInvalidArgument);

  acg::Result<void> err = acg::make_error(acg::Status::kInvalidArgument);
  acg::Result<int> err_cvt = err;
  CHECK(! err.HasValue());
  CHECK(! err_cvt.HasValue());
  CHECK(err_cvt.Error() == acg::Status::kInvalidArgument);

  auto nocopy_result = acg::make_result<NoCopy>();
}


TEST_CASE("RotingRecord") {
  acg::utils::RotingRecord<float, 10> record10(10);
  record10.Sum();
}
