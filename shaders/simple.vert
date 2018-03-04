#version 150

uniform mat4x4[3] proj_mat;

in vec3 pos;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main() {
  // frag_uv = uv;
  // frag_color = color;
  // gl_Position = proj_mat * vec4(pos.xy, 0, 1);
  gl_Position = proj_mat[1] * vec4(pos.xyz, 1);
}
