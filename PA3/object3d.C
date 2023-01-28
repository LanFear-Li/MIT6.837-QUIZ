#include "object3d.h"

extern InputParser input_parser;

Object3D::Object3D() = default;

Object3D::~Object3D() = default;


Sphere::Sphere(Vec3f c, float r, Material *m) {
    this->center = c;
    this->radius = r;
    this->material_ptr = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f cen = this->center;
    float rad = this->radius;
    Vec3f ori = r.getOrigin(), dir = r.getDirection();
    Vec3f start = ori - cen;

    // calculate ray-sphere intersection with equation:
    // (start + t * dir) * (start + t * dir) - rad * rad = 0
    // (dir * dir) * t^2 + (2 * start * dir) * t + (start * start - rad * rad) = 0
    float a = 1;
    float half_b = start.Dot3(dir);
    float c = start.Dot3(start) - rad * rad;

    // discriminant = 4(half_b * half_b - a * c)
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }

    // get equation root
    float sqrt_dis = sqrt(discriminant);
    float t = (-half_b - sqrt_dis) / a;
    if (t < t_min) {
        t = (-half_b + sqrt_dis) / a;
        if (t < t_min) {
            return false;
        }
    }

    Vec3f p = ori + dir * t;
    Vec3f normal = p - cen;
    normal.Normalize();
    h.set(t, material_ptr, normal, r);
    return true;
}

Vec3f Sphere::sphere_loc(float theta, float phi) const {
    // Y-axis is upward and Z-axis is flipped(negative is forward)
    float x = this->radius * sinf(phi) * cosf(theta);
    float y = this->radius * cosf(phi);
    float z = -this->radius * sinf(phi) * sinf(theta);

    return Vec3f(x, y, z) + this->center;
}

void Sphere::paint() {
    this->material_ptr->glSetMaterial();

    bool gouraud = input_parser.gouraud;
    float num_theta = input_parser.num_theta, num_phi = input_parser.num_phi;
    float step_theta = M_PI * 2.0f / num_theta;
    float step_phi = M_PI * 1.0f / num_phi;

    glBegin(GL_QUADS);
    float cur_theta = 0.0f, cur_phi = 0.0f;
    for (int iPhi = 0; iPhi <= num_phi; iPhi++) {
        float next_phi = cur_phi + step_phi;

        for (int iTheta = 0; iTheta <= num_theta; iTheta++) {
            float next_theta = cur_theta + step_theta;

            Vec3f a = sphere_loc(cur_theta, cur_phi);
            Vec3f b = sphere_loc(cur_theta, next_phi);
            Vec3f c = sphere_loc(next_theta, next_phi);
            Vec3f d = sphere_loc(next_theta, cur_phi);

            if (!gouraud) {
                Vec3f normal = (a + b + c + d) * 0.25f - this->center;
                glNormal3f(normal.x(), normal.y(), normal.z());
                glVertex3f(a.x(), a.y(), a.z());
                glVertex3f(b.x(), b.y(), b.z());
                glVertex3f(c.x(), c.y(), c.z());
                glVertex3f(d.x(), d.y(), d.z());
            } else {
                glNormal3f(a.x() - this->center.x(), a.y() - this->center.y(), a.z() - this->center.z());
                glVertex3f(a.x(), a.y(), a.z());
                glNormal3f(b.x() - this->center.x(), b.y() - this->center.y(), b.z() - this->center.z());
                glVertex3f(b.x(), b.y(), b.z());
                glNormal3f(c.x() - this->center.x(), c.y() - this->center.y(), c.z() - this->center.z());
                glVertex3f(c.x(), c.y(), c.z());
                glNormal3f(d.x() - this->center.x(), d.y() - this->center.y(), d.z() - this->center.z());
                glVertex3f(d.x(), d.y(), d.z());
            }

            cur_theta = next_theta;
        }

        cur_phi = next_phi;
    }

    glEnd();
}

Sphere::~Sphere() = default;


Group::Group(int num) {
    this->num_objects = num;
    this->object3D_ptr = new Object3D *[num];
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
}

Group::~Group() {
    /*for (int i = 0; i < num_objects; i++) {
        delete object3D_ptr[i];
    }

    delete object3D_ptr;*/
}

void Group::paint() {
    for (int i = 0; i < num_objects; i++) {
        object3D_ptr[i]->paint();
    }
}


Plane::Plane(Vec3f &normal, float d, Material *m) {
    this->normal = normal;
    this->distance = d;
    this->material_ptr = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // ray parallel with plane
    if (dir.Dot3(this->normal) == 0) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (this->distance - ori.Dot3(this->normal)) / (dir.Dot3(this->normal));

    if (t < t_min) {
        return false;
    }

    h.set(t, material_ptr, this->normal, r);
    return true;
}

void Plane::paint() {
    // calculate plane basis vector
    Vec3f v(1, 0, 0);
    float epsilon = 1e-5;
    if (fabs(this->normal.y()) < epsilon && fabs(this->normal.z()) < epsilon) {
        v.Set(0, 1, 0);
    }

    Vec3f basis_x, basis_y;
    Vec3f::Cross3(basis_x, v, this->normal);
    Vec3f::Cross3(basis_y, this->normal, basis_x);

    float MAX_LIMIT = 1e4;
    Vec3f a = basis_x * MAX_LIMIT + this->normal * distance;
    Vec3f b = basis_y * MAX_LIMIT + this->normal * distance;
    Vec3f c = basis_x * -MAX_LIMIT + this->normal * distance;
    Vec3f d = basis_y * -MAX_LIMIT + this->normal * distance;

    this->material_ptr->glSetMaterial();
    glBegin(GL_QUADS);
    glNormal3f(this->normal.x(), this->normal.y(), this->normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glVertex3f(d.x(), d.y(), d.z());
    glEnd();
}

Plane::~Plane() = default;


Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
    this->a = a;
    this->b = b;
    this->c = c;

    Vec3f n;
    Vec3f::Cross3(n, this->a - this->b, this->a - this->c);
    n.Normalize();
    this->normal = n;

    this->material_ptr = m;
}

bool Triangle::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    float dis = this->a.Dot3(normal);

    // ray parallel with plane
    if (dir.Dot3(normal) == 0) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (dis - ori.Dot3(normal)) / (dir.Dot3(normal));

    if (t < t_min) {
        return false;
    }

    // get intersect point with the plane formed by triangle
    Vec3f p = ori + dir * t;

    // use cross product to check whether inside the triangle
    Vec3f ab = this->b - this->a, bc = this->c - this->b, ca = this->a - this->c;
    Vec3f ap = p - this->a, bp = p - this->b, cp = p - this->c;

    Vec3f cross;
    Vec3f::Cross3(cross, ab, ap);
    float cross_a = normal.Dot3(cross);
    Vec3f::Cross3(cross, bc, bp);
    float cross_b = normal.Dot3(cross);
    Vec3f::Cross3(cross, ca, cp);
    float cross_c = normal.Dot3(cross);

    // TODO: ray-triangle intersection with barycentric coordinate
    if (cross_a > 0 && cross_b > 0 && cross_c > 0 || cross_a <= 0 && cross_b <= 0 && cross_c <= 0) {
        h.set(t, material_ptr, normal, r);
        return true;
    }

    return false;
}

void Triangle::paint() {
    this->material_ptr->glSetMaterial();

    glBegin(GL_TRIANGLES);
    glNormal3f(this->normal.x(), this->normal.y(), this->normal.z());
    glVertex3f(this->a.x(), this->a.y(), this->a.z());
    glVertex3f(this->b.x(), this->b.y(), this->b.z());
    glVertex3f(this->c.x(), this->c.y(), this->c.z());
    glEnd();
}

Triangle::~Triangle() = default;


Transform::Transform(Matrix &m, Object3D *o) {
    this->mat = m;

    this->mat_inv = m;
    this->mat_inv.Inverse();

    this->mat_transpose = m;
    this->mat_transpose.Transpose();

    this->mat_trans_inv = this->mat_inv;
    this->mat_trans_inv.Transpose();

    this->object3d_ptr = o;
}

bool Transform::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // transform the ray from world space to object space
    Vec4f ori_homo(ori, 1.0f);
    this->mat_inv.Transform(ori_homo);
    ori_homo.DivideByW();
    ori = Vec3f(ori_homo.x(), ori_homo.y(), ori_homo.z());

    Vec4f dir_homo(dir, 0.0f);
    this->mat_inv.Transform(dir_homo);
    dir = Vec3f(dir_homo.x(), dir_homo.y(), dir_homo.z());
    float dir_depth = dir.Length();
    dir.Normalize();

    Ray trans_ray(ori, dir);
    if (object3d_ptr->intersect(trans_ray, h, t_min)) {
        // transform the normal from object space to world space
        Vec3f normal = h.getNormal();
        Vec4f normal_homo(normal, 0.0f);
        this->mat_trans_inv.Transform(normal_homo);
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
    GLfloat *glMatrix = this->mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;

    this->object3d_ptr->paint();
    glPopMatrix();
}

Transform::~Transform() = default;
