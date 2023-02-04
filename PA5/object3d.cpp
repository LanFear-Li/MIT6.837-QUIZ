#include "object3d.h"

extern RayTracer ray_tracer;

Object3D::Object3D() = default;

Material* Object3D::getMaterial() const {
    return material_ptr;
}

BoundingBox* Object3D::getBoundingBox() const {
    return bbox_ptr;
}

void Object3D::insertIntoGrid(Grid *g, Matrix *m) {
    // do nothing
}

Object3D::~Object3D() {
    delete material_ptr;
    delete bbox_ptr;
}

