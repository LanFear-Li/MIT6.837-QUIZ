#ifndef _PA_FILTER_H
#define _PA_FILTER_H

#include "vectors.h"
#include "film.h"

class Filter {
public:
    Filter() {}

    Vec3f getColor(int i, int j, Film *film);

    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {

};

class TentFilter : public Filter {

};

class GaussianFilter : public Filter {

};

#endif
