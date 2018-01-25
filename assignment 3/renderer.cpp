// G-buffer renderer


#include "headers.h"
#include "renderer.h"
#include "shader.h"


// Draw a quad over the full screen.  This generates a fragment for
// each pixel on the screen, allowing the fragment shader to run on
// each fragment.


void drawFullscreenQuad()

{
  vec2 verts[4] = { vec2( -1, -1 ), vec2( -1, 1 ), vec2( 1, -1 ), vec2( 1, 1 ) };
    
  GLuint VAO, VBO;

  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );

  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  glDeleteBuffers( 1, &VBO );
  glDeleteVertexArrays( 1, &VAO );
}


// Render the scene in three passes.


void Renderer::render( seq<wfModel*> objs, mat4 &sceneM, mat4 &cameraV, mat4 &cameraP, mat4 &lightV, mat4 &lightP, vec3 &lightPos )

{
  // Pass 1: Store colour, normal, depth in G-Buffers

  gbuffer->BindForWriting();

  pass1Prog->activate();

  gbuffer->BindTexture( COLOUR_GBUFFER );
  gbuffer->BindTexture( NORMAL_GBUFFER );

  int activeDrawBuffers1[] = { COLOUR_GBUFFER, NORMAL_GBUFFER, RAY_PREPASS_GBUFFER };
  gbuffer->setDrawBuffers( 3, activeDrawBuffers1 );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );

  pass1Prog->setInt(   "shadowMap",    SHADOW_MAP   );

  pass1Prog->setVec3(  "lightPos",     lightPos     );

  for(int i = 0; i < objs.size(); i++) {
    mat4 M = objs[i]->Matrix * sceneM;
    mat4 MV = cameraV * M;
    mat4 MVP = cameraP * MV;

    mat4 depthMVP = lightP * lightV * M;

    mat4 biasMatrix = mat4();
    biasMatrix.rows[0] = vec4(0.5, 0.0, 0.0, 0.0);
    biasMatrix.rows[1] = vec4(0.0, 0.5, 0.0, 0.0);
    biasMatrix.rows[2] = vec4(0.0, 0.0, 0.5, 0.0);
    biasMatrix.rows[3] = vec4(0.5, 0.5, 0.5, 1.0);

    mat4 depthBiasMVP = biasMatrix*depthMVP;

    pass1Prog->setMat4(  "M",            M            );
    pass1Prog->setMat4(  "MV",           MV           );
    pass1Prog->setMat4(  "MVP",          MVP          );
    pass1Prog->setMat4(  "DepthBiasMVP", depthBiasMVP );

    objs[i]->draw( pass1Prog );
  }

  pass1Prog->deactivate();

  if (debug == 1) {
    gbuffer->DrawGBuffers();
    return;
  }


  // Pass 2: render rays onto prepass buffer

  pass2Prog->activate();

  vec4 screenLightPos = (cameraP * cameraV) * vec4(lightPos.x, lightPos.y, lightPos.z, 1.0);
  pass2Prog->setVec2( "ScreenLightPos", vec2(screenLightPos.x, screenLightPos.y));

  pass2Prog->setVec2( "resolution", vec2(windowWidth, windowHeight) );

  pass2Prog->setInt( "prepassSampler", RAY_PREPASS_GBUFFER );



  gbuffer->BindTexture( COLOUR_GBUFFER );

  int activeDrawBuffers2[] = { COLOUR_GBUFFER };
  gbuffer->setDrawBuffers( 1, activeDrawBuffers2 );

  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );

  drawFullscreenQuad();

  pass2Prog->deactivate();

  if (debug == 2) {
    gbuffer->DrawGBuffers();
    return;
  }

  /*
  // Pass 3: Draw everything using data from G-Buffers

  glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );

  pass3Prog->activate();

  pass3Prog->setVec2( "texCoordInc", vec2( 1 / (float) windowWidth, 1 / (float) windowHeight ) );
  pass3Prog->setVec3( "lightDir", lightDir );

  pass3Prog->setInt( "colourSampler",    COLOUR_GBUFFER );
  pass3Prog->setInt( "normalSampler",    NORMAL_GBUFFER );
  pass3Prog->setInt( "depthSampler",     DEPTH_GBUFFER );
  pass3Prog->setInt( "laplacianSampler", LAPLACIAN_GBUFFER );

  gbuffer->BindTexture( COLOUR_GBUFFER );
  gbuffer->BindTexture( NORMAL_GBUFFER );
  gbuffer->BindTexture( DEPTH_GBUFFER );
  gbuffer->BindTexture( LAPLACIAN_GBUFFER  );

  drawFullscreenQuad();

  pass3Prog->deactivate();
  */
}

void Renderer::renderShadowMap( seq<wfModel*> objs, mat4 &sceneM, mat4 &lightV, mat4 &lightP )
{
  // ---------- GENERATE DEPTH TEXTURE ----------

  // glDrawBuffer(GL_NONE); // No color buffer is drawn to

  //---------- SHIT COPIED FROM FARTHER UP ----------

  // Pass 0: Store depth in a G-Buffer

  gbuffer->BindForWriting(); // TODO: declare another FBO in gbuffer.h
                             // Do we need to or can we just write to the depth buffer instead?

  pass0Prog->activate();

  // binds SHADOW_MAP to shader param at (location = 0)
  gbuffer->BindTexture( SHADOW_MAP );

  int activeDrawBuffers1[] = { SHADOW_MAP };
  gbuffer->setDrawBuffers( 1, activeDrawBuffers1 );

  //glClearColor(0.0, 0.0, 0.0, 0.0);

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  //whats this shit
  glEnable( GL_DEPTH_TEST );

  for (int i = 0; i < objs.size(); i++) {
    mat4 MVP = lightP * lightV * objs[i]->Matrix * sceneM;

    // Send our transformation to the currently bound shader, in the "depthMVP" uniform
    pass0Prog->setMat4( "lightMVP", MVP );

    objs[i]->draw( pass0Prog );
  }

  pass0Prog->deactivate();

  if (debug == 1) {
    //this blits the 4 windows in each corner
    gbuffer->DrawGBuffers();
    return;
  }

  /*
  // Pass 2: Store Laplacian (computed from depths) in G-Buffer

  pass2Prog->activate();

  pass2Prog->setVec2( "texCoordInc", vec2( 1 / (float) windowWidth, 1 / (float) windowHeight ) );

  pass2Prog->setInt( "depthSampler", DEPTH_GBUFFER );

  gbuffer->BindTexture( LAPLACIAN_GBUFFER );

  int activeDrawBuffers2[] = { LAPLACIAN_GBUFFER };
  gbuffer->setDrawBuffers( 1, activeDrawBuffers2 );

  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );

  drawFullscreenQuad();

  pass2Prog->deactivate();

  if (debug == 2) {
    gbuffer->DrawGBuffers();
    return;
  }

  // Pass 3: Draw everything using data from G-Buffers

  glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );

  pass3Prog->activate();

  pass3Prog->setVec2( "texCoordInc", vec2( 1 / (float) windowWidth, 1 / (float) windowHeight ) );
  pass3Prog->setVec3( "lightDir", lightDir );

  pass3Prog->setInt( "colourSampler",    COLOUR_GBUFFER );
  pass3Prog->setInt( "normalSampler",    NORMAL_GBUFFER );
  pass3Prog->setInt( "depthSampler",     DEPTH_GBUFFER );
  pass3Prog->setInt( "laplacianSampler", LAPLACIAN_GBUFFER );

  // These have to be re-bound because glBindFramebuffer unbinds stuff (I think)
  gbuffer->BindTexture( COLOUR_GBUFFER );
  gbuffer->BindTexture( NORMAL_GBUFFER );
  gbuffer->BindTexture( DEPTH_GBUFFER );
  gbuffer->BindTexture( LAPLACIAN_GBUFFER  );

  drawFullscreenQuad();

  pass3Prog->deactivate();
  */
}