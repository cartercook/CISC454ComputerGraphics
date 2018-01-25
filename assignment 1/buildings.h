// Types of building


#ifndef BUILDING_H
#define BUILDING_H


#include "vector.h"


class Building {
 public:

  // Constructors

  Building() {}

  Building( vector p ) {
    pos = p;
  }

  // Draw the building

  virtual void draw() = 0;

 protected:

  vector pos;			// centre
};



#endif
