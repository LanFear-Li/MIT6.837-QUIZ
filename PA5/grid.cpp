#include "grid.h"

#include <limits>

#include "boundingbox.h"
#include "plane.h"
#include "rayTree.h"

void MarchingInfo::nextCell() {
    // find smallest t_next
    float t_next_min = std::numeric_limits<float>::max();
    int min_index = -1;
    for (int i = 0; i < 3; i++) {
        if (t_next[i] >= t_min && t_next[i] < t_next_min) {
            t_next_min = t_next[i];
            min_index = i;
        }
    }

    // construct marching info for new cell
    t_min = t_next[min_index];
    grid_index[min_index] += sign[min_index];
    t_next[min_index] += d_t[min_index];

    Vec3f normals[3];
    normals[0] = {1.0f, 0.0f, 0.0f};
    normals[1] = {0.0f, 1.0f, 0.0f};
    normals[2] = {0.0f, 0.0f, 1.0f};
    cell_normal = normals[min_index] * sign[min_index];
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

    material_ptr = new PhongMaterial(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(), 0.0f, Vec3f(), Vec3f(), 1.0f);

    cout << "new grid: " << this->nx << " " << this->ny << " " << this->nz << endl;
    this->bbox_ptr->Print();
    cout << "step: " << step << endl;
}

bool Grid::intersect(const Ray &r, Hit &h, float t_min) {
    cout << "start grid intersect..." << endl;
    MarchingInfo info;
    initializeRayMarch(info, r, t_min);

    bool hit_opaque = false;
    float hit_t_value;
    Vec3f hit_normal;
    if (info.hit_cell) {
        cout << "info hit cell" << endl;
        while (validate_index(info.grid_index)) {
            int x = info.grid_index[0], y = info.grid_index[1], z = info.grid_index[2];

            if (cell_state[x][y][z]) {
                // this cell is opaque
                info.hit_cell = true;

                if (!hit_opaque) {
                    hit_t_value = info.t_min;
                    hit_normal = info.cell_normal;
                    hit_opaque = true;
                }
            }

            info.nextCell();
        }

        if (info.hit_cell) {
            hit_normal.Negate();
            h.set(hit_t_value, material_ptr, hit_normal, r);
            return true;
        }
    }

    return false;
}

void Grid::paint() {
    // literate all cells and paint quads if cell is occupied
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                if (cell_state[i][j][k]) {
                    Vec3f vertex[8];

                    vertex[0] = minn + step * Vec3f(i + 0, j + 0, k + 1);
                    vertex[1] = minn + step * Vec3f(i + 1, j + 0, k + 1);
                    vertex[2] = minn + step * Vec3f(i + 1, j + 0, k + 0);
                    vertex[3] = minn + step * Vec3f(i + 0, j + 0, k + 0);
                    vertex[4] = minn + step * Vec3f(i + 0, j + 1, k + 1);
                    vertex[5] = minn + step * Vec3f(i + 1, j + 1, k + 1);
                    vertex[6] = minn + step * Vec3f(i + 1, j + 1, k + 0);
                    vertex[7] = minn + step * Vec3f(i + 0, j + 1, k + 0);

                    material_ptr->glSetMaterial();
                    glBegin(GL_QUADS);
                    // sequence: up, down
                    glNormal3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(vertex[4].x(), vertex[4].y(), vertex[4].z());
                    glVertex3f(vertex[5].x(), vertex[5].y(), vertex[5].z());
                    glVertex3f(vertex[6].x(), vertex[6].y(), vertex[6].z());
                    glVertex3f(vertex[7].x(), vertex[7].y(), vertex[7].z());

                    glNormal3f(0.0f, -1.0f, 0.0f);
                    glVertex3f(vertex[0].x(), vertex[0].y(), vertex[0].z());
                    glVertex3f(vertex[1].x(), vertex[1].y(), vertex[1].z());
                    glVertex3f(vertex[2].x(), vertex[2].y(), vertex[2].z());
                    glVertex3f(vertex[3].x(), vertex[3].y(), vertex[3].z());

                    // sequence: left, right
                    glNormal3f(-1.0f, 0.0f, 0.0f);
                    glVertex3f(vertex[3].x(), vertex[3].y(), vertex[3].z());
                    glVertex3f(vertex[0].x(), vertex[0].y(), vertex[0].z());
                    glVertex3f(vertex[4].x(), vertex[4].y(), vertex[4].z());
                    glVertex3f(vertex[7].x(), vertex[7].y(), vertex[7].z());

                    glNormal3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(vertex[1].x(), vertex[1].y(), vertex[1].z());
                    glVertex3f(vertex[2].x(), vertex[2].y(), vertex[2].z());
                    glVertex3f(vertex[6].x(), vertex[6].y(), vertex[6].z());
                    glVertex3f(vertex[5].x(), vertex[5].y(), vertex[5].z());

                    // sequence: front, back
                    glNormal3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(vertex[0].x(), vertex[0].y(), vertex[0].z());
                    glVertex3f(vertex[1].x(), vertex[1].y(), vertex[1].z());
                    glVertex3f(vertex[5].x(), vertex[5].y(), vertex[5].z());
                    glVertex3f(vertex[4].x(), vertex[4].y(), vertex[4].z());

                    glNormal3f(0.0f, 0.0f, -1.0f);
                    glVertex3f(vertex[2].x(), vertex[2].y(), vertex[2].z());
                    glVertex3f(vertex[3].x(), vertex[3].y(), vertex[3].z());
                    glVertex3f(vertex[7].x(), vertex[7].y(), vertex[7].z());
                    glVertex3f(vertex[6].x(), vertex[6].y(), vertex[6].z());
                    glEnd();
                }
            }
        }
    }
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float t_min) const {
    cout << "start initializeRayMarch..." << endl;
    for (int i = 0; i < 3; i++) {
        mi.sign[i] = r.getDirection()[i] > 0.0f ? 1 : -1;
        mi.d_t[i] = step[i] / r.getDirection()[i] * mi.sign[i];
    }

    // generate grid vertices
    Vec3f vertex[8];
    vertex[0] = Vec3f(minn.x(), minn.y(), maxn.z());
    vertex[1] = Vec3f(maxn.x(), minn.y(), maxn.z());
    vertex[2] = Vec3f(maxn.x(), minn.y(), minn.z());
    vertex[3] = Vec3f(minn.x(), minn.y(), minn.z());
    vertex[4] = Vec3f(minn.x(), maxn.y(), maxn.z());
    vertex[5] = Vec3f(maxn.x(), maxn.y(), maxn.z());
    vertex[6] = Vec3f(maxn.x(), maxn.y(), minn.z());
    vertex[7] = Vec3f(minn.x(), maxn.y(), minn.z());

    // construct intersect planes and its normals
    // sequence: up, down, left, right, front, back
    Plane plane[6];
    plane[0] = Plane(vertex[4], vertex[5], vertex[6], material_ptr);
    plane[1] = Plane(vertex[0], vertex[1], vertex[2], material_ptr);
    plane[2] = Plane(vertex[3], vertex[0], vertex[4], material_ptr);
    plane[3] = Plane(vertex[1], vertex[2], vertex[6], material_ptr);
    plane[4] = Plane(vertex[0], vertex[1], vertex[5], material_ptr);
    plane[5] = Plane(vertex[2], vertex[3], vertex[7], material_ptr);

    Vec3f normals[6];
    normals[0] = Vec3f(0.0f, 1.0f, 0.0f);
    normals[1] = Vec3f(0.0f, -1.0f, 0.0f);
    normals[2] = Vec3f(-1.0f, 0.0f, 0.0f);
    normals[3] = Vec3f(1.0f, 0.0f, 0.0f);
    normals[4] = Vec3f(0.0f, 0.0f, 1.0f);
    normals[5] = Vec3f(0.0f, 0.0f, -1.0f);

    Vec3f point_hit;
    Vec3f point_start = r.pointAtParameter(t_min);
    int enter_face_index;
    // check if point_start point is inside the grid
    if (validate_point(point_start)) {
        mi.hit_cell = true;
        point_hit = point_start;
        cout << "inside bbox" << endl;
    } else {
        // if not, find the nearest point on the grid
        // find closest ray-plane intersection
        Hit hit_closest;
        for (int i = 0; i < 6; i++) {
            Hit hit;
            if (plane[i].intersect(r, hit, t_min)) {
                Vec3f p = hit.getIntersectionPoint();
                if (!validate_point(p)) {
                    continue;
                }

                if (hit_closest.getMaterial() == nullptr || hit.getT() < hit_closest.getT()) {
                    hit_closest = hit;
                    mi.cell_normal = normals[i];
                    mi.hit_cell = true;

                    enter_face_index = i;
                }
            }
        }

        mi.t_min = hit_closest.getT();
        point_hit = hit_closest.getIntersectionPoint();
        cout << "outside bbox" << endl;
    }

    // construct the marching info: next grid index and the t-value to it
    get_index(point_hit, mi.grid_index);
    for (int i = 0; i < 3; i++) {
        float index_next = mi.grid_index[i] + mi.sign[i];
        mi.t_next[i] = (minn[i] + step[i] * index_next - r.getOrigin()[i]) / r.getDirection()[i];
    }

    // open ray tree debugging mode
    if (mi.hit_cell) {
        for (int i = 0; i < 6; i++) {
            RayTree::AddHitCellFace(vertex[plane_index[i][0]], vertex[plane_index[i][1]], vertex[plane_index[i][2]],
                                    vertex[plane_index[i][3]], normals[i], material_ptr);

            int x = enter_face_index;
            RayTree::AddEnteredFace(vertex[plane_index[x][0]], vertex[plane_index[x][1]], vertex[plane_index[x][2]],
                                    vertex[plane_index[x][3]], normals[i], material_ptr);
        }
    }

    cout << "hit cell: " << mi.hit_cell << endl;
}

void Grid::get_index(const Vec3f &point, int *index) const {
    for (int i = 0; i < 3; i++) {
        index[i] = ceil((point[i] - minn[i]) / step[i]);
    }
}

bool Grid::validate_index(int *index) const {
    if (index[0] < 0 || index[0] >= nx) return false;
    if (index[1] < 0 || index[1] >= ny) return false;
    if (index[2] < 0 || index[2] >= nz) return false;

    return true;
}

bool Grid::validate_point(const Vec3f &p) const {
    if (p.x() < minn.x() || p.x() > maxn.x()) return false;
    if (p.y() < minn.y() || p.y() > maxn.y()) return false;
    if (p.z() < minn.z() || p.z() > maxn.z()) return false;

    return true;
}

Grid::~Grid() {
    // delete material_ptr;
}