#ifndef PA0_IFS_H
#define PA0_IFS_H

#include <iostream>
#include <fstream>
#include <cstdio>

#include "matrix.h"
#include "vectors.h"
#include "image.h"

extern const Vec3f WHITE;
extern const Vec3f BLACK;

class IFS {
private:
    int transform_nums;

    // affine transformation for 2d points
    Matrix *transforms;

    // probability is accumulated
    float *transform_probs;

public:
    IFS();

    // generate ifs from txt file
    void get_description(string &filename);

    // render self-similar fractal int image
    void render(Image &image, int point_nums, int iter_nums);

    ~IFS();
};

#endif //PA0_IFS_H
