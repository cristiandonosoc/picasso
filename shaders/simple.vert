#version 150

in vec3 pos;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main() {
  gl_Position = vec4(pos.xy, 0, 1);
}
