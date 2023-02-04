#ifndef _PA_PLANE_H_
#define _PA_PLANE_H_

#include <GL/gl.h>

#include "object3d.h"
#include "material.h"

#include "hit.h"
#include "boundingbox.h"

class Plane : public Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f normal;

    // all point P on plane satisfy: P * n = d
    float distance;
};

#endif