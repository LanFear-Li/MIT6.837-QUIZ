#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "ray.h"
#include "hit.h"

class Object3D {
public:
    Object3D();

    virtual bool intersect(const Ray &r, Hit &h, float min) = 0;

    ~Object3D();

    Material *material_ptr{};
};

class Sphere : public virtual Object3D {
public:
    Sphere(Vec3f c, float r, Material *m);

    bool intersect(const Ray &r, Hit &h, float min) override;

    ~Sphere();

    Vec3f center;

    float radius;
};

class Group : public virtual Object3D {
public:
    explicit Group(int num);

    bool intersect(const Ray &r, Hit &h, float min) override;

    void addObject(int index, Object3D *obj);

    ~Group();

    int num_objects;

    Object3D *object3D_ptr[1024]{};
};

#endif