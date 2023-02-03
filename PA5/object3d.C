#include "object3d.h"

extern RayTracer ray_tracer;

Object3D::Object3D() = default;

Material* Object3D::getMaterial() const {
    return material_ptr;
}

BoundingBox* Object3D::getBoundingBox() const {
    return bbox_ptr;
}


Object3D::~Object3D() {
    delete material_ptr;
    delete bbox_ptr;
}


Grid::Grid(BoundingBox *bb, int nx, int ny, int nz) {
    this->bbox_ptr = bb;
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    object_type = GRID;

    minn = bb->getMin();
    maxn = bb->getMax();

    cell_state.resize(nx);
    for (int i = 0; i < nx; i++) {
        cell_state[i].resize(ny);
        for (int j = 0; j < ny; j++) {
            cell_state[i][j].resize(nz);
        }
    }

    step = (maxn - minn) * Vec3f(1.0f / nx, 1.0f / ny, 1.0f / nz);
}

bool Grid::intersect(const Ray &r, Hit &h, float t_min) {
    return false;
}

void Grid::insertIntoGrid(Grid *g, Matrix *m) {

}

void Grid::paint() {

}

Grid::~Grid() = default;


Sphere::Sphere(Vec3f c, float r, Material *m) {
    center = c;
    radius = r;
    material_ptr = m;

    object_type = SPHERE;

    // set bbox for sphere
    Vec3f minn = c - Vec3f(r, r, r);
    Vec3f maxn = c + Vec3f(r, r, r);
    bbox_ptr = new BoundingBox(minn, maxn);
    bbox_ptr->Print();
}

bool Sphere::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f cen = center;
    float rad = radius;
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
    if (discriminant < 0.0f) {
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

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    Vec3f minn = g->minn, maxn = g->maxn;
    Vec3f step = g->step;
    float grid_radius = step.Length() / 2.0f;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                Vec3f grid_center(i + 0.5f, j + 0.5f, k + 0.5f);
                grid_center = minn + grid_center * step;

                float distance = (grid_center - center).Length();
                if (distance < radius + grid_radius) {
                    g->cell_state[i][j][k] = true;
                }
            }
        }
    }
}

Vec3f Sphere::sphere_loc(float theta, float phi) const {
    // Y-axis is upward and Z-axis is flipped(negative is forward)
    float x = radius * sinf(phi) * cosf(theta);
    float y = radius * cosf(phi);
    float z = -radius * sinf(phi) * sinf(theta);

    return Vec3f(x, y, z) + center;
}

void Sphere::paint() {
    material_ptr->glSetMaterial();

    InputParser *input_parser = ray_tracer.input_parser;
    bool gouraud = input_parser->gouraud;
    float num_theta = input_parser->num_theta, num_phi = input_parser->num_phi;
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
                Vec3f normal = (a + b + c + d) * 0.25f - center;
                glNormal3f(normal.x(), normal.y(), normal.z());
                glVertex3f(a.x(), a.y(), a.z());
                glVertex3f(b.x(), b.y(), b.z());
                glVertex3f(c.x(), c.y(), c.z());
                glVertex3f(d.x(), d.y(), d.z());
            } else {
                glNormal3f(a.x() - center.x(), a.y() - center.y(), a.z() - center.z());
                glVertex3f(a.x(), a.y(), a.z());
                glNormal3f(b.x() - center.x(), b.y() - center.y(), b.z() - center.z());
                glVertex3f(b.x(), b.y(), b.z());
                glNormal3f(c.x() - center.x(), c.y() - center.y(), c.z() - center.z());
                glVertex3f(c.x(), c.y(), c.z());
                glNormal3f(d.x() - center.x(), d.y() - center.y(), d.z() - center.z());
                glVertex3f(d.x(), d.y(), d.z());
            }

            cur_theta = next_theta;
        }

        cur_phi = next_phi;
    }

    glEnd();
}


Plane::Plane(Vec3f &n, float d, Material *m) {
    normal = n;
    distance = d;
    material_ptr = m;

    object_type = PLANE;

    // plane has infinite bbox
    bbox_ptr = nullptr;
}

bool Plane::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // ray parallel with plane
    float epsilon = 1e-5;
    if (fabsf(dir.Dot3(normal)) < epsilon) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (distance - ori.Dot3(normal)) / (dir.Dot3(normal));

    if (t < t_min) {
        return false;
    }

    h.set(t, material_ptr, normal, r);
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

    float MAX_LIMIT = 1e6;
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

void Plane::insertIntoGrid(Grid *g, Matrix *m) {

}


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

    // get transformed bbox
    Vec3f p0(minn.x(), minn.y(), minn.z());
    Vec3f p1(maxn.x(), minn.y(), minn.z());
    Vec3f p2(minn.x(), maxn.y(), minn.z());
    Vec3f p3(maxn.x(), maxn.y(), minn.z());
    Vec3f p4(maxn.x(), minn.y(), maxn.z());
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
    bbox_ptr->Print();
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

}

Transform::~Transform() = default;

