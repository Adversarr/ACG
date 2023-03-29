#pragma once

#include <acore/math/common.hpp>

namespace acg::geometry {

class UniformLaplacianSmoother {
private:
  const types::topology::TriangleList& triangle_list_;

  template <typename S, int i> void Aggre(Field<S, i>& output, const Field<S, i>& input,
                                          Field<S, 1>& weight, Index self, Index lhs, Index rhs) const;

public:
  explicit UniformLaplacianSmoother(const types::topology::TriangleList& faces) : triangle_list_(faces) {}

  template <typename S, int i> Field<S, i> Compute(const Field<S, i>&) const;

  template <typename S, int i> void ComputeInplace(Field<S, i>&) const;
};

template <typename S, int i>
inline void UniformLaplacianSmoother::Aggre(Field<S, i>& output, const Field<S, i>& input,
                                            Field<S, 1>& weight, Index self, Index lhs, Index rhs) const {
  output.col(self) += input.col(lhs) + input.col(rhs);
  weight(1, self) += static_cast<S>(2);
}

template <typename S, int i>
Field<S, i> UniformLaplacianSmoother::Compute(const Field<S, i>& attr) const {
  Field<S, i> output;
  Index v_cnt = attr.cols();
  Field<S, 1> weight;
  output.setZero(i, v_cnt);
  weight.setZero(1, v_cnt);
  for (const auto& col : triangle_list_.colwise()) {
    Aggre<S, i>(output, attr, weight, col.x(), col.y(), col.z());
    Aggre<S, i>(output, attr, weight, col.y(), col.z(), col.x());
    Aggre<S, i>(output, attr, weight, col.z(), col.x(), col.y());
  }
  return (output.array().rowwise() / weight.array()).matrix();
}

template <typename S, int i>
void UniformLaplacianSmoother::ComputeInplace(Field<S, i>& attr) const {
  attr = Compute<S, i>(attr);
}

}  // namespace acg::geometry
