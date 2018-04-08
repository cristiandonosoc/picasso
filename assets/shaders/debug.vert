#version 150

in vec3 SV_POSITION;
in vec3 SV_COLOR;

out vec3 frag_color;

uniform mat4 M_MODEL;
uniform mat4 M_VIEW;
uniform mat4 M_PROJ;

mat4 CalculateMVP() {
  return M_PROJ * M_VIEW * M_MODEL;
}

void main() {
  mat4 mvp = CalculateMVP();
  gl_Position = mvp * vec4(SV_POSITION, 1); 
  frag_color = SV_COLOR;
}
