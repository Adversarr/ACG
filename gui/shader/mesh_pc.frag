#version 450

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inWorldPosiion;

layout(binding = 0) uniform Ubo {
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

layout(location = 0) out vec4 outColor;

vec3 normal;
vec3 view;

vec3 get_point_light_color() {
  // Bling-Phong Model
  // 1. diffuse
	vec3 L = normalize(ubo.point_light_pos - inWorldPosiion);
  float diffuse_density = dot(L, normal);
  if (pc.options[0] != 0) {
    diffuse_density = abs(diffuse_density);
  } else {
    diffuse_density = max(diffuse_density, 0.0);
  }
  vec3 plc = ubo.point_light_color.xyz * inColor.xyz;
  vec3 diffuse = diffuse_density * plc.xyz;

  // 2. ambient
  vec3 ambient = ubo.ambient_light_color.xyz * inColor.xyz;
  
  // 3. specular
  vec3 view = normalize(ubo.eye_position - inWorldPosiion);
  vec3 halfwayDir = normalize(L + view);
  float specular_density = dot(normal, halfwayDir);
  if (pc.options[0] != 0) {
    specular_density = abs(specular_density);
  } else {
    specular_density = max(specular_density, 0.0);
  }
  vec3 specular = pow(specular_density, float(pc.options[1])) * plc.xyz;

  return (diffuse + specular) * ubo.point_light_color.w + ambient;
}

vec3 get_parallel_light_color() {
  float density = dot(-ubo.parallel_light_dir, normal);
  if (pc.options[0] != 0) {
    density = abs(density);
  } else {
    density = max(density, 0.0);
  }

  return density * ubo.parallel_light_color.xyz * inColor.xyz * ubo.parallel_light_color.w;
}

void main() {
  normal = normalize(inNormal);
  outColor = vec4(
      // inWorldPosiion.xyz
      get_point_light_color()
      + get_parallel_light_color()
      ,
      inColor.w
  );
}
