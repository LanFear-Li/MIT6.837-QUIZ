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
    int grid_index[3];
    float t_next[3];
    float d_t[3];
    int sign[3];
    Vec3f cell_normal;

    bool hit_cell = false;
};

typedef vector<Object3D *> object_list;

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz);

    bool intersect(const Ray &r, Hit &h, float t_min) override;

    bool intersectObject(const Ray &r, Hit &h, float t_min);

    void paint() override;

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float t_min) const;

    void get_index(const Vec3f &point, int *index) const;

    bool validate_index(int *index) const;

    bool validate_point(const Vec3f &p) const;

    ~Grid();

    int nx, ny, nz;

    Vec3f step;

    Vec3f minn, maxn;

    vector<vector<vector<object_list>>> cell_bucket;

    int plane_index[6][4] = {
            {4, 5, 6, 7},
            {0, 1, 2, 3},
            {3, 0, 4, 7},
            {1, 2, 6, 5},
            {0, 1, 5, 4},
            {2, 3, 7, 6}
    };

    PhongMaterial material_type[13];

    object_list infinite_objects;

    float epsilon = 1e-5;
};

#endif