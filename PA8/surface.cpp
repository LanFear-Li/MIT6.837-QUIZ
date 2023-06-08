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

}

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args) {
    return nullptr;
}