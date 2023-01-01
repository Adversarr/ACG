#version 450

layout(binding = 0) uniform UniformBufferObject {
  mat4 mvp;
  vec3 eye_position;
  vec4 ambient_light_color;
  vec3 light_color;
  vec3 light_position;
  int options[4];
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 normal;
layout(location = 0) out vec4 fragColor;

void main() {
  vec4 frag_position = ubo.mvp * vec4(inPosition, 1.0);
  gl_Position = frag_position;
  if (ubo.options[0] == 0) {
    fragColor = vec4(inColor, 1.0);
    return;
  }

  vec3 light_dir   = normalize(ubo.light_position - inPosition);
  vec3 view_dir  = normalize(ubo.eye_position - inPosition);
  vec3 halfway_dir = normalize(light_dir + view_dir);
  // Bling-Phong Model.
  vec3 ambient = ubo.ambient_light_color.w * ubo.ambient_light_color.xyz;
  vec3 diffuse;
  // Compute Diffusion Light, if normal vector is available.
  vec3 nnormal = normalize(normal);
  float diff = max(dot(nnormal, light_dir), 0.0);
  diffuse = diff * ubo.light_color;

  float spec = pow(max(dot(nnormal, halfway_dir), 0.0), 64); // use shiness = 128
  vec3 specular = ubo.light_color * spec;
  vec3 result = (ambient + diffuse + specular) * inColor;
  fragColor = vec4(result, 1.0);
}
