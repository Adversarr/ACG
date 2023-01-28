#pragma once

#include "../camera.hpp"
#include "../light.hpp"
#include "graphics_context.hpp"
#include "graphics_pass.hpp"

namespace acg::gui {
namespace details {

struct MeshVertex {
  glm::vec3 position_;
  glm::vec4 color_;
  glm::vec3 normal_;
  glm::vec2 uv_;

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();
  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};

struct MeshInstance {
  glm::vec3 position;
  glm::vec4 rotation;
  glm::vec4 color;
  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();
  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};

// Vulkan states that, Push constant support at least 128 bytes.
struct MeshPushConstants {
  // 64B
  glm::mat4 model;
  // 4 * 4B
  // Options[0] => use double side lighting
  // Options[1] => specular shineness
  // Options[2] => use pc color
  int options[4] = {0, 32, 0, 0};
};

// Vulkan states that, Uniform buffer support at least 16384 bytes.
struct MeshUniform {
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;
  alignas(16) glm::vec3 eye_position;
  alignas(16) glm::vec3 point_light_pos;
  alignas(16) glm::vec4 point_light_color;
  alignas(16) glm::vec3 parallel_light_dir;
  alignas(16) glm::vec4 parallel_light_color;
  alignas(16) glm::vec4 ambient_light_color;
};

class MeshPipeline {
public:
  struct Config {
    vk::CullModeFlags cull_mode_{vk::CullModeFlagBits::eNone};
    vk::FrontFace front_face_{vk::FrontFace::eCounterClockwise};
    bool enable_color_blending{false};
    bool use_push_constants{true};
  };

  void SetCamera(const Camera& cam);

  void SetLight(const Light& light);

  void Destroy();

  void CreateDescriptorSetLayout();

  void CreateDescriptorSets(const GraphicsRenderPass& pass);

  void CreateUniformBuffers();

  void CreateGraphicsPipeline(const GraphicsRenderPass& graphics_pass);

  void BeginPipeline(vk::CommandBuffer& current_command_buffer);

  void EndPipeline(vk::CommandBuffer& current_command_buffer);

  void UpdateUbo(bool fast = false);

  ~MeshPipeline();

  explicit MeshPipeline(const GraphicsRenderPass& graphics_pass, Config config);

private:
  void Init(const GraphicsRenderPass& graphics_pass);

  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline pipeline_;

  Config config_;
  std::vector<BufferWithMemory> uniform_buffers_;

  MeshUniform ubo_;

public:
  inline vk::PipelineLayout GetPipelineLayout() const { return pipeline_layout_; }
  inline MeshPipeline& SetCullMode(vk::CullModeFlags cull_mode) noexcept {
    config_.cull_mode_ = cull_mode;
    return *this;
  }

  inline MeshPipeline& SetFrontFace(vk::FrontFace front) noexcept {
    config_.front_face_ = front;
    return *this;
  }
};
}  // namespace details
}  // namespace acg::gui
