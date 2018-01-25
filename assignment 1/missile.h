// Missile class


#ifndef MISSILE_H
#define MISSILE_H


#include "vector.h"

class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vector p, vector v, float y, vector c ) {
    pos0 = p;
    pos1 = p;
    velocity = v;
    colour = c;
    destY = y;
  }

  // Draw the missile and its trail

  void draw() {
    glColor3f( colour.x, colour.y, colour.z );
    glBegin( GL_LINES );
    float lerp = 0;
    vector distVect = pos1 - pos0;
    while (lerp < 1) {
      glColor3f( lerp - colour.x, lerp - colour.y, lerp - colour.z);
      vector lerpVect = distVect * lerp;
      glVertex2f( pos0.x + lerpVect.x, pos0.y + lerpVect.y);
      lerp += 0.1;
      lerpVect = distVect * lerp;
      glVertex2f( pos0.x + lerpVect.x, pos0.y + lerpVect.y);
      glColor3f( colour.x, colour.y, colour.z );
    }
    glEnd();
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;
  }

  // Return the current position 

  vector position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {
    //if moving upwards
    if (velocity.y >= 0) {
      return pos1.y >= destY;
    } else { //if moving downward
      return pos1.y <= destY;
    };
  }

 private:

  vector pos0;			// start position
  vector pos1;			// current position
  vector velocity;		// velocity
  vector colour;		// colour of missile trail
  float  destY;			// y position at destination
};


#endif
