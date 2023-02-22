#ifndef _PA_FILTER_H_
#define _PA_FILTER_H_

#include "vectors.h"
#include "film.h"

class Filter {
public:
    Filter();

    Vec3f getColor(int i, int j, Film *film);

    static Filter *getFilter(const std::string &filter_type, float filter_param);

    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;

    ~Filter();
};

class BoxFilter : public Filter {
public:
    explicit BoxFilter(float radius);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

    float radius;
};

class TentFilter : public Filter {
public:
    explicit TentFilter(float radius);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

    float radius;
};

class GaussianFilter : public Filter {
public:
    explicit GaussianFilter(float sigma);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

    float sigma;
};

#endif
