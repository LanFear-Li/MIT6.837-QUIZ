#ifndef PA0_IFS_H
#define PA0_IFS_H

#include <iostream>
#include <fstream>

#include "matrix.h"
#include "image.h"

class IFS {
private:
    int transform_nums;
    Matrix *transforms;
    float *transform_probs;

public:
    IFS();

    void get_description(string &filename);

    void render(Image &image, int point_nums, int iter_nums);

    ~IFS();
};

#endif //PA0_IFS_H
