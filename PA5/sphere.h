#ifndef _PA_SPHERE_H_
#define _PA_SPHERE_H_

#include "object3d.h"

#include "hit.h"
#include "grid.h"
#include "boundingbox.h"

class Sphere : public Object3D {
public:
    Sphere(Vec3f c, float r, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f sphere_loc(float theta, float phi) const;

    Vec3f center;

    float radius;
};

#endif