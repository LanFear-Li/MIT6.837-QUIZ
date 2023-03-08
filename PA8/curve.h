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

class Curve : public virtual Spline {
public:
    void Paint(ArgParser *arg_parser) override;

    void output(FILE *file);

    int getNumVertices() override { return 0; }

    Vec3f getVertex(int i) override { return {}; }

    void moveControlPoint(int selectedPoint, float x, float y) override {}

    void addControlPoint(int selectedPoint, float x, float y) override {}

    void deleteControlPoint(int selectedPoint) override {}

    TriangleMesh *OutputTriangles(ArgParser *args) override { return {}; }

    virtual Matrix *getB() = 0;

    void generateSection(int section_idx, int begin_idx);

    virtual void generateSections() = 0;

    std::vector<CurveSection> getSections();

    std::vector<CurveSection> sections;

    string curve_type;
};

class BezierCurve;

class BSplineCurve;

class BezierCurve : public virtual Curve {
public:
    explicit BezierCurve(int num_vertices);

    void set(int i, const Vec3f &vec);

    Matrix *getB() override;

    static Matrix _B;

    void generateSections() override;

    BSplineCurve *bezierToBSpline();

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;
};

class BSplineCurve : public virtual Curve {
public:
    explicit BSplineCurve(int num_vertices);

    void set(int i, const Vec3f &vec);

    Matrix *getB() override;

    static Matrix mat_b;

    void generateSections() override;

    BezierCurve *bSplineToBezier();

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;
};

#endif
