#version 450

// Vertex Input, see mesh_ppl.hpp
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inUV;

layout(binding = 0) uniform Ubo{
  mat4 view;
  mat4 projection;
  vec3 eye_position;
  vec3 point_light_pos;
  vec4 point_light_color;
  vec3 parallel_light_dir;
  vec4 parallel_light_color;
  vec4 ambient_light_color;
} ubo;

// push constants block
layout( push_constant ) uniform constants
{
	mat4 model;
} pc;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec2 outUV;
layout (location = 3) out vec3 outWorldPosiion;

void main() {
  vec4 world_pos = vec4(inPosition.xyz, 1.0);
  gl_Position = ubo.projection * ubo.view * pc.model * world_pos;
  vec4 normal_rotated = pc.model * vec4(inNormal.xyz, 1.0);
  outNormal = inNormal.xyz;
  outColor = inColor;
  outUV = inUV;
  return;
}
