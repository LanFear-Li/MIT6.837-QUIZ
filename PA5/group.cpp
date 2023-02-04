#include "group.h"

extern RayTracer ray_tracer;

Group::Group(int num) {
    num_objects = num;
    object3D_ptr = new Object3D *[num];

    object_type = GROUP;

    float maxn = std::numeric_limits<float>::max();
    float minn = std::numeric_limits<float>::lowest();
    bbox_ptr = new BoundingBox(Vec3f(maxn, maxn, maxn), Vec3f(minn, minn, minn));
}

bool Group::intersect(const Ray &r, Hit &h, float t_min) {
    h = Hit(std::numeric_limits<float>::max(), nullptr, Vec3f());
    bool intersected = false;

    for (int i = 0; i < num_objects; i++) {
        Hit cur_hit;
        if (object3D_ptr[i]->intersect(r, cur_hit, t_min)) {
            if (cur_hit.getT() < h.getT()) {
                h = cur_hit;
            }

            intersected = true;
        }
    }

    return intersected;
}

void Group::addObject(int index, Object3D *obj) {
    assert(index >= 0 && index < num_objects);
    object3D_ptr[index] = obj;

    // set bbox for group
    if (object3D_ptr[index]->object_type != PLANE) {
        bbox_ptr->Extend(object3D_ptr[index]->getBoundingBox());
    }
}

Group::~Group() {
    /*for (int i = 0; i < num_objects; i++) {
        delete object3D_ptr[i];
    }

    delete object3D_ptr;*/
}

void Group::paint() {
    for (int i = 0; i < num_objects; i++) {
        cout << "Group painting: [" << i << "]" << endl;
        object3D_ptr[i]->paint();
    }
}

void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (int i = 0; i < num_objects; i++) {
        object3D_ptr[i]->insertIntoGrid(g, m);
    }
}

