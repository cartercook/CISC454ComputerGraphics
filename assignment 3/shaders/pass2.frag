#version 330 core

uniform vec2 resolution;
uniform vec3 ScreenLightPos;
uniform mediump sampler2D prepassSampler;

layout(location = 0) out mediump vec3 colour;

void main()
{
  int NUM_SAMPLES = 100;
  mediump float Density = 1.0;
  mediump float Weight = 0.01;
  mediump float Decay = 1.0;
  mediump float Exposure = 1.0;

  mediump vec2 texCoord = gl_FragCoord.xy;

  // Calculate vector from pixel to light source in screen space.
  mediump vec2 deltaTexCoord = texCoord/resolution - ScreenLightPos.xy;

  // // Divide by number of samples and scale by control factor.
  // deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;

  // // Store initial sample.
  // colour = texture(prepassSampler, texCoord).xyz;

  // // Set up illumination decay factor.
  // mediump float illuminationDecay = 1.0f;

  // // Evaluate summation from Equation 3 NUM_SAMPLES iterations.
  // for (int i = 0; i < NUM_SAMPLES; i++)
  // {  
  //   // Step sample location along ray.
  //   texCoord -= deltaTexCoord;

  //   // Retrieve sample at new location.
  //   mediump vec3 sample = texture(prepassSampler, texCoord).xyz;

  //   // Apply sample attenuation scale/decay factors.
  //   sample *= illuminationDecay * Weight;

  //   // Accumulate combined colour.
  //   colour += sample;

  //   // Update exponential decay factor.
  //   illuminationDecay *= Decay;
  // } 

  // // Output final colour with a further scale control factor.
  // colour *= Exposure;
  colour = vec3(ScreenLightPos);
}