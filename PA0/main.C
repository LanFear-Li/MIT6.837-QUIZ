#include "ifs.h"
#include "image.h"

const int WIDTH = 200;
const int HEIGHT = 200;

int main() {
    Image base_image(WIDTH, HEIGHT);
    IFS ifs;
    string filename = "./IFS/sierpinski_triangle.txt";
    int point_nums = 100, iter_nums = 1;

    ifs.get_description(filename);
    ifs.render(base_image, point_nums, iter_nums);
    base_image.SaveTGA("test.tga");

    return 0;
}
