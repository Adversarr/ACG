#pragma once
#include <vulkan/vulkan.hpp>
#include <acg_utils/log.hpp>

#ifndef VK_CHECK_SUCCESS
#define VK_CHECK_SUCCESS(expr) do {\
    auto ret = static_cast<vk::Result>(expr); \
    ACG_CHECK(ret == vk::Result::eSuccess, "Return Value of ["#expr"] is not VK_SUCCESS.");\
  } while (false)
#endif

