#version 150

in vec3 SV_POSITION;  
in vec3 SV_COLOR;      
in vec2 SV_UV;         

out vec2 frag_uv;
out vec3 frag_color;

uniform float offset;

void main() {
  gl_Position = vec4(SV_POSITION.xy, 0, 1) + vec4(offset, 0, 0, 0);
  frag_color = SV_COLOR;
  frag_uv = SV_UV;
}
