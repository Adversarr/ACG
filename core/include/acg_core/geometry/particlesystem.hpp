#pragma once
#include <fmt/format.h>

#include <Eigen/Eigen>
#include <initializer_list>

#include "../common.hpp"
#include "particle.hpp"

namespace acg::geometry {

// TODO: Refactor
template <typename T = F32> class ParticleSystem {
private:
  // Center position coordinate in world space.
  using StateType = Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor>;
  StateType centers_;
  // Radius for each particle
  Eigen::VectorX<T> radiuses_;

public:
  ParticleSystem(std::initializer_list<Particle<T>> l) {
    centers_.resize(3, l.size());
    radiuses_.resize(l.size());
    int i = 0;
    for (const auto& p : l) {
      centers_.col(i) = p.GetCenter();
      radiuses_(i) = p.GetRadius();
      i += 1;
    }
  }

  /**
   * Append one particle to the system
   */
  ParticleSystem<T>& Append(const Particle<T>& particle) {
    centers_.resize(3, centers_.cols() + 1);
    radiuses_.resize(radiuses_.cols() + 1);
    centers_.col(centers_.cols() - 1) = particle.GetCenter();
    radiuses_(radiuses_.cols() - 1) = particle.GetRadius();
  }

  /**
   * Append the particle system from given iterator
   */
  template <typename InputIt> ParticleSystem<T>& Append(InputIt first, InputIt last) {
    auto extended_size = std::distance(first, last);
    centers_.resize(3, centers_.cols() + extended_size);
    radiuses_.resize(radiuses_.cols() + extended_size);
    Idx i = centers_.cols() - extended_size;
    for (auto it = first; it != last; ++it) {
      centers_.col(i) = it->GetCenter();
      radiuses_(i) = it->GetRadius();
    }
  }

  std::string to_string() const {
    fmt::memory_buffer buf;
    for (Idx i = 0; i < centers_.cols(); ++i) {
      fmt::format_to(std::back_inserter(buf), "center=({} {} {}) radius={}\n", centers_.col(i).x(),
                     centers_.col(i).y(), centers_.col(i).z(), radiuses_[i]);
    }
    return fmt::to_string(buf);
  }

  inline StateType& GetCenter() { return centers_; }

  inline const StateType& GetCenter() const { return centers_; }

  inline auto& UpdateCenter(Idx idx, const Vec3<T>& center) {
    centers_.col(idx) = center;
    return *this;
  }

  inline auto& UpdateRadius(Idx idx, T r) {
    ACG_DEBUG_CHECK(r > 0, "Radius should be greater than zero");
    radiuses_(idx) = r;
    return *this;
  }
};

}  // namespace acg::geometry
