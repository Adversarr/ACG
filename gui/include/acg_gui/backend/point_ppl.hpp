#pragma once

#include "../camera.hpp"
#include "../light.hpp"
#include "graphics_context.hpp"
#include "graphics_pass.hpp"

namespace acg::gui {

namespace details {

struct PointVertex {
  glm::vec3 position;
  glm::vec4 color;
  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();
  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};

// Vulkan states that, Uniform buffer support at least 16384 bytes.
struct PointUniform {
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;
  alignas(16) glm::vec3 eye_position;
  alignas(16) glm::vec3 point_light_pos;
  alignas(16) glm::vec4 point_light_color;
  alignas(16) glm::vec3 parallel_light_dir;
  alignas(16) glm::vec4 parallel_light_color;
  alignas(16) glm::vec4 ambient_light_color;
};

struct PointPushConstants {
  float size;
  glm::vec4 color;
  // Option[0] => opt[0] = 1: use pc color instead of in color.
  int options[4] = {0, 0, 0, 0};
};

class PointPipeline {
public:
  struct Config {
    bool enable_color_blending = false;
  };

  void SetCamera(const Camera& cam);

  void Destroy();

  void CreateDescriptorSetLayout();

  void CreateDescriptorSets(const GraphicsRenderPass& pass);

  void CreateUniformBuffers();

  void CreateGraphicsPipeline(const GraphicsRenderPass& graphics_pass);

  void BeginPipeline(vk::CommandBuffer& current_command_buffer);

  void EndPipeline(vk::CommandBuffer& current_command_buffer);

  void UpdateUbo(bool fast = false);

  void Recreate(const GraphicsRenderPass& graphics_pass);

  ~PointPipeline();

  explicit PointPipeline(const GraphicsRenderPass& graphics_pass, Config config);

private:
  void Init(const GraphicsRenderPass& graphics_pass);

  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline pipeline_;

  Config config_;
  PointUniform ubo_;
  std::vector<BufferWithMemory> uniform_buffers_;

public:
  inline vk::PipelineLayout GetPipelineLayout() const { return pipeline_layout_; }
};

}  // namespace details

}  // namespace acg::gui
