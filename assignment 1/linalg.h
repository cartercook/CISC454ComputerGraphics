// linalg.h


#ifndef LINALG_H
#define LINALG_H


#include <iostream>
#include <math.h>

#ifdef _WIN32
  #pragma warning(disable : 4244 4305 4996)
#endif


// ---------------- vec3 ----------------

class vec3 {
public:

  float x, y, z;

  vec3() {}

  vec3( float xx, float yy, float zz )
    { x = xx; y = yy; z = zz; }

  bool operator == (const vec3 p) {
    return x == p.x && y == p.y && z == p.z;
  }

  bool operator != (const vec3 p) {
    return x != p.x || y != p.y || z != p.z; 
  }

  vec3 operator + (vec3 p)
    { return vec3( x+p.x, y+p.y, z+p.z ); }

  vec3 operator - (vec3 p)
    { return vec3( x-p.x, y-p.y, z-p.z ); }

  float operator * (vec3 p)	/* dot product */
    { return x * p.x + y * p.y + z * p.z; }

  vec3 operator ^ (vec3 p)	/* cross product */
    { return vec3( y*p.z-p.y*z, -(x*p.z-p.x*z), x*p.y-p.x*y ); }

  vec3 normalize() {
    float len;
    len = sqrt( x*x + y*y + z*z );
    return vec3( x/len, y/len, z/len );
  }

  float length()
    { return sqrt( x*x + y*y + z*z ); }

  float squaredLength()
    { return x*x + y*y + z*z; }

  float & operator[]( unsigned int index ) {
    return (&x)[index];
  }
  
  vec3 perp1();
  vec3 perp2();
};

// Scalar/vec3 multiplication

vec3 operator * ( float k, vec3 const& p );

// I/O operators

std::ostream& operator << ( std::ostream& stream, vec3 const& p );
std::istream& operator >> ( std::istream& stream, vec3 & p );



// ---------------- vec4 ----------------



class vec4 {
public:

  float x, y, z, w;

  vec4() {}

  vec4( float xx, float yy, float zz, float ww )
    { x = xx; y = yy; z = zz; w = ww; }

  bool operator == (const vec4 p) const
  { return x == p.x && y == p.y && z == p.z && w == p.w; }

  bool operator != (const vec4 p) const
  { return x != p.x || y != p.y || z != p.z || w != p.w; }

  vec4 operator + (vec4 p) const
  { return vec4( x+p.x, y+p.y, z+p.z, w+p.w ); }

  vec4 operator - (vec4 p) const
  { return vec4( x-p.x, y-p.y, z-p.z, w-p.w ); }

  float operator * (vec4 const &p) const
    { return x * p.x + y * p.y + z * p.z + w * p.w; }

  vec4 normalize() {
    float len;
    len = sqrt( x*x + y*y + z*z + w*w );
    return vec4( x/len, y/len, z/len, w/len );
  }

  vec3 tovec3() {
    return vec3( x/w, y/w, z/w );
  }

  float length() const
  { return sqrt( x*x + y*y + z*z + w*w ); }

  float squaredLength() const
  { return x*x + y*y + z*z + w*w; }

  float & operator[]( unsigned int index ) {
    return (&x)[index];
  }
  
};

// Scalar/vec4 multiplication

vec4 operator * ( float k, vec4 const& p );

// I/O operators

std::ostream& operator << ( std::ostream& stream, vec4 const& p );
std::istream& operator >> ( std::istream& stream, vec4 & p );


// ---------------- mat4 ----------------


class mat4 {

 public:
  
  vec4 rows[4];

  mat4() {}

  vec4 & operator[]( unsigned int index ) const {
    return ((vec4*)(&rows[0]))[index];
  }
};


// operations

mat4 operator * (       float k, mat4 const& m );
vec4 operator * ( mat4 const& m, vec4 const& v );
mat4 operator * ( mat4 const& m, mat4 const& n );

mat4 identity();

mat4 scale( float x, float y, float z );
mat4 translate( float x, float y, float z );
mat4 translate( vec3 v );
mat4 rotate( float theta, vec3 axis );

mat4 frustum( float l, float r, float b, float t, float n, float f );
mat4 ortho( float l, float r, float b, float t, float n, float f );
mat4 perspective( float fovy, float aspect, float n, float f );

// I/O operators

std::ostream& operator << ( std::ostream& stream, mat4 const& m );
std::istream& operator >> ( std::istream& stream, mat4 & m );

#endif