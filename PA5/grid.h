#ifndef _PA_GRID_H_
#define _PA_GRID_H_

#include <vector>
#include <GL/gl.h>

#include "matrix.h"

#include "object3d.h"
#include "material.h"

class RayTracer;

class Grid : public Object3D {
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

    PhongMaterial grid_material;
};

#endif