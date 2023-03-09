#pragma once
#include <map>

#include <iostream>
#include "../tetface.hpp"

namespace acg::geometry {

template <typename Scalar> Tet2Face<Scalar>& Tet2Face<Scalar>::operator()() noexcept {
  std::map<std::tuple<Index, Index, Index>, bool> face_data;

  auto pac = access(position_);

  auto update_face = [&](Index i, Index j, Index k, Index l) {
    // For ijk, test l.
    // 1. make sure i < j < k:
    if (i > j) {
      std::swap(i, j);
    }
    if (j > k) {
      std::swap(j, k);
    }
    if (i > j) {
      std::swap(i, j);
    }

    auto x0 = pac(i);
    auto x1 = pac(j);
    auto x2 = pac(k);
    auto x3 = pac(l);
    auto x30 = x3 - x0, x20 = x2 - x0, x10 = x1 - x0;
    auto normal = x10.cross(x20);
    bool invert = false;
    if (normal.dot(x30) >= static_cast<Scalar>(0)) {
      invert = true;
    }
    auto tup = std::make_tuple(i, j, k);
    if (auto it = face_data.find(tup); it != face_data.end()) {
      face_data.erase(it);
    } else {
      face_data.insert({tup, invert});
    }
  };
  for (auto [i, v] : enumerate(access(tetras_))) {
    Index x = v.x(), y = v.y(), z = v.z(), w = v.w();
    update_face(x, y, z, w);
    update_face(w, x, y, z);
    update_face(z, w, x, y);
    update_face(y, z, w, x);
  }

  Index id = 0;
  faces_.resize(Eigen::NoChange, face_data.size());
  face_normals_.resize(Eigen::NoChange, face_data.size());
  for (auto [tup, v] : face_data) {
    auto [i, j, k] = tup;
    auto x0 = pac(i);
    auto x1 = pac(j);
    auto x2 = pac(k);
    auto x20 = x2 - x0, x10 = x1 - x0;
    Vec3<Scalar> normal = x10.cross(x20).eval();
    normal.normalize();
    if (v) {
      faces_.col(id) = Vec3Index{i, k, j};
      face_normals_.col(id) = -normal;
    } else {
      faces_.col(id) = Vec3Index{i, j, k};
      face_normals_.col(id) = normal;
    }
    ++id;
  }
  return *this;
}
}  // namespace acg::geometry
