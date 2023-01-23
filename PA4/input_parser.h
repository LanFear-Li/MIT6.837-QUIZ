#ifndef _INPUT_PARSER_H
#define _INPUT_PARSER_H

#include <iostream>
#include <cstring>
#include <cassert>

class InputParser {
public:
    void parse_input(int argc, char *argv[]);

    int width{};
    int height{};

    float depth_min{};
    float depth_max{};

    float num_theta{};
    float num_phi{};

    char *input_file = nullptr;
    char *output_file = nullptr;
    char *depth_file = nullptr;
    char *normal_file = nullptr;

    bool shade_back = false;
    bool gui = false;
    bool gouraud = false;
};

#endif //_INPUT_PARSER_H
