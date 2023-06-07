#ifndef _PA_SURFACE_H_
#define _PA_SURFACE_H_

#include "spline.h"
#include "curve.h"

class Surface : public virtual Spline {};

class SurfaceOfRevolution : public virtual Surface {
public:
    explicit SurfaceOfRevolution(Curve *c);

    void Paint(ArgParser *arg_parser) override;

    void output(FILE *file);

    int getNumVertices() override { return this->point_num; }

    Vec3f getVertex(int i) override { return this->ctrl_point[i]; }

    void moveControlPoint(int selectedPoint, float x, float y) override;

    void addControlPoint(int selectedPoint, float x, float y) override;

    void deleteControlPoint(int selectedPoint) override;
};

class BezierPatch : public virtual Surface {
public:
    explicit BezierPatch() = default;

    void set(int i, const Vec3f& vec) {}
};

#endif
