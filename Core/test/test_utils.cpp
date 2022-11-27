#include <co/unitest.h>

#include <acg_utils/god/algorithms.hpp>
#include <acg_utils/god/god.hpp>
#include <cassert>

using namespace acg::utils::god::details;

template <typename L, typename R> struct Right {
  using type = R;
};

template <typename L, typename R> struct Left {
  using type = L;
};
struct X {
  static constexpr int value = 1; // NOLINT
};

struct Y {
  static constexpr int value = 2; // NOLINT
};

struct Z {
  static constexpr int value = 3; // NOLINT
};
template <typename X, typename Y> struct Pred {
  static constexpr bool value = X::value < Y::value; // NOLINT
};

DEF_test(acg_utils) {
  DEF_case(TList) {
    EXPECT((std::is_same_v<List<int>::extend<float>, List<int, float>>));
    EXPECT((std::is_same_v<ConcatType<List<int>, List<float, long>>, List<int, float, long>>));
    EXPECT((std::is_same_v<Map<std::remove_pointer, List<int*, float*>>::type, List<int, float>>));
    EXPECT((std::is_same_v<Car<List<int, float, char>>::type, int>));
    EXPECT((std::is_same_v<Cdr<List<int, float, char>>::type, List<float, char>>));
    EXPECT((std::is_same_v<GetElem<0, List<int, float, char>>::type, int>));
    EXPECT((!std::is_same_v<GetElem<1, List<int, float, char>>::type, int>));
    EXPECT((std::is_same_v<GetElem<2, List<int, float, char>>::type, char>));
    EXPECT((std::is_same_v<GetElem<3, List<int, float, char>>::type, Empty>));
    EXPECT((std::is_same_v<Reduce<Right, List<int, float, char>>::type, char>));
    EXPECT((std::is_same_v<Reduce<Left, List<int, float, char>>::type, int>));
    EXPECT((All<std::is_integral, List<int, long>>::value));
    EXPECT((!All<std::is_floating_point, List<int, long>>::value));
  }
  DEF_case(algorithms) {
    EXPECT((std::is_same_v<Partition<2, List<int, float, double>>::left, List<int, float>>));
    EXPECT((std::is_same_v<Merge<Pred, List<Y>, List<X, Z>>::type, List<X, Y, Z>>));
    EXPECT((std::is_same_v<Merge<Pred, List<X, Z>, List<Y>>::type, List<X, Y, Z>>));
    EXPECT((std::is_same_v<MergeSort<Pred, List<X, Z, Y>>::type, List<X, Y, Z>>));
    EXPECT((std::is_same_v<MergeSort<Pred, List<Z, X, Y>>::type, List<X, Y, Z>>));
  }
}
