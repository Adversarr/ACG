#pragma once
#include "def.hpp"
#include "log.hpp"

namespace acg::utils {

template <typename T> class Singleton {
  static std::unique_ptr<T> data_;


public:
  inline Singleton() {
    ACG_CHECK(static_cast<bool>(data_), "Access Singleton without initialize");
  }


  template <typename... Args> static void Init(Args&&... args) {
    ACG_CHECK(!static_cast<bool>(data_), "Double Initialize Singleton Error");
    data_ = new T(std::forward<Args>(args)...);
  }

  static void Init(std::unique_ptr<T>&& uptr) {
    ACG_CHECK(!static_cast<bool>(data_), "Double Initialize Singleton Error");
    data_.swap(uptr);
  }

  static void Release() {
    // If double release is ok
    data_.release();
  }

  T& GetRef() { return *data_.get(); }

  T* GetPtr() { return data_.get(); }

  const T& GetCRef() const { return *data_.get(); }

  const T* GetCPtr() const { return data_.get(); }
};

template <typename T> std::unique_ptr<T> Singleton<T>::data_{nullptr};

}  // namespace acg::utils
