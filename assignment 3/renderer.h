// G-buffer renderer

#ifndef RENDER_H
#define RENDER_H


#include "wavefront.h"
#include "gpuProgram.h"
#include "gbuffer.h"


class Renderer {

  enum { COLOUR_GBUFFER,
	 NORMAL_GBUFFER,
	 DEPTH_GBUFFER,
	 RAY_PREPASS_GBUFFER,
   SHADOW_MAP,
	 NUM_GBUFFERS };

  GPUProgram *pass0Prog, *pass1Prog, *pass2Prog; //, *pass3Prog;
  GBuffer    *gbuffer;

 public:

  int debug;

  Renderer( int windowWidth, int windowHeight ) {
    gbuffer = new GBuffer( windowWidth, windowHeight, NUM_GBUFFERS );
    pass0Prog = new GPUProgram( "shaders/pass0.vert", "shaders/pass0.frag" );
    pass1Prog = new GPUProgram( "shaders/pass1.vert", "shaders/pass1.frag" );
    pass2Prog = new GPUProgram( "shaders/pass2.vert", "shaders/pass2.frag" );
    // pass3Prog = new GPUProgram( "shaders/pass3.vert", "shaders/pass3.frag" );
    debug = 0;
  }

  ~Renderer() {
    delete gbuffer;
    // delete pass3Prog;
    delete pass2Prog;
    delete pass1Prog;
    delete pass0Prog;
  }

  void reshape( int windowWidth, int windowHeight ) {
    delete gbuffer;
    gbuffer = new GBuffer( windowWidth, windowHeight, NUM_GBUFFERS );
  }

  void render( seq<wfModel*> objs, mat4 &sceneM, mat4 &cameraV, mat4 &cameraP, mat4 &lightV, mat4 &lightP, vec3 &lightPos );

  void renderShadowMap( seq<wfModel*> objs, mat4 &sceneM, mat4 &lightV, mat4 &lightP );

  void incDebug() {
    debug = (debug+1) % 3;
  }

  void makeStatusMessage( char *buffer ) {
    if (debug == 0)
      sprintf( buffer, "Program output" );
    else
      sprintf( buffer, "After pass %d", debug );
  }
};

#endif
