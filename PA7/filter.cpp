#include "filter.h"

Filter::Filter() = default;

Filter *Filter::getFilter(const std::string &filter_type, float filter_param) {
    if (filter_type == "-box_filter") {
        return new BoxFilter(filter_param);
    } else if (filter_type == "-tent_filter") {
        return new TentFilter(filter_param);
    } else if (filter_type == "-gaussian_filter") {
        return new GaussianFilter(filter_param);
    } else {
        assert(false);
    }
}

Vec3f Filter::getColor(int i, int j, Film *film) {
    int radius = getSupportRadius();

    float weight_sum = 0.0f;
    Vec3f color;

    // iterate pixels around current pixel (i, j) within radius range
    for (int offset_x = -radius; offset_x <= radius; offset_x++) {
        for (int offset_y = -radius; offset_y <= radius; offset_y++) {
            int x = i + offset_x, y = j + offset_y;
            if (x < 0 || x >= film->getWidth() || y < 0 || y >= film->getHeight()) {
                continue;
            }

            // literate all sample of pixel (i + offset_x, j + offset_y) also (x, y) and accumulate weighted color
            for (int n = 0; n < film->getNumSamples(); n++) {
                Sample sample = film->getSample(x, y, n);

                Vec2f sample_loc = sample.getPosition();
                Vec2f inside_loc(sample_loc.x() - 0.5f + offset_x, sample_loc.y() - 0.5f + offset_y);

                float weight = getWeight(inside_loc.x(), inside_loc.y());
                color += sample.getColor() * weight;
                weight_sum += weight;
            }
        }
    }

    return color * (1.0f / weight_sum);
}

Filter::~Filter() = default;


BoxFilter::BoxFilter(float radius) {
    this->radius = radius;
}

float BoxFilter::getWeight(float x, float y) {
    return fabs(x) <= this->radius && fabs(y) <= this->radius;
}

int BoxFilter::getSupportRadius() {
    return int(ceil(this->radius - 0.5f));
}


TentFilter::TentFilter(float radius) {
    this->radius = radius;
}

float TentFilter::getWeight(float x, float y) {
    float dis = sqrt(x * x + y * y);
    float ans = 1.0f - dis / this->radius;

    return max(0.0f, ans);
}

int TentFilter::getSupportRadius() {
    return int(ceil(this->radius - 0.5f));
}


GaussianFilter::GaussianFilter(float sigma) {
    this->sigma = sigma;
}

float GaussianFilter::getWeight(float x, float y) {
    float dis = x * x + y * y;
    return exp(-dis / (2.0f * this->sigma * this->sigma));
}

int GaussianFilter::getSupportRadius() {
    return int(ceil(2.0f * this->sigma - 0.5f));
}
