#ifndef _PA_RAY_H_
#define _PA_RAY_H_

#include <iostream>
using namespace std;

#include "vectors.h"

// ====================================================================
// ====================================================================

// Ray class mostly copied from Peter Shirley and Keith Morley

class Ray {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Ray () {}
  Ray (const Vec3f &orig, const Vec3f &dir) {
    origin = orig; 
    direction = dir; }
  Ray (const Ray& r) {*this=r;}

  // ACCESSORS
  const Vec3f& getOrigin() const { return origin; }
  const Vec3f& getDirection() const { return direction; }
  Vec3f pointAtParameter(float t) const {
    return origin+direction*t; }

private:

  // REPRESENTATION
  Vec3f origin;
  Vec3f direction;
};

inline ostream &operator<<(ostream &os, const Ray &r) {
  os << "Ray <o:" <<r.getOrigin()<<", d:"<<r.getDirection()<<">";
  return os;
}

// ====================================================================
// ====================================================================

#endif
