#ifndef _PA_SURFACE_H_
#define _PA_SURFACE_H_

#include "spline.h"
#include "curve.h"

class Surface : public virtual Spline {
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

class SurfaceOfRevolution : public virtual Surface {
public:
    explicit SurfaceOfRevolution(Curve *c) {}

    void set(int i, Vec3f vec) {}
};

class BezierPatch : public virtual Surface {
public:
    explicit BezierPatch() = default;

    void set(int i, Vec3f vec) {}
};

#endif
