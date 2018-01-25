// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "buildings.h"


class Silo : public Building {
 public:

  // Constructors 

  Silo() {}

  Silo( vector pos ) : Building( pos ) {

    roundsLeft = 15;
  }

  bool canShoot() {
    return (roundsLeft > 0);
  }

  void decrMissiles() {
    roundsLeft--;
  }

  vector position() {
    return pos;
  }

  // Draw the silo

  void draw() {

    glColor3f( 1,1,1 );

    glBegin( GL_POLYGON );
    for (float theta=0; theta<M_PI+0.01; theta += M_PI/32)
      glVertex2f( pos.x + .04 * cos(theta), pos.y + .04 * sin(theta) );
    glEnd();
  }

 private:

  int roundsLeft;
};
  

#endif
