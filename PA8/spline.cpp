#include "spline.h"

Spline::Spline(int point_num) {
    this->point_num = point_num;

    this->ctrl_point.clear();
    this->ctrl_point.resize(point_num);
}

void Spline::set(int i, const Vec3f &p) {
    this->ctrl_point[i] = p;
}

int Spline::getNumVertices() {
    return this->point_num;
}

Vec3f Spline::getVertex(int i) {
    return this->ctrl_point[i];
}

void Spline::moveControlPoint(int selectedPoint, float x, float y) {
    this->ctrl_point[selectedPoint] = Vec3f(x, y, 0);
}

void Spline::addControlPoint(int selectedPoint, float x, float y) {
    this->ctrl_point.insert(ctrl_point.begin() + selectedPoint, Vec3f(x, y, 0));
    this->point_num++;
}

void Spline::deleteControlPoint(int selectedPoint) {
    this->ctrl_point.erase(ctrl_point.begin() + selectedPoint);
    this->point_num--;
}