#ifndef _PA_CURVE_H_
#define _PA_CURVE_H_

#include "spline.h"

class Curve : public virtual Spline {
public:
    void Paint(ArgParser *arg_parser) override {}

    void OutputBezier(FILE *file) override {}

    void OutputBSpline(FILE *file) override {}

    int getNumVertices() override { return 0; }

    Vec3f getVertex(int i) override {return {}; }

    void moveControlPoint(int selectedPoint, float x, float y) override {}

    void addControlPoint(int selectedPoint, float x, float y) override {}

    void deleteControlPoint(int selectedPoint) override {}

    TriangleMesh* OutputTriangles(ArgParser *args) override { return {}; }
};

class BezierCurve : public virtual Curve {
public:
    explicit BezierCurve(int num_vertices) {}

    void set(int i, Vec3f vec) {}
};

class BSplineCurve : public virtual Curve {
public:
    explicit BSplineCurve(int num_vertices) {}

    void set(int i, Vec3f vec) {}
};

#endif
