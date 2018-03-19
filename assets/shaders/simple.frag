#version 150

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 color;
uniform float dim;

in vec2 frag_uv;
in vec3 frag_color;

out vec4 out_color;

void main() {
  //out_color = vec4(frag_color, 1);
  // out_color = texture(tex0, frag_uv) * vec4(frag_color, 1) * color;
  //out_color = vec4(frag_uv, 0, 1);

  vec4 texval0 = texture(tex0, frag_uv);
  vec4 texval1 = texture(tex1, frag_uv);

  float mix_val = texval1.a * dim;

  out_color = mix(texval0.rgba, texval1.rgba, mix_val);

  // out_color = mix(texture(tex0, frag_uv), texture(tex1, frag_uv), dim);
}
