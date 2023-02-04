#ifndef _PA_GROUP_H_
#define _PA_GROUP_H_

#include <limits>
#include <GL/gl.h>

#include "object3d.h"

#include "hit.h"
#include "boundingbox.h"

class Group : public virtual Object3D {
public:
    Group(int num);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    void addObject(int index, Object3D *obj);

    ~Group();

    int num_objects;

    Object3D **object3D_ptr;
};

#endif
