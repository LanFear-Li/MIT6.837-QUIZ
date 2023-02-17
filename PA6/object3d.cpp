#include "object3d.h"

#include "grid.h"
#include "transform.h"
#include "boundingbox.h"

Object3D::Object3D() = default;

Material* Object3D::getMaterial() const {
    return material_ptr;
}

BoundingBox* Object3D::getBoundingBox() const {
    return bbox_ptr;
}

void Object3D::insertIntoGrid(Grid *g, Matrix *m) {
    // do nothing
    cout << "what happen?" << endl;

    Transform *transform = new Transform(*m, this);

    int min_idx[3], max_idx[3];
    BoundingBox *trans_bbox = transform->bbox_ptr;
    g->get_index(trans_bbox->getMin(), min_idx);
    g->get_index(trans_bbox->getMax(), max_idx);
    for (int i = min_idx[0]; i <= max_idx[0]; i++) {
        for (int j = min_idx[1]; j <= max_idx[1]; j++) {
            for (int k = min_idx[2]; k <= max_idx[2]; k++) {
                g->cell_bucket[i][j][k].push_back(transform);
            }
        }
    }

}

Object3D::~Object3D() {
    delete bbox_ptr;
}

