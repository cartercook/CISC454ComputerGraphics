// state.cpp


#include "headers.h"

#include "state.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;

  for (i=0; i<silos.size(); i++)
    silos[i].draw();

  for (i=0; i<cities.size(); i++)
    cities[i].draw();

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw();

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw();

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw();

  for (i=0; i<implosions.size(); i++)
    implosions[i].draw();
}


// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


void State::updateState( float deltaT )

{
  int i;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // should increase with time.
  std::cout << currentTime / (60*3) << std::endl;
  if (randIn01() < 0.01 + std::min(currentTime / (60*5), 0.1f)) {	// New missile with probability 10%
    vector source = vector( randIn01(), worldTop, 0);

    missilesIn.add(
      Missile(
        source,                                                                       // source
			  (vector(randIn01(), 0, 0) - source).normalize() / 20,   // velocity
			  0,                                                                            // destination y
			  vector( 1,1,0 )                                                               // colour
      )
    );
  }

  // Look for terminating missiles

  for (i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
      explosions.add(Circle(missilesIn[i].position(), 0.1, 0.03, vector(1, 0, 0)));

      missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
      explosions.add(Circle(missilesOut[i].position(), 0.1, 0.05, vector(1, 0.5, 0)));

      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (i=0; i<explosions.size(); i++) {
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO
      for (int j=0; j<cities.size(); j++) {
        if (cities[j].isHit(explosions[i].position(), explosions[i].radius())) {
          cities.remove(j);
        }
      }

      for (int k=0; k<silos.size(); k++) {
        if ((silos[k].position() - explosions[i].position()).length() <= explosions[i].radius()) {
          silos.remove(k);
        }
      }

      for (int l=0; l<missilesIn.size(); l++) {
        if ((missilesIn[l].position() - explosions[i].position()).length() <= explosions[i].radius()) {
          missilesIn.remove(l);
        }
      }
      
      implosions.add(explosions[i]);
      explosions.remove(i);
      i--;
    }
  }

  for (i=0; i<implosions.size(); i++) {
    if (implosions[i].radius() <= 0) {
      implosions.remove(i);
      i--;
    }
  }
  // Look for incoming missiles that hit an explosion and are
  // destroyed

     // ADD CODE HERE


  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );

  for (i=0; i<implosions.size(); i++)
    implosions[i].expand( -deltaT );
}


// Fire a missile

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.3;
    
  if (silos.size() > siloIndex && silos[siloIndex].canShoot()) {

    silos[siloIndex].decrMissiles();

    // CHANGE THIS

    missilesOut.add(
      Missile(
        silos[siloIndex].position(),                                         // source
			  speed * (vector(x, y,0) - silos[siloIndex].position()).normalize(),  // velocity
			  y,                                                                   // destination y
			  vector( 0,1,1 )                                                      // colour
      )
    );
  }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vector( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vector( 0.2, 0, 0 ) ) );
  cities.add( City( vector( 0.3, 0, 0 ) ) );
  cities.add( City( vector( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vector( 0.6, 0, 0 ) ) );
  cities.add( City( vector( 0.7, 0, 0 ) ) );
  cities.add( City( vector( 0.8, 0, 0 ) ) );
}
