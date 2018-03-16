#version 150

uniform sampler2D tex;
uniform vec4 color;

in vec2 frag_uv;
in vec3 frag_color;

out vec4 out_color;

void main() {
  //out_color = vec4(frag_color, 1);
  out_color = texture(tex, frag_uv);
  //out_color = vec4(frag_uv, 0, 1);
}
