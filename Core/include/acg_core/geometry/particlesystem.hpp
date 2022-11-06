#pragma once
#include <Eigen/Eigen>
#include "../core.hpp"
#include <initializer_list>
#include "particle.hpp"
#include <fmt/format.h>

namespace acg::geometry {


template <typename T = F32> class ParticleSystem {
  
private:
  // Center position coordinate in world space.
  Eigen::Matrix<T, 3, Eigen::Dynamic, Eigen::ColMajor> centers_;

  // Radius for each particle
  Eigen::VectorX<T> radiuses_;

public:

  ParticleSystem(std::initializer_list<Particle<T>> l) { 
    centers_.resize(3, l.size());
    radiuses_.resize(l.size());
    int i = 0;
    for (const auto& p: l) {
      centers_.col(i) = p.GetCenter(); 
      radiuses_(i)    = p.GetRadius();
      i += 1;
    }
  }

  /**
   * Append one particle to the system
   */
  ParticleSystem<T>& Append(const Particle<T>& particle);

  /**
   * Append the particle system from given iterator
   */
  template<typename InputIt>
  ParticleSystem<T>& Append(InputIt first, InputIt last);

  std::string to_string() const {
    fmt::memory_buffer buf;
    for (Idx i = 0; i < centers_.cols(); ++i) {
      fmt::format_to(std::back_inserter(buf), "center=({} {} {}) radius={}\n",
                    centers_.col(i).x(),
                    centers_.col(i).y(),
                    centers_.col(i).z(),
                    radiuses_[i]);
    }
    return fmt::to_string(buf);
  }
};



}
