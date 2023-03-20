#pragma once
#include <acore/math/access.hpp>
namespace acg::geometry {

template <typename Scalar> class Tet2Face {
public:
  Tet2Face(const Field<Scalar, 3> &position, const Field<Index, 4> &tetras)
      : position_(position), tetras_(tetras) {}

  Tet2Face<Scalar> &operator()() noexcept;

  Field<Index, 3> faces_;
  Field<Scalar, 3> face_normals_;

private:
  const Field<Scalar, 3> &position_;
  const Field<Index, 4> &tetras_;
};

} // namespace acg::geometry
#include "details/tetface-inl.hpp" // IWYU pragma: export
