#ifndef _PA_OBJECT3D_H_
#define _PA_OBJECT3D_H_

#include <GL/gl.h>

#include "matrix.h"

class RayTracer;

class Ray;
class Hit;
class Material;
class Grid;
class BoundingBox;

enum ObjectType {
    GRID, SPHERE, PLANE, TRIANGLE, GROUP
};

class Object3D {
public:
    Object3D();

    virtual bool intersect(const Ray &r, Hit &h, float t_min) = 0;

    virtual void paint() = 0;

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    Material *getMaterial() const;

    BoundingBox *getBoundingBox() const;

    ~Object3D();

    Material *material_ptr;

    BoundingBox *bbox_ptr;

    ObjectType object_type;
};

#endif