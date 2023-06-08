#include "curve.h"

Vec3f CurveSection::calculatePoint(float t) const {
    auto basis = Vec4f(t * t * t, t * t, t, 1);
    (this->G * *this->B).Transform(basis);
    basis.DivideByW();
    return basis.xyz();
}

void Curve::Paint(ArgParser *arg_parser) {
    // control points
    glColor3f(1.f, 0.f, 0.f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < this->point_num; i++) {
        glVertex3fv(this->ctrl_point[i].GetAll());
    }
    glEnd();

    // control lines
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < this->point_num; i++) {
        glVertex3fv(this->ctrl_point[i].GetAll());
    }
    glEnd();

    // curves
    int tessellation = arg_parser->curve_tessellation;
    generateSections();
    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto &section: this->sections) {
        for (int i = 0; i <= tessellation; i++) {
            glVertex3fv(section.calculatePoint(1.0f / tessellation * i).GetAll());
        }
    }
    glEnd();
}

void Curve::generateSection(int section_idx, int begin_idx) {
    auto &sec = this->sections[section_idx];
    sec.B = getB();
    Vec4f v[4];
    for (int j = 0; j < 4; ++j) {
        v[j] = Vec4f(this->ctrl_point[begin_idx + j], 1.0f);
    }
    sec.G = Matrix(v);
}

std::vector<CurveSection> Curve::getSections() {
    generateSections();
    return this->sections;
}


static float Bernstein_Bezier[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 3.f, 0.f,
        -3.f, 3.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 0.f
};
Matrix BezierCurve::basis_bezier = Matrix(Bernstein_Bezier);

Matrix *BezierCurve::getB() {
    return &BezierCurve::basis_bezier;
}

void BezierCurve::generateSections() {
    int section_num = (point_num - 1) / 3;
    this->sections.resize(static_cast<unsigned long>(section_num));
    for (int i = 0; i < section_num; ++i) {
        int index = 3 * i;
        generateSection(i, index);
    }
}

BSplineCurve *BezierCurve::bezierToBSpline() {
    assert(this->point_num == 4);

    generateSections();

    auto *bspline = new BSplineCurve(this->point_num);

    Matrix B_bspline_i = *bspline->getB();
    B_bspline_i.Inverse();
    Matrix G_bspline = this->getSections()[0].G * (*this->getB() * B_bspline_i);

    for (int i = 0; i < this->point_num; i++) {
        bspline->ctrl_point[i] = Vec3f(G_bspline.Get(i, 0), G_bspline.Get(i, 1), G_bspline.Get(i, 2));
    }

    return bspline;
}

void BezierCurve::OutputBezier(FILE *file) {
    fprintf(file, "%s\nnum_vertices %d\n", "bezier", this->point_num);
    for (const auto &p: this->ctrl_point) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

void BezierCurve::OutputBSpline(FILE *file) {
    auto *bspline = this->bezierToBSpline();

    fprintf(file, "%s\nnum_vertices %d\n", "bspline", bspline->point_num);
    for (const auto &p: bspline->ctrl_point) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

vector<Vec3f> BezierCurve::tessellatePoint(ArgParser *arg_parser) {
    int tessellate_num = arg_parser->curve_tessellation;
    generateSections();
    vector<Vec3f> points;

    float step = 1.0f / tessellate_num;
    for (const auto &section: this->sections) {
        float cur = 0;
        for (int i = 0; i <= tessellate_num; i++) {
            points.push_back(section.calculatePoint(cur));
            cur += step;
        }
    }

    return points;
}


static float Bernstein_BSpline[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 0.f, 4.f,
        -3.f, 3.f, 3.f, 1.f,
        1.f, 0.f, 0.f, 0.f
};

Matrix BSplineCurve::basis_bspline = Matrix(Bernstein_BSpline) * (1.f / 6.f);

Matrix *BSplineCurve::getB() {
    return &BSplineCurve::basis_bspline;
}

void BSplineCurve::generateSections() {
    int section_num = this->point_num - 3;
    sections.resize(static_cast<unsigned long>(section_num));
    for (int i = 0; i < section_num; ++i) {
        int index = i;
        generateSection(i, index);
    }
}

BezierCurve *BSplineCurve::bSplineToBezier() {
    assert(this->point_num == 4);

    generateSections();

    auto *bezier = new BezierCurve(this->point_num);

    Matrix B_bezier_i = *bezier->getB();
    B_bezier_i.Inverse();
    Matrix G_bezier = this->getSections()[0].G * *this->getB() * B_bezier_i;

    for (int i = 0; i < this->point_num; i++) {
        bezier->ctrl_point[i] = Vec3f(G_bezier.Get(i, 0), G_bezier.Get(i, 1), G_bezier.Get(i, 2));
    }

    return bezier;
}

void BSplineCurve::OutputBSpline(FILE *file) {
    fprintf(file, "%s\nnum_vertices %d\n", "bspline", this->point_num);
    for (const auto &p: this->ctrl_point) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

void BSplineCurve::OutputBezier(FILE *file) {
    auto *bezier = this->bSplineToBezier();
    fprintf(file, "%s\nnum_vertices %d\n", "bezier", bezier->point_num);
    for (const auto &p: bezier->ctrl_point) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

vector<Vec3f> BSplineCurve::tessellatePoint(ArgParser *arg_parser) {
    int tessellate_num = arg_parser->curve_tessellation;
    generateSections();
    vector<Vec3f> points;

    // for bspline: the first three points are the same
    float step = 1.0f / tessellate_num, cur = 0;
    for (const auto &section: this->sections) {
        cur = 0;
        for (int i = 0; i <= tessellate_num; i++) {
            points.push_back(section.calculatePoint(1.0f - cur));
            cur += step;
        }
    }

    return points;
}
