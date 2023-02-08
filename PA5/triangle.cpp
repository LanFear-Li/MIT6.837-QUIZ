#include "triangle.h"

#include "hit.h"
#include "material.h"
#include "plane.h"
#include "boundingbox.h"

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
    this->a = a;
    this->b = b;
    this->c = c;

    Vec3f n;
    Vec3f::Cross3(n, a - b, a - c);
    n.Normalize();
    normal = n;

    material_ptr = m;

    object_type = TRIANGLE;

    // set bbox for triangle
    bbox_ptr = new BoundingBox(a, a);
    bbox_ptr->Extend(b);
    bbox_ptr->Extend(c);
    bbox_ptr->Print();
    cout << "Triangle created" << endl;
}

bool Triangle::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    float dis = a.Dot3(normal);
    Hit hit;
    Plane plane(normal, dis, material_ptr);
    if (plane.intersect(r, hit, t_min) && hit.getT() >= t_min) {
        Vec3f p = hit.getIntersectionPoint();

        // use cross product to check whether inside the triangle
        Vec3f ab = b - a, bc = c - b, ca = a - c;
        Vec3f ap = p - a, bp = p - b, cp = p - c;

        Vec3f cross;
        Vec3f::Cross3(cross, ab, ap);
        float cross_a = normal.Dot3(cross);
        Vec3f::Cross3(cross, bc, bp);
        float cross_b = normal.Dot3(cross);
        Vec3f::Cross3(cross, ca, cp);
        float cross_c = normal.Dot3(cross);

        if (cross_a > 0 && cross_b > 0 && cross_c > 0 || cross_a <= 0 && cross_b <= 0 && cross_c <= 0) {
            h = hit;
            return true;
        }
    }

    return false;
}

void Triangle::paint() {
    material_ptr->glSetMaterial();

    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m) {

}