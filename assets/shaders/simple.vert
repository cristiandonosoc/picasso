#version 150

in vec3 SV_POSITION;  
in vec3 SV_COLOR;      
in vec2 SV_UV;         

out vec2 frag_uv;
out vec3 frag_color;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(SV_POSITION, 1); 
  frag_color = SV_COLOR;
  frag_uv = SV_UV;
}
