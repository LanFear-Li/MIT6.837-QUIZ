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
    int k =  sqrt(nSamples);
    if (k * k < nSamples) {
        k += 1;
    }

    this->rank = k;
    this->step = 1.0f / k;
}

Vec2f UniformSampler::getSamplePosition(int n) {
    int x = n % this->rank, y = n / this->rank;
    return {this->step * (x + 0.5f), this->step * (y + 0.5f)};
}


JitteredSampler::JitteredSampler(int nSamples) : UniformSampler(nSamples) {}

Vec2f JitteredSampler::getSamplePosition(int n) {
    Vec2f uniform_loc = UniformSampler::getSamplePosition(n);

    float x = ((float) rand() / (float) RAND_MAX - 0.5f) * this->step;
    float y = ((float) rand() / (float) RAND_MAX - 0.5f) * this->step;

    Vec2f jittered_loc = uniform_loc + Vec2f(x, y);
    jittered_loc.Clamp();
    return jittered_loc;
}
