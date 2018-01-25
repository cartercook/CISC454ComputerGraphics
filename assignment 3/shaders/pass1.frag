// Pass 1 fragment shader
//
// Outputs colour, normal, depth to three textures

#version 300 es

uniform mediump vec3 lightPos;
uniform mediump sampler2D shadowMap;

in mediump vec3 colour;
in mediump vec3 normal;
in mediump vec4 ShadowCoord;
in mediump vec3 position;

// Output to the three textures.  Location i corresponds to
// GL_COLOUR_ATTACHMENT + i in the Framebuffer Object (FBO).

layout (location = 0) out mediump vec3 fragColour;
layout (location = 1) out mediump vec3 fragNormal;
layout (location = 2) out mediump vec3 prepassColour;

// Interpolated inputs as simply copied to the three outputs.  You do
// not have to modify this shader.

void main()

{
  //leftover from A2
  //fragColour = colour;
  fragNormal = normal;
  prepassColour = vec3(0.0);      //god-ray prepass
  //fragDepth  = vec3( depth );   // depth is stored (inefficiently) in an RGB texture


  //calculate shadowmap visibility
  mediump float visibility = 1.0;
  if ( texture2D( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z) {
    visibility = 0.0;
  }


  //Phong
  mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
  mediump vec3 Ia  = vec3( 0.2, 0.0, 0.0 ); // ambient light
  mediump vec3 kd  = colour;		    // kd = surface colour from calling program
  mediump vec3 ks  = vec3( 0.4, 0.4, 0.4 ); // ks = specular coefficients
  mediump float shininess = 400.0;	    // shininess = specular exponent

  mediump vec3 N = normalize( normal );

  //calc lightDir from point light
  mediump vec3 lightDir = normalize( lightPos - position );

  mediump float NdotL = dot( N, lightDir );

  // Compute the outgoing colour

  mediump vec3 Iout = Ia;

  if (NdotL > 0.0) {

    // diffuse component

    Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ) * visibility; 

    // specular component

    mediump vec3 R = (2.0f * NdotL) * N - lightDir;
    mediump vec3 V = vec3(0,0,1);	// direction toward eye in the VCS

    mediump float RdotV = dot( R, V );

    if (RdotV > 0.0f)
      Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b ) * visibility;
  }

  fragColour = Iout;
}