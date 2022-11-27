#include <co/fastream.h>
#include <co/fastring.h>
#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/math.hpp>
#include <acg_core/sad/all.hpp>
#include <acg_utils/god/algorithms.hpp>
using namespace acg::utils::god;
using namespace acg::sad;

template <typename T> decltype(auto) identity(T&& value) { return std::forward<T>(value); }

namespace test {

DEF_test(sad_lazy) {
  DEF_case(scalar) {
    Variable(float, X);
    Variable(float, Y);
    using X2 = Mul<X, X>;           // X * X
    using E = Add<X2, Y>;           // X * X + Y
    using P = Add<E, Ones<float>>;  // X * X + Y + 1
    using dx = Simpliest_t<DirectionalDiff_t<P, X, OnesLike<X>>>;
    using ddx = Simpliest_t<DirectionalDiff_t<dx, X, OnesLike<X>>>;
    LazyContext<List<P, dx, ddx>> c;  // Compute Context
    c.Set<X>(10);                     // Setup Input X
    c.Set<Y>(20);                     // Setup Input Y
    decltype(c)::lazy_layers ll;
    auto r = LazyResult(c);
    EXPECT_EQ(r.Get<X>(), 10);
    EXPECT_EQ(r.Get<Y>(), 20);
    EXPECT_EQ(r.Get<P>(), 121);
    EXPECT_EQ(r.Get<dx>(), 20);
    EXPECT_EQ(r.Get<ddx>(), 2);
  }

  DEF_case(matrix) {
    Variable(Eigen::Matrix2f, Mx);
    Variable(Eigen::Matrix2f, My);
    using Yx = Mul<My, Mx>;
    using Yxax = Add<Yx, Mx>;
    using dx = DirectionalDiff_t<Yx, Mx, OnesLike<Mx>>;
    using dy = DirectionalDiff_t<Yx, My, OnesLike<My>>;
    Eigen::Matrix2f x;
    LazyContext<List<Yxax, dx, dy>> c;
    c.Set<My>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    c.Set<Mx>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    for (int i = 0; i < 1000; ++i) {
      auto r = LazyResult(c);
      r.Get<dy>().eval();
    }
  }

  DEF_case(vector) {
    Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
    Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
    Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
    Variable(acg::Vec3f, X);
    Variable(acg::Vec3f, Y);
    using FinalExp = Dot<Sub<X, Y>, Sub<X, Y>>;
    using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
    using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
    using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
    using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
    using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
    using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
    LazyContext<List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
    decltype(context)::exprs exs;
    context.Set<X>(acg::Vec3f{1, 2, 3});
    context.Set<Y>(acg::Vec3f{3, 2, 1});
    auto result = LazyResult(context);
    std::cout << result.Get<Dy0>() << std::endl;
    std::cout << result.Get<Dy1>() << std::endl;
    std::cout << result.Get<Dy2>() << std::endl;
  }
}

DEF_test(sad_nonlazy) {
  DEF_case(scalar) {
    Variable(float, X);
    Variable(float, Y);
    using X2 = Mul<X, X>;           // X * X
    using E = Add<X2, Y>;           // X * X + Y
    using P = Add<E, Ones<float>>;  // X * X + Y + 1
    using dx = Simpliest_t<DirectionalDiff_t<P, X, OnesLike<X>>>;
    using ddx = Simpliest_t<DirectionalDiff_t<dx, X, OnesLike<X>>>;
    Context<List<P, dx, ddx>> c;  // Compute Context
    c.Set<X>(10);                 // Setup Input X
    c.Set<Y>(20);                 // Setup Input Y
    acg::sad::run(c);
    EXPECT_EQ(c.Get<X>(), 10);
    EXPECT_EQ(c.Get<Y>(), 20);
    EXPECT_EQ(c.Get<P>(), 121);
    EXPECT_EQ(c.Get<dx>(), 20);
    EXPECT_EQ(c.Get<ddx>(), 2);
  }

  DEF_case(matrix) {
    Variable(Eigen::Matrix2f, Mx);
    Variable(Eigen::Matrix2f, My);
    using Yx = Mul<My, Mx>;
    using Yxax = Add<Yx, Mx>;
    using dx = DirectionalDiff_t<Yx, Mx, OnesLike<Mx>>;
    using dy = DirectionalDiff_t<Yx, My, OnesLike<My>>;
    Eigen::Matrix2f x;
    Context<List<Yxax, dx, dy>> c;
    c.Set<My>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    c.Set<Mx>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    acg::sad::run(c);
    std::cout << c.Get<dy>() << std::endl;
  }

  DEF_case(vector) {
    Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
    Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
    Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
    Variable(acg::Vec3f, X);
    Variable(acg::Vec3f, Y);
    using FinalExp = Dot<Sub<X, Y>, Add<X, Y>>;
    using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
    using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
    using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
    using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
    using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
    using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
    Context<List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
    decltype(context)::data_type c;
    context.Set<X>(acg::Vec3f{1, 1, 1});
    context.Set<Y>(acg::Vec3f{1, 1, 1});
    acg::sad::run(context);
    std::cout << context.Get<Dy0>() << std::endl;
    std::cout << context.Get<Dy1>() << std::endl;
    std::cout << context.Get<Dy2>() << std::endl;
  }
}

DEF_test(sad_general) {
  DEF_case(matrix_sad) {
    Variable(Eigen::Matrix2f, Mx);
    Variable(Eigen::Matrix2f, My);
    using Yx = Mul<My, Mx>;
    using dx = DirectionalDiff_t<Yx, Mx, OnesLike<Mx>>;
    using dy = DirectionalDiff_t<Yx, My, OnesLike<My>>;
    Eigen::Matrix2f x;
    Context<List<Yx, dy, dx>> c;
    c.Set<My>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    c.Set<Mx>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    acg::sad::run(c);
    std::cout << c.Get<dy>() << std::endl;
  }
  DEF_case(vector_sad) {
    Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
    Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
    Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
    Variable(acg::Vec3f, X);
    Variable(acg::Vec3f, Y);
    using FinalExp = Dot<Sub<X, Y>, Add<X, Y>>;
    using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
    using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
    using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
    using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
    using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
    using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
    LazyContext<List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
    context.Set<X>(acg::Vec3f{1, 2, 3});
    context.Set<Y>(acg::Vec3f{1, 2, 3});
    // acg::sad::run(context);
    auto result = LazyResult(context);
    EXPECT_EQ(result.Get<Dy0>(), -2);
    EXPECT_EQ(result.Get<Dy1>(), -4);
    EXPECT_EQ(result.Get<Dy2>(), -6);
    EXPECT_EQ(result.Get<Dx0>(), 2);
    EXPECT_EQ(result.Get<Dx1>(), 4);
    EXPECT_EQ(result.Get<Dx2>(), 6);
  }
  DEF_case(transpose) { std::cout << Dirac<Eigen::Matrix3f, 1, 2>{}() << std::endl; }
}

}  // namespace test
