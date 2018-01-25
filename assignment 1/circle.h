// Circle class


#ifndef CIRCLE_H
#define CIRCLE_H


#include "vector.h"

class Circle {
 public:

  // Constructors

  Circle() {}

  Circle( vector p, float s, float maxRad, vector c ) {
    pos = p;
    r = 0;
    maxR = maxRad;
    speed = s;
    colour = c;
  }

  // Draw the circle

  void draw() {
    glColor3f( colour.x, colour.y, colour.z );
    glBegin( GL_POLYGON );
    for (float theta=0; theta<2*M_PI; theta += M_PI/32)
      glVertex2f( pos.x + r * sin(theta), pos.y + r * cos(theta) );
    glEnd();
  }

  // Expand the circle over a time interval, deltaT

  void expand( float deltaT ) {
    r = r + deltaT * speed;
  }

  // Return the current and max radii

  float radius() {
    return r;
  }

  float maxRadius() {
    return maxR;
  }

  vector position() {
    return pos;
  }

 private:

  vector pos;			// centre
  float  r;			// current radius
  float  maxR;			// maximum radius
  float  speed;			// speed at which radius increases
  vector colour;		// colour of circle
};


#endif
