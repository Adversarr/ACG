#include <co/fastream.h>
#include <co/fastring.h>
#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/math.hpp>
#include <acg_core/sad/dual.hpp>
#include <acg_core/sad/la.hpp>
#include <acg_utils/god/algorithms.hpp>
using namespace acg::utils::god;
using namespace acg::sad::details;
namespace test {

DEF_test(sad) {
  DEF_case(scalar_sad) {
    Variable(float, X);
    Variable(float, Y);

    using namespace acg::sad::details;
    using xx= Mul<X, Add<X, X>>;
    using Dx = DirectionalDiff_t<xx, X, Ones<float>>;
    using X2 = Mul<X, X>;           // X * X
    using E = Add<X2, Y>;           // X * X + Y
    using P = Add<E, Ones<float>>;  // X * X + Y + 1
    using dx = Simpliest_t<DirectionalDiff_t<P, X, OnesLike<X>>>;
    using ddx = Simpliest_t<DirectionalDiff_t<dx, X, OnesLike<X>>>;
    Context<P, dx, ddx> c;  // Compute Context
    c.Set<X>(10);           // Setup Input X
    c.Set<Y>(20);           // Setup Input Y
    acg::sad::run(c);
    std::cout << c.Get<dx>() << std::endl;
    std::cout << c.Get<ddx>() << std::endl;
  }

  DEF_case(matrix_sad) {
    struct Mx : public Input<Eigen::Matrix<float, 2, 3>> {};
    // struct Mx : public Input<Eigen::Matrix2f> {};
    struct My : public Input<Eigen::Matrix2f> {};
    using Yx = Mul<My, Mx>;
    using dx = DirectionalDiff_t<Yx, Mx, OnesLike<Mx>>;
    using dy = DirectionalDiff_t<Yx, My, OnesLike<My>>;
    Eigen::Matrix2f x;
    Context<Yx, dy, dx> c;
    c.Set<My>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    // c.Set<Mx>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    using Prod = decltype(x * x);
    c.Set<Mx>(Eigen::Matrix<float, 2, 3>{{1, 2, 3}, {4, 5, 6}});
    acg::sad::run(c);
    std::cout << c.Get<dy>() << std::endl;
    // std::cout << c.Get<dx>() << std::endl;
  }

  DEF_case(vector_sad) {
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
    Context<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2> context;
    context.Set<X>(acg::Vec3f{1, 2, 3});
    context.Set<Y>(acg::Vec3f{3, 2, 1});
    acg::sad::run(context);
    std::cout << context.Get<Dy0>() << std::endl;
    std::cout << context.Get<Dy1>() << std::endl;
    std::cout << context.Get<Dy2>() << std::endl;
    Ones<acg::Vec3f>{}();
    Zeros<acg::Vec3f>{}();
  }

  DEF_case(transpose) { std::cout << Dirac<Eigen::Matrix3f, 1, 2>{}() << std::endl; }
}

}  // namespace test
