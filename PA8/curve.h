#ifndef _PA_CURVE_H_
#define _PA_CURVE_H_

#include <GL/gl.h>
#include <GL/glut.h>

#include "spline.h"
#include "matrix.h"

class CurveSection {
public:
    Matrix G, *B;

    Vec3f calculatePoint(float t) const;
};

class Curve : public Spline {
public:
    explicit Curve(int point_num) : Spline(point_num) {}

    explicit Curve(const vector<Vec3f>& points) : Spline(points) {}

    void Paint(ArgParser *arg_parser) override;

    virtual vector<Vec3f> tessellatePoint(ArgParser *arg_parser) = 0;

    virtual Matrix *getB() = 0;

    void generateSection(int section_idx, int begin_idx);

    virtual void generateSections() = 0;

    std::vector<CurveSection> getSections();

    std::vector<CurveSection> sections;
};

class BezierCurve;

class BSplineCurve;

class BezierCurve : public Curve {
public:
    explicit BezierCurve(int point_num) : Curve(point_num) {}

    explicit BezierCurve(const vector<Vec3f>& points) : Curve(points) {}

    Matrix *getB() override;

    static Matrix basis_bezier;

    void generateSections() override;

    BSplineCurve *bezierToBSpline();

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;

    vector<Vec3f> tessellatePoint(ArgParser *arg_parser) override;
};

class BSplineCurve : public Curve {
public:
    explicit BSplineCurve(int point_num) : Curve(point_num) {}

    explicit BSplineCurve(const vector<Vec3f>& points) : Curve(points) {}

    Matrix *getB() override;

    static Matrix basis_bspline;

    void generateSections() override;

    BezierCurve *bSplineToBezier();

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;

    vector<Vec3f> tessellatePoint(ArgParser *arg_parser) override;
};

#endif
