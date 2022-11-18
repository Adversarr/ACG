#include <co/fastream.h>
#include <co/fastring.h>
#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/math.hpp>
#include <acg_core/sad/dual.hpp>
#include <acg_utils/god/algorithms.hpp>
using namespace acg::utils::god;
using namespace acg::sad::details;
Variable(float, X);
Variable(float, Y);

namespace test {

DEF_test(sad) {
  DEF_case(scalar_sad) {
    using X2 = Mul<X, X>; // X * X
    using E = Add<X2, Y>;  // X * X + Y
    using P = Add<E, Ones<float>>; // X * X + Y + 1
    using dx = Simpliest_t<Diff<P, X>::Grad>;
    using ddx = Simpliest_t<Diff<dx, X>::Grad>;
    Context<P, dx, ddx> c; // Compute Context
    c.Set<X>(10); // Setup Input X
    c.Set<Y>(20); // Setup Input Y
    acg::sad::run(c);
    std::cout << c.Get<dx>() << std::endl;
    std::cout << c.Get<ddx>() << std::endl;
  }

  DEF_case(matrix_sad) {
    struct Mx : public Input<Eigen::Matrix<float, 2, 3>> {};
    // struct Mx : public Input<Eigen::Matrix2f> {};
    struct My : public Input<Eigen::Matrix2f> {};
    using Yx = Mul<My, Mx>;
    using dx = Diff<Yx, Mx>::Grad;
    using dy = Diff<Yx, My>::Grad;
    Context<Yx, dy, dx> c;
    c.Set<My>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    // c.Set<Mx>(Eigen::Matrix2f{{1, 2}, {3, 4}});
    c.Set<Mx>(Eigen::Matrix<float, 2, 3>{{1, 2, 3}, {4, 5, 6}});
    acg::sad::run(c);
    std::cout << c.Get<dy>() << std::endl;
    // std::cout << c.Get<dx>() << std::endl;
  }
}

}  // namespace test
