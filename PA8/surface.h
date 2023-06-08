#ifndef _PA_SURFACE_H_
#define _PA_SURFACE_H_

#include "spline.h"
#include "curve.h"

class Surface : public Spline {
public:
    explicit Surface(int point_num) : Spline(point_num) {}
};

class SurfaceOfRevolution : public Surface {
public:
    explicit SurfaceOfRevolution(Curve *c);

    void Paint(ArgParser *arg_parser) override;

    int getNumVertices() override;

    Vec3f getVertex(int i) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;

    void addControlPoint(int selectedPoint, float x, float y) override;

    void deleteControlPoint(int selectedPoint) override;

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

    Curve *curve;
};

class BezierPatch : public Surface {
public:
    explicit BezierPatch() : Surface(16) {}

    void Paint(ArgParser *arg_parser) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;
};

#endif
