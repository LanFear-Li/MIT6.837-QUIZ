#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <limits>
#include <GL/gl.h>
#include <vector>

#include "ray.h"
#include "hit.h"
#include "matrix.h"
#include "material.h"
#include "boundingbox.h"
#include "input_parser.h"

enum ObjectType {
    GRID, SPHERE, PLANE, TRIANGLE, GROUP
};

class Object3D {
public:
    Object3D();

    virtual bool intersect(const Ray &r, Hit &h, float t_min) = 0;

    virtual void paint() = 0;

    virtual void insertIntoGrid(Grid *g, Matrix *m) = 0;

    Material *getMaterial() const;

    BoundingBox *getBoundingBox() const;

    ~Object3D();

    Material *material_ptr{};

    BoundingBox *bbox_ptr{};

    ObjectType object_type{};
};

class Grid : public virtual Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    ~Grid();

    int nx, ny, nz;

    Vec3f step;

    Vec3f minn, maxn;

    vector<vector<vector<bool>>> cell_state;
};

class Sphere : public virtual Object3D {
public:
    Sphere(Vec3f c, float r, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f sphere_loc(float theta, float phi) const;

    Vec3f center;

    float radius;
};

class Plane : public virtual Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f normal;

    // all point P on plane satisfy: P * n = d
    float distance;
};

class Triangle : public virtual Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    Vec3f a, b, c;
    Vec3f normal;
};

class Group : public virtual Object3D {
public:
    explicit Group(int num);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    void addObject(int index, Object3D *obj);

    ~Group();

    int num_objects;

    Object3D **object3D_ptr;
};

class Transform : public virtual Object3D {
public:
    Transform(Matrix &m, Object3D *o);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    ~Transform();

    Matrix mat;
    Matrix mat_inv;
    Matrix mat_transpose;
    Matrix mat_trans_inv;

    Object3D *object3d_ptr;
};

#endif