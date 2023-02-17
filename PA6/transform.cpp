#include "transform.h"

#include "hit.h"
#include "boundingbox.h"
#include "triangle.h"

Transform::Transform(Matrix &m, Object3D *o) {
    mat = m;

    mat_inv = m;
    mat_inv.Inverse();

    mat_transpose = m;
    mat_transpose.Transpose();

    mat_trans_inv = mat_inv;
    mat_trans_inv.Transpose();

    object3d_ptr = o;

    // set bbox for transform
    Vec3f minn, maxn;
    o->bbox_ptr->Get(minn, maxn);

    // get transformed bbox, triangle should be handled separately
    if (object3d_ptr->object_type == TRIANGLE) {
        Triangle *triangle = (Triangle *) object3d_ptr;

        Vec3f p0 = triangle->a;
        Vec3f p1 = triangle->b;
        Vec3f p2 = triangle->c;

        mat.Transform(p0);
        mat.Transform(p1);
        mat.Transform(p2);

        bbox_ptr = new BoundingBox(p0, p0);
        bbox_ptr->Extend(p1);
        bbox_ptr->Extend(p2);
    } else {
        Vec3f p0(minn.x(), minn.y(), minn.z());
        Vec3f p1(maxn.x(), minn.y(), minn.z());
        Vec3f p2(minn.x(), maxn.y(), minn.z());
        Vec3f p3(maxn.x(), maxn.y(), minn.z());
        Vec3f p4(minn.x(), minn.y(), maxn.z());
        Vec3f p5(maxn.x(), minn.y(), maxn.z());
        Vec3f p6(minn.x(), maxn.y(), maxn.z());
        Vec3f p7(maxn.x(), maxn.y(), maxn.z());

        mat.Transform(p0);
        mat.Transform(p1);
        mat.Transform(p2);
        mat.Transform(p3);
        mat.Transform(p4);
        mat.Transform(p5);
        mat.Transform(p6);
        mat.Transform(p7);

        bbox_ptr = new BoundingBox(p0, p0);
        bbox_ptr->Extend(p1);
        bbox_ptr->Extend(p2);
        bbox_ptr->Extend(p3);
        bbox_ptr->Extend(p4);
        bbox_ptr->Extend(p5);
        bbox_ptr->Extend(p6);
        bbox_ptr->Extend(p7);
    }
}

bool Transform::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // transform the ray from world space to object space
    Vec4f ori_homo(ori, 1.0f);
    mat_inv.Transform(ori_homo);
    ori_homo.DivideByW();
    ori = Vec3f(ori_homo.x(), ori_homo.y(), ori_homo.z());

    Vec4f dir_homo(dir, 0.0f);
    mat_inv.Transform(dir_homo);
    dir = Vec3f(dir_homo.x(), dir_homo.y(), dir_homo.z());
    float dir_depth = dir.Length();
    dir.Normalize();

    Ray trans_ray(ori, dir);
    if (object3d_ptr->intersect(trans_ray, h, t_min)) {
        // transform the normal from object space to world space
        Vec3f normal = h.getNormal();
        Vec4f normal_homo(normal, 0.0f);
        mat_trans_inv.Transform(normal_homo);
        normal = Vec3f(normal_homo.x(), normal_homo.y(), normal_homo.z());
        normal.Normalize();

        // transform the depth from object space to world space
        float t = h.getT();
        t = t / dir_depth;

        h.set(t, h.getMaterial(), normal, trans_ray);
        return true;
    }

    return false;
}

void Transform::paint() {
    glPushMatrix();
    GLfloat *glMatrix = mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;

    object3d_ptr->paint();
    glPopMatrix();
}

void Transform::insertIntoGrid(Grid *g, Matrix *m) {
    cout << "Transform: insert into grid" << endl;
    Matrix transform = this->mat;
    if (m) {
        transform = *m * transform;
    }

    object3d_ptr->insertIntoGrid(g, &transform);
}

Transform::~Transform() = default;
