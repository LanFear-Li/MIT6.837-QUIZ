#include "ifs.h"

float get_random_num(int x, int y) {
    // generate random float int between [x, y)
    return (rand() * 1.0f / RAND_MAX) * (y - x) + x;
}

Vec2f get_random_point(int width, int height) {
    // generate random float between [0, width) and [0, height)
    float x = (rand() * 1.0f / RAND_MAX) * width;
    float y = (rand() * 1.0f / RAND_MAX) * height;
    return Vec2f(x, y);
}

IFS::IFS() {
    transform_nums = 0;
    transforms = new Matrix[1024];
    transform_probs = new float[1024];
}

void IFS::get_description(string &filename) {
    FILE *file = fopen(filename.c_str(), "r");
    if (file == nullptr) {
        std::cerr << "Error: file not existed: " << filename << std::endl;
        return;
    }

    float probability, prob_acc = 0;

    fscanf(file, "%d", &transform_nums);
    for (int k = 0; k < transform_nums; k++) {
        fscanf(file, "%f", &probability);
        transforms[k].Read3x3(file);

        prob_acc += probability;
        transform_probs[k] = prob_acc;
    }

    fclose(file);
}

void IFS::render(Image &image, int point_nums, int iter_nums) {
    int width = image.Width();
    int height = image.Height();
    Vec2f point;

    image.SetAllPixels(WHITE);
    for (int p = 0; p < point_nums; p++) {
        point = get_random_point(1, 1);

        for (int k = 0; k < iter_nums; k++) {
            int kth = 0;
            float random = get_random_num(0, 1);

            for (int i = 0; i < transform_nums; i++) {
                if (transform_probs[i] >= random) {
                    kth = i;
                    break;
                }
            }

            transforms[kth].Transform(point);
        }

        if (point.x() >= 0 && point.x() <= 1 && point.y() >= 0 && point.y() <= 1) {
            image.SetPixel(point.x() * width, point.y() * height, BLACK);
        }
    }
}

IFS::~IFS() {
    delete[] transforms;
    delete[] transform_probs;
}