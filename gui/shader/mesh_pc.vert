#version 450

// Vertex Input, see mesh_ppl.hpp
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

layout (location = 4) in vec3 instancePosition;
layout (location = 5) in vec4 instanceRotation;
layout (location = 6) in vec4 instanceColor;


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
  int options[4];
} pc;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec4 outColor;
layout (location = 2) out vec2 outUV;
layout (location = 3) out vec3 outWorldPosiion;

mat4 instance_matrix() {
  float c = cos(instanceRotation.w);
  float s = sin(instanceRotation.w);
  float x = instanceRotation.x;
  float y = instanceRotation.y;
  float z = instanceRotation.z;
  return mat4(
      x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
      y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0,
      x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0,
      0, 0, 0, 1);
}

void main() {
  mat4 pre = instance_matrix() * pc.model;
  vec4 world_pos = pre * vec4(inPosition.xyz, 1.0) + vec4(instancePosition.xyz, 0.0);
  gl_Position =  (ubo.projection * ubo.view * world_pos);
  vec4 normal_rotated = pre * vec4(inNormal.xyz, 0.0);
  outNormal = normal_rotated.xyz;
  if (pc.options[2] != 0) {
    outColor = instanceColor;
  } else {
    outColor = inColor;
  }
  outUV = inUV;
  outWorldPosiion = world_pos.xyz;
  return;
}
