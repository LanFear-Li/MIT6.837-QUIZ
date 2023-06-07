#include "spline.h"

void Spline::moveControlPoint(int selectedPoint, float x, float y) {
    ctrl_point[selectedPoint] = Vec3f(x, y, 0);
}

void Spline::addControlPoint(int selectedPoint, float x, float y) {
    ctrl_point.insert(ctrl_point.begin() + selectedPoint, Vec3f(x, y, 0));
}

void Spline::deleteControlPoint(int selectedPoint) {
    ctrl_point.erase(ctrl_point.begin() + selectedPoint);
}