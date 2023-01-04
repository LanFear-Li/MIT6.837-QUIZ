#include "ifs.h"

IFS::IFS() {
    transform_nums = 0;
    transforms = new Matrix[1024];
    transform_probs = new float[1024];
}

void IFS::get_description(string &filename) {
    ifstream file(filename);
    int nums;

    float probability, mat_value;
    if (file.is_open()) {
        file >> nums;
        transform_nums = nums;

        for (int k = 0; k < nums; k++) {
            file >> probability;

            Matrix temp_mat;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    file >> mat_value;
                    temp_mat.Set(i, j, mat_value);
                }
            }

            transform_probs[k] = probability;
            transforms[k] = temp_mat;
        }
    } else {
        std::cerr << "Error: file not existed: " << filename << std::endl;
    }
}

void IFS::render(Image &image, int point_nums, int iter_nums) {
}

IFS::~IFS() {
    delete[] transforms;
    delete[] transform_probs;
}
