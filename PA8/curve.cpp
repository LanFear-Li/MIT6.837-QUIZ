#include "curve.h"

Vec3f CurveSection::calculatePoint(float t) const {
    auto basis = Vec4f(t * t * t, t * t, t, 1);
    (G * *B).Transform(basis);
    basis.DivideByW();
    return basis.xyz();
}

void Curve::Paint(ArgParser *arg_parser) {
    // control points
    glColor3f(1.f, 0.f, 0.f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < point_num; i++) {
        glVertex3fv(ctrl_point[i].GetAll());
    }
    glEnd();

    // control lines
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < point_num; i++) {
        glVertex3fv(ctrl_point[i].GetAll());
    }
    glEnd();

    // curves
    int tessellation = arg_parser->curve_tessellation;
    generateSections();
    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& section : this->sections) {
        for (int i = 0; i <= tessellation; i++) {
            glVertex3fv(section.calculatePoint(1.f / tessellation * i).GetAll());
        }
    }
    glEnd();
}

void Curve::output(FILE *file) {
    fprintf(file, "%s\nnum_vertices %d\n", this->curve_type.c_str(), this->point_num);
    for (const auto &p: this->ctrl_point) {
        fprintf(file, "%.2f %.2f %.2f\n", p.x(), p.y(), p.z());
    }
}

void Curve::generateSection(int section_idx, int begin_idx) {
    auto &sec = this->sections[section_idx];
    sec.B = getB();
    Vec4f v[4];
    for (int j = 0; j < 4; ++j) {
        v[j] = Vec4f(this->ctrl_point[begin_idx + j], 1.f);
    }
    sec.G = Matrix(v);
}

std::vector<CurveSection> Curve::getSections() {
    generateSections();
    return sections;
}


static float Bernstein_Bezier[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 3.f, 0.f,
        -3.f, 3.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 0.f
};
Matrix BezierCurve::_B = Matrix(Bernstein_Bezier);

BezierCurve::BezierCurve(int num_vertices) {
    this->curve_type = "bezier";
    this->point_num = num_vertices;

    this->ctrl_point.clear();
    this->ctrl_point.resize(num_vertices);
}

void BezierCurve::set(int i, const Vec3f& vec) {
    assert(i >= 0 && i < this->point_num);
    this->ctrl_point[i] = vec;
}

Matrix *BezierCurve::getB() {
    return &_B;
}

void BezierCurve::generateSections() {
    int section_num = (point_num - 1) / 3;
    sections.resize(static_cast<unsigned long>(section_num));
    for (int i = 0; i < section_num; ++i) {
        int index = 3 * i;
        generateSection(i, index);
    }
}

BSplineCurve *BezierCurve::bezierToBSpline() {
    generateSections();

    auto *bspline = new BSplineCurve(this->point_num);

    Matrix B_bspline_i = *bspline->getB();
    B_bspline_i.Inverse();
    Matrix G_bspline = this->getSections()[0].G * *this->getB() * B_bspline_i;

    for (int i = 0; i < this->point_num; i++) {
        bspline->ctrl_point[i] = Vec3f(G_bspline.Get(0, i), G_bspline.Get(1, i), G_bspline.Get(2, i));
    }

    return bspline;
}

void BezierCurve::OutputBezier(FILE *file) {
    output(file);
}

void BezierCurve::OutputBSpline(FILE *file) {
    bezierToBSpline()->output(file);
}


static float Bernstein_BSpline[16] = {
        -1.f, 3.f, -3.f, 1.f,
        3.f, -6.f, 0.f, 4.f,
        -3.f, 3.f, 3.f, 1.f,
        1.f, 0.f, 0.f, 0.f
};

Matrix BSplineCurve::basis_b = Matrix(Bernstein_BSpline) * (1.f / 6.f);

BSplineCurve::BSplineCurve(int num_vertices) {
    this->curve_type = "bspline";
    this->point_num = num_vertices;

    this->ctrl_point.clear();
    this->ctrl_point.resize(num_vertices);
}

void BSplineCurve::set(int i, const Vec3f& vec) {
    assert(i >= 0 && i < this->point_num);
    this->ctrl_point[i] = vec;
}

Matrix *BSplineCurve::getB() {
    return &basis_b;
}

void BSplineCurve::generateSections() {
    int section_num = (point_num - 1) / 3;
    sections.resize(static_cast<unsigned long>(section_num));
    for (int i = 0; i < section_num; ++i) {
        int index = 3 * i;
        generateSection(i, index);
    }
}

BezierCurve *BSplineCurve::bSplineToBezier() {
    generateSections();

    auto *bezier = new BezierCurve(this->point_num);

    Matrix B_bezier_i = *bezier->getB();
    B_bezier_i.Inverse();
    Matrix G_bezier = this->getSections()[0].G * *this->getB() * B_bezier_i;

    for (int i = 0; i < this->point_num; i++) {
        bezier->ctrl_point[i] = Vec3f(G_bezier.Get(0, i), G_bezier.Get(1, i), G_bezier.Get(2, i));
    }
}

void BSplineCurve::OutputBSpline(FILE *file) {
    output(file);
}

void BSplineCurve::OutputBezier(FILE *file) {
    bSplineToBezier()->output(file);
}
