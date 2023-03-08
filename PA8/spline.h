#ifndef _PA_SPLINE_H_
#define _PA_SPLINE_H_

#include <iostream>
#include <vector>

#include "vectors.h"
#include "arg_parser.h"
#include "triangle_mesh.h"

class Spline {
public:
    virtual void Paint(ArgParser *arg_parser) = 0;

    virtual void OutputBezier(FILE *file) = 0;

    virtual void OutputBSpline(FILE *file) = 0;

    virtual int getNumVertices() = 0;

    virtual Vec3f getVertex(int i) = 0;

    virtual void moveControlPoint(int selectedPoint, float x, float y) = 0;

    virtual void addControlPoint(int selectedPoint, float x, float y) = 0;

    virtual void deleteControlPoint(int selectedPoint) = 0;

    virtual TriangleMesh* OutputTriangles(ArgParser *args) = 0;

    int point_num;

    vector<Vec3f> ctrl_point;
};

#endif
