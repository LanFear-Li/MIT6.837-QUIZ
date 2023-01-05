#include <cstring>

#include "ifs.h"
#include "image.h"

const Vec3f WHITE(255, 255, 255);
const Vec3f BLACK(0, 0, 0);

int main(int argc, char *argv[]) {
    IFS ifs;
    string input_file, output_file;
    int point_nums, iter_nums, size;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i], "-points")) {
            i++;
            assert (i < argc);
            point_nums = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-iters")) {
            i++;
            assert (i < argc);
            iter_nums = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert (i < argc);
            size = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert (i < argc);
            output_file = argv[i];
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    Image base_image(size, size);
    ifs.get_description(input_file);
    ifs.render(base_image, point_nums, iter_nums);
    base_image.SaveTGA(output_file.c_str());
    return 0;
}
