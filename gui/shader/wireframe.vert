#version 450
layout(binding = 0) uniform UniformBufferObject {
  mat4 mvp;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
  gl_Position = ubo.mvp * vec4(inPosition, 1.0);
  fragColor = vec4(inColor, 1.0);
}
