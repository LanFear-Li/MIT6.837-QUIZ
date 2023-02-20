#include "sampler.h"

Sampler *Sampler::getSampler(const std::string &sample_type, int sample_num) {
    if (sample_type == "-random_samples") {
        return new RandomSampler();
    } else if (sample_type == "-uniform_samples") {
        return new UniformSampler(sample_num);
    } else if (sample_type == "-jittered_samples") {
        return new JitteredSampler(sample_num);
    } else {
        assert(false);
    }
}


Vec2f RandomSampler::getSamplePosition(int n) {
    // random generate points within this pixel [0, 0] -> [1, 1]
    float x = (float) rand() / (float) RAND_MAX;
    float y = (float) rand() / (float) RAND_MAX;
    return {x, y};
}

UniformSampler::UniformSampler(int nSamples) {

}

Vec2f UniformSampler::getSamplePosition(int n) {
    return Vec2f();
}


JitteredSampler::JitteredSampler(int nSamples) {

}

Vec2f JitteredSampler::getSamplePosition(int n) {
    return Vec2f();
}
