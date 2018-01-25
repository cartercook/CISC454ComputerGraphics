// Pass 1 vertex shader
//
// Stores colour, normal, depth

#version 300 es

uniform mediump mat4 M;
uniform mediump mat4 MV;
uniform mediump mat4 MVP;
uniform mediump mat4 DepthBiasMVP;

layout (location = 0) in mediump vec3 vertPosition;
layout (location = 1) in mediump vec3 vertNormal;
layout (location = 2) in mediump vec3 vertTexCoord;

// Your shader should compute the colour, normal (in the VCS), and
// depth (in the range [0,1] with 0=near and 1=far) and store these
// values in the corresponding variables.

out mediump vec3 colour;
out mediump vec3 normal;
out mediump vec4 ShadowCoord;
out mediump vec3 position;

void main()

{
  // calc vertex position in CCS (always required)

  gl_Position = MVP * vec4( vertPosition, 1.0 );

  // Provide a colour 

  colour = vec3(1,0,0);         // YOUR CODE HERE

  // calculate normal in VCS

  normal = (MV * vec4( vertNormal, 0.0)).xyz;       // YOUR CODE HERE

  // Calculate the depth in [0,1]

  //depth = (gl_Position.z/gl_Position.w+1f)/2f;                // YOUR CODE HERE

  //translate this vertex into the point light's viewing space

  ShadowCoord = DepthBiasMVP * vec4( vertPosition, 1.0 );

  position = vertPosition;
}
