#include "input_parser.h"

// arguments for sphere
extern bool gouraud;
extern int num_theta, num_phi;

InputParser::InputParser(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert (i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert (i < argc);
            width = atoi(argv[i]);
            i++;
            assert (i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-depth")) {
            i++;
            assert (i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert (i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert (i < argc);
            depth_file = argv[i];
        } else if (!strcmp(argv[i], "-normals")) {
            i++;
            assert (i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i], "-shade_back")) {
            shade_back = true;
        } else if (!strcmp(argv[i], "-gui")) {
            gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++;
            assert (i < argc);
            num_theta = atof(argv[i]);
            i++;
            assert (i < argc);
            num_phi = atof(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++;
            assert (i < argc);
            max_bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++;
            assert (i < argc);
            cutoff_weight = atof(argv[i]);
        } else if (!strcmp(argv[i], "-grid")) {
            with_grid = true;
            i++;
            assert (i < argc);
            nx = atoi(argv[i]);
            i++;
            assert (i < argc);
            ny = atoi(argv[i]);
            i++;
            assert (i < argc);
            nz = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-visualize_grid")) {
            visualize_grid = true;
        } else if (!strcmp(argv[i], "-stats")) {
            stats = true;
        } else if (!strcmp(argv[i], "-random_samples") || !strcmp(argv[i], "-uniform_samples") ||
                   !strcmp(argv[i], "-jittered_samples")) {
            sample_type = argv[i];

            i++;
            assert (i < argc);
            num_samples = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-box_filter") || !strcmp(argv[i], "-tent_filter") ||
                   !strcmp(argv[i], "-gaussian_filter")) {
            filter_type = argv[i];

            i++;
            assert (i < argc);
            filter_param = atof(argv[i]);
        } else if (!strcmp(argv[i], "-render_samples")) {
            render_samples = true;

            i++;
            assert (i < argc);
            sample_file = argv[i];

            i++;
            assert (i < argc);
            sample_zoom_factor = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-render_filter")) {
            render_filter = true;

            i++;
            assert (i < argc);
            filter_file = argv[i];

            i++;
            assert (i < argc);
            filter_zoom_factor = atoi(argv[i]);
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}