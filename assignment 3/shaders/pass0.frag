#version 330 core

// Ouput data
layout(location = 0) out vec3 fragmentdepth;

void main(){
    // write depth to depth buffer
    fragmentdepth = vec3(gl_FragCoord.z);
}