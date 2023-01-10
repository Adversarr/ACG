#version 450

// Vertex Input, see mesh_ppl.hpp
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

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

layout(push_constant) uniform constants {
  float size;
  vec3 color;
  int options[4];
} pc;

layout(location = 0) out vec3 outColor;

void main(){
  gl_Position = ubo.projection * ubo.view * vec4(inPosition, 1.0);
  gl_PointSize = pc.size;
  if (pc.options[0] == 0) {
    outColor = inColor;
  } else {
    outColor = pc.color;
  }
  return ;
}
