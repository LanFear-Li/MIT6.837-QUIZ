#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "ray.h"
#include "hit.h"

class Object3D {
public:
    Object3D();

    virtual bool intersect(const Ray &r, Hit &h, float t_min) = 0;

    ~Object3D();

    Material *material_ptr{};
};

class Sphere : public virtual Object3D {
public:
    Sphere(Vec3f c, float r, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    ~Sphere();

    Vec3f center;

    float radius;
};

class Plane : public virtual Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    ~Plane();
};

class Triangle : public virtual Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    ~Triangle();
};

class Group : public virtual Object3D {
public:
    explicit Group(int num);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void addObject(int index, Object3D *obj);

    ~Group();

    int num_objects;

    Object3D **object3D_ptr;
};

class Transform : public virtual Object3D {
public:
    Transform(Matrix &m, Object3D *o);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    ~Transform();
};

#endif