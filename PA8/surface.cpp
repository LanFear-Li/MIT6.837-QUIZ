#include "surface.h"

SurfaceOfRevolution::SurfaceOfRevolution(Curve *c) : Surface(0) {
    this->curve = c;
}

void SurfaceOfRevolution::Paint(ArgParser *arg_parser) {
    this->curve->Paint(arg_parser);
}

int SurfaceOfRevolution::getNumVertices() {
    return this->curve->getNumVertices();
}

Vec3f SurfaceOfRevolution::getVertex(int i) {
    return this->curve->getVertex(i);
}

void SurfaceOfRevolution::moveControlPoint(int selectedPoint, float x, float y) {
    this->curve->moveControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::addControlPoint(int selectedPoint, float x, float y) {
    this->curve->addControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::deleteControlPoint(int selectedPoint) {
    this->curve->deleteControlPoint(selectedPoint);
}

void SurfaceOfRevolution::OutputBezier(FILE *file) {
    fprintf(file, "surface_of_revolution\n");
    this->curve->OutputBezier(file);
}

void SurfaceOfRevolution::OutputBSpline(FILE *file) {
    fprintf(file, "surface_of_revolution\n");
    this->curve->OutputBSpline(file);
}

TriangleMesh *SurfaceOfRevolution::OutputTriangles(ArgParser *args) {
    vector<Vec3f> points = this->curve->tessellatePoint(args);
    float angle = 2.0f * M_PI / args->revolution_tessellation;
    Matrix rotateMatrix = Matrix::MakeYRotation(angle);

    auto *mesh = new TriangleNet(points.size() - 1, args->revolution_tessellation);
    for (int i = 0; i <= args->revolution_tessellation; i++) {
        for (int j = 0; j < points.size(); j++) {
            mesh->SetVertex(j, i, points[j]);
            rotateMatrix.Transform(points[j]);
        }
    }

    return mesh;
}


void BezierPatch::Paint(ArgParser *arg_parser) {
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
}

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args) {
    vector<Vec3f> curve_points[4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            curve_points[i].push_back(this->ctrl_point[i * 4 + j]);
        }
    }

    BezierCurve curve[4] = {
            BezierCurve(curve_points[0]),
            BezierCurve(curve_points[1]),
            BezierCurve(curve_points[2]),
            BezierCurve(curve_points[3])
    };

    float t = 0, step_t = 1.0f / args->curve_tessellation;
    float s = 0, step_s = 1.0f / args->patch_tessellation;
    auto *mesh = new TriangleNet(args->patch_tessellation, args->curve_tessellation);

    vector<Vec3f> points;
    for (int i = 0; i <= args->curve_tessellation; i++) {
        points.clear();

        for (auto &c : curve) {
            c.generateSections();
            points.push_back((c.sections[0].calculatePoint(t)));
        }

        BezierCurve tempCurve(points);
        tempCurve.generateSections();
        s = 0;
        for (int j = 0; j <= args->patch_tessellation; j++) {
            mesh->SetVertex(j, i, tempCurve.sections[0].calculatePoint(s));
            s += step_s;
        }

        t += step_t;
    }

    return mesh;
}