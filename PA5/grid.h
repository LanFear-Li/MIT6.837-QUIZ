#ifndef _PA_GRID_H_
#define _PA_GRID_H_

#include <vector>
#include <GL/gl.h>

#include "matrix.h"

#include "object3d.h"
#include "material.h"

class MarchingInfo {
public:
    void nextCell();

    float t_min;
    Vec3f grid_index;
    Vec3f t_next;
    Vec3f d_t;
    Vec3f sign;
    Vec3f cell_normal;
};

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float t_min) const;

    ~Grid();

    int nx, ny, nz;

    Vec3f step;

    Vec3f minn, maxn;

    vector<vector<vector<bool>>> cell_state;

    PhongMaterial grid_material;
};

#endif