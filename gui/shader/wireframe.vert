#version 450
layout(binding = 0) uniform UniformBufferObject {
  mat4 mvp;
  vec3 eye_position;
  vec4 ambient_light_color;
  // Point Lights
  vec3 light_color;
  vec3 light_position;
  int options[4];
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
  gl_Position = ubo.mvp * vec4(inPosition, 1.0);
  fragColor = vec4(inColor, 1.0);
}
