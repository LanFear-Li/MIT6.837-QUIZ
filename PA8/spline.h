#ifndef _PA_SPLINE_H_
#define _PA_SPLINE_H_

#include <iostream>
#include <vector>

#include "vectors.h"
#include "arg_parser.h"
#include "triangle_mesh.h"

class Spline {
public:
    explicit Spline(int point_num);

    virtual void Paint(ArgParser *arg_parser) = 0;

    virtual void OutputBezier(FILE *file) {}

    virtual void OutputBSpline(FILE *file) {}

    virtual void set(int i, const Vec3f &p);

    virtual int getNumVertices();

    virtual Vec3f getVertex(int i);

    virtual void moveControlPoint(int selectedPoint, float x, float y);

    virtual void addControlPoint(int selectedPoint, float x, float y);

    virtual void deleteControlPoint(int selectedPoint);

    virtual TriangleMesh* OutputTriangles(ArgParser *args) {};

    int point_num;

    vector<Vec3f> ctrl_point;
};

#endif
