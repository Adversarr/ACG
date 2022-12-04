#pragma once

#include <Eigen/Eigen>
#include "../math/common.hpp"

namespace acg::geometry {

// TODO: Deprecate
template<typename Tp=F32>
class Particle {
private:
  Vec3<Tp> center_{0.0, 0.0, 0.0};
  
  Tp radius_{0.1};

public:
  inline Particle(Vec3<Tp> center, Tp radius): center_(center), radius_(radius) {
    // do nothing.
  }
  
  Particle(const Particle<Tp>& ) = default;
  
  Particle(Particle<Tp> &&) = default;

  inline const Vec3<Tp>& GetCenter() const {
    return center_;
  }

  inline Tp GetRadius() const {
    return radius_;
  }

  inline Particle<Tp>& SetCenter(const Vec3<Tp>& value) {
    center_ = value;
    return *this;
  }

  inline Particle<Tp>& SetRadius(Tp value) {
    radius_ = value;
    return *this;
  }

  template<typename T2>
  inline Particle<T2> Cast() const {
    return Particle<T2>(center_.template cast<T2>(), static_cast<T2>(radius_));
  }
};


}
