#pragma once

#include "../camera.hpp"
#include "../point_light.hpp"
#include "graphics_pass.hpp"

namespace acg::gui {
namespace details {

class MeshPipeline2 {
public:
  class Builder;

  void SetCamera(const Camera& cam);

  void SetLight(const Light& light);

  void CleanUp();

  void CreateDescriptorSetLayout();
  void CreateDescriptorSets(const GraphicsRenderPass& pass);

  void CreateUniformBuffers();

  // TODO: Recreate Graphics Pipleine method should be supported.

  void CreateGraphicsPipeline(const GraphicsRenderPass& graphics_pass);

  void BeginPipeline(vk::CommandBuffer& current_command_buffer);

  void EndPipeline(vk::CommandBuffer& current_command_buffer);

  void UpdateUbo(bool fast = false);

  ~MeshPipeline2();

private:
  MeshPipeline2() = default;
  void Init(const GraphicsRenderPass& graphics_pass);
  bool is_inited_{false};

  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;

  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline pipeline_;

  vk::PolygonMode polygon_mode_{vk::PolygonMode::eFill};
  vk::CullModeFlags cull_mode_{vk::CullModeFlagBits::eNone};
  vk::FrontFace front_face_{vk::FrontFace::eCounterClockwise};

  std::vector<std::unique_ptr<VkContext::BufMem>> uniform_buffers_;

  Ubo ubo_;
};

class MeshPipeline2::Builder {
public:
  inline std::unique_ptr<MeshPipeline2> Build(const GraphicsRenderPass& r) const {
    auto retval = std::unique_ptr<MeshPipeline2>(new MeshPipeline2);
    retval->Init(r);
    return retval;
  }
};

}  // namespace details
}  // namespace acg::gui
