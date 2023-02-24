#pragma once
#include <vulkan/vulkan.hpp>
#include <autils/log.hpp>

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger);

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL debug_message_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/);
#ifndef VK_CHECK_SUCCESS
#define VK_CHECK_SUCCESS(expr) do {\
    auto ret = static_cast<vk::Result>(expr); \
    ACG_CHECK(ret == vk::Result::eSuccess, "Return Value of ["#expr"] is not VK_SUCCESS.");\
  } while (false)
#endif

