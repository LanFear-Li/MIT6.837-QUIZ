#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"

int main(int argc, char *argv[]) {

    // parse the command line arguments & input file
    auto *args = new ArgParser(argc,argv);
    auto* splines = new SplineParser(args->input_file);

    // launch curve editor!
    if (args->gui) {
        glutInit(&argc, argv);
        GLCanvas glcanvas;
        GLCanvas::initialize(args,splines);
        // this never returns...
    }

    // output as required
    splines->SaveBezier(args);
    splines->SaveBSpline(args);
    splines->SaveTriangles(args);

    // cleanup
    delete args;
    delete splines;
    return 0;
}