#version 450

layout(location = 0) in vec3 inColor;


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



layout(location = 0) out vec4 outColor;
const float PI = 3.14159265359;
void main() {
  vec2 off = gl_PointCoord - vec2(0.5, 0.5);
  float density = clamp(length(off), 0, 0.5) * PI;
  outColor = vec4(inColor * cos(density), 1.0);
}
