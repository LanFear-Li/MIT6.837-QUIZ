#ifndef _PA_INPUT_PARSER_H_
#define _PA_INPUT_PARSER_H_

#include <iostream>
#include <cstring>
#include <cassert>

class InputParser {
public:
    InputParser(int argc, char *argv[]);

    int width{};
    int height{};

    float depth_min{};
    float depth_max{};

    char *input_file = nullptr;
    char *output_file = nullptr;
    char *depth_file = nullptr;
    char *normal_file = nullptr;

    bool shade_back = false;
    bool gui = false;

    bool shadows = false;
    int max_bounces{};
    float cutoff_weight{};

    int nx{};
    int ny{};
    int nz{};
    bool with_grid = false;
    bool visualize_grid = false;

    bool stats = false;
};

#endif
