#version 150

uniform sampler2D tex;

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

void main() {
  //out_color = frag_color * texture(tex, frag_uv.st);
  out_color = vec4(1, 0.5, 0.2, 1);
}
