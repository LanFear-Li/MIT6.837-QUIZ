#include "sphere.h"

#include "hit.h"
#include "grid.h"
#include "boundingbox.h"
#include "raytracing_stats.h"

bool gouraud = false;
int num_theta = 10, num_phi = 10;

Sphere::Sphere(Vec3f c, float r, Material *m) {
    center = c;
    radius = r;
    material_ptr = m;

    object_type = SPHERE;

    // set bbox for sphere
    Vec3f minn = c - Vec3f(r, r, r);
    Vec3f maxn = c + Vec3f(r, r, r);
    bbox_ptr = new BoundingBox(minn, maxn);
}

bool Sphere::intersect(const Ray &r, Hit &h, float t_min) {
    // Stats: Ray-Primitive intersection operation
    RayTracingStats::IncrementNumIntersections();

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
    if (m) {
        Object3D::insertIntoGrid(g, m);
        return;
    }

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
                    g->cell_bucket[i][j][k].push_back(this);
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