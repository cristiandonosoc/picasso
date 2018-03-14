#version 150

in vec3 pos;        // ATTR:VERTEX
in vec3 color;      // ATTR:COLOR
in vec2 uv;         // ATTR:UV

out vec2 frag_uv;
out vec3 frag_color;

void main() {
  gl_Position = vec4(pos.xy, 0, 1);
  frag_color = color;
}
