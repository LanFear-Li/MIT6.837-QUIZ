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
    return Vec3f();
}

Filter::~Filter() = default;


BoxFilter::BoxFilter(float filter_param) {

}

float BoxFilter::getWeight(float x, float y) {
    return 0;
}

int BoxFilter::getSupportRadius() {
    return 0;
}


TentFilter::TentFilter(float filter_param) {

}

float TentFilter::getWeight(float x, float y) {
    return 0;
}

int TentFilter::getSupportRadius() {
    return 0;
}


GaussianFilter::GaussianFilter(float filter_param) {

}

float GaussianFilter::getWeight(float x, float y) {
    return 0;
}

int GaussianFilter::getSupportRadius() {
    return 0;
}
