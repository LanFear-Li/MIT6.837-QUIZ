#ifndef _PA_SAMPLER_H_
#define _PA_SAMPLER_H_

#include "vectors.h"

class Sampler {
public:
    virtual Vec2f getSamplePosition(int n) = 0;

    static Sampler *getSampler(const std::string &sample_type, int sample_num);
};

class RandomSampler : public Sampler {
public:
    Vec2f getSamplePosition(int n) override;
};

class UniformSampler : public Sampler {
public:
    explicit UniformSampler(int nSamples);

    Vec2f getSamplePosition(int n) override;

    int rank;
    float step;
};

class JitteredSampler : public UniformSampler {
public:
    explicit JitteredSampler(int nSamples);

    Vec2f getSamplePosition(int n) override;
};

#endif
