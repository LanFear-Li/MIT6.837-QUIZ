#ifndef _PA_TRANSFORM_H_
#define _PA_TRANSFORM_H_

#include "object3d.h"

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
