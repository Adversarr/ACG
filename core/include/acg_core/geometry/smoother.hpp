#pragma once

#include "common.hpp"

namespace acg::geometry {

class UniformLaplacianSmoother {
private:
  const topology::TriangleList& triangle_list_;

  template <typename S, int i> void Aggre(Attr<S, i>& output, const Attr<S, i>& input,
                                          Attr<S, 1>& weight, Idx self, Idx lhs, Idx rhs) const;

public:
  explicit UniformLaplacianSmoother(const topology::TriangleList& faces) : triangle_list_(faces) {}

  template <typename S, int i> Attr<S, i> Compute(const Attr<S, i>&) const;

  template <typename S, int i> void ComputeInplace(Attr<S, i>&) const;
};

template <typename S, int i>
inline void UniformLaplacianSmoother::Aggre(Attr<S, i>& output, const Attr<S, i>& input,
                                            Attr<S, 1>& weight, Idx self, Idx lhs, Idx rhs) const {
  output.col(self) += input.col(lhs) + input.col(rhs);
  weight(1, self) += static_cast<S>(2);
}

template <typename S, int i>
Attr<S, i> UniformLaplacianSmoother::Compute(const Attr<S, i>& attr) const {
  Attr<S, i> output;
  Idx v_cnt = attr.cols();
  Attr<S, 1> weight;
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
void UniformLaplacianSmoother::ComputeInplace(Attr<S, i>& attr) const {
  attr = Compute<S, i>(attr);
}

}  // namespace acg::geometry
