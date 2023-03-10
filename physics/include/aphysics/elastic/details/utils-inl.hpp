#pragma once
#include <aphysics/elastic/common.hpp>
#include <aphysics/elastic/invariants.hpp>
namespace acg::physics::elastic::details {

// NOTE: See page 71, Equation 5.50.
template <typename Scalar, int dim>
auto compose_final_hessian(Scalar ddi1, Scalar ddi2, Scalar ddi3, Scalar di1,
                                            Scalar di2, Scalar di3,
                                            const InvariantHessianResult<Scalar, dim>& r) {
  return ddi1 * r.inv_grad_result_.i1_grad_ * r.inv_grad_result_.i1_grad_.transpose()
         + ddi2 * r.inv_grad_result_.i2_grad_ * r.inv_grad_result_.i2_grad_.transpose()
         + ddi3 * r.inv_grad_result_.i3_grad_ * r.inv_grad_result_.i3_grad_.transpose()
         + di1 * r.i1_hessian_ + di2 * r.i2_hessian_ + di3 * r.i3_hessian_;
}
}  // namespace acg::physics::elastic::details
