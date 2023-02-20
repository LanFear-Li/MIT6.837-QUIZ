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
    explicit BoxFilter(float filter_param);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;
};

class TentFilter : public Filter {
public:
    explicit TentFilter(float filter_param);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;
};

class GaussianFilter : public Filter {
public:
    explicit GaussianFilter(float filter_param);

    float getWeight(float x, float y) override;

    int getSupportRadius() override;
};

#endif
