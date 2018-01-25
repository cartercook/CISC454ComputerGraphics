#version 330 core

uniform mediump mat4 lightMVP;

layout(location = 0) in vec3 vertPosition;

void main(){
  // calc vertex position in CCS (always required)
  gl_Position = lightMVP * vec4( vertPosition, 1.0 );
}
