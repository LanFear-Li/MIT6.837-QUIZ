#ifndef _PA_SAMPLER_H
#define _PA_SAMPLER_H

#include "vectors.h"

class Sampler {
public:
    virtual Vec2f getSamplePosition(int n) = 0;
};

class RandomSampler : public Sampler {
public:
    Vec2f getSamplePosition(int n) override;
};

class UniformSampler : public Sampler {
public:
    explicit UniformSampler(int nSamples);

    Vec2f getSamplePosition(int n) override;
};

class JitteredSampler : public Sampler {
public:
    explicit JitteredSampler(int nSamples);

    Vec2f getSamplePosition(int n) override;
};

#endif
