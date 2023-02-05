#ifndef _PA_TRIANGLE_H_
#define _PA_TRIANGLE_H_

#include <GL/gl.h>

#include "object3d.h"

class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f a, b, c;
    Vec3f normal;
};

#endif
