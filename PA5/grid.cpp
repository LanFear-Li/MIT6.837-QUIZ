#include "grid.h"

#include "boundingbox.h"

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

    grid_material = PhongMaterial(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(), 0.0f, Vec3f(), Vec3f(), 1.0f);
}

bool Grid::intersect(const Ray &r, Hit &h, float t_min) {
    return false;
}

void Grid::insertIntoGrid(Grid *g, Matrix *m) {

}

void Grid::paint() {
    // literate all cells and paint quads if cell is occupied
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                if (cell_state[i][j][k]) {
                    Vec3f vertex[8];

                    vertex[0] = minn + step * Vec3f(i + 0, j + 0, k + 0);
                    vertex[1] = minn + step * Vec3f(i + 1, j + 0, k + 0);
                    vertex[2] = minn + step * Vec3f(i + 1, j + 0, k - 1);
                    vertex[3] = minn + step * Vec3f(i + 0, j + 0, k - 1);
                    vertex[4] = minn + step * Vec3f(i + 0, j + 1, k + 0);
                    vertex[5] = minn + step * Vec3f(i + 1, j + 1, k + 0);
                    vertex[6] = minn + step * Vec3f(i + 1, j + 1, k - 1);
                    vertex[7] = minn + step * Vec3f(i + 0, j + 1, k - 1);

                    grid_material.glSetMaterial();
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
                    glVertex3f(vertex[4].x(), vertex[4].y(), vertex[4].z());
                    glVertex3f(vertex[5].x(), vertex[5].y(), vertex[5].z());

                    glNormal3f(0.0f, 0.0f, -1.0f);
                    glVertex3f(vertex[2].x(), vertex[2].y(), vertex[2].z());
                    glVertex3f(vertex[3].x(), vertex[3].y(), vertex[3].z());
                    glVertex3f(vertex[6].x(), vertex[6].y(), vertex[6].z());
                    glVertex3f(vertex[7].x(), vertex[7].y(), vertex[7].z());
                    glEnd();
                }
            }
        }
    }
}

Grid::~Grid() = default;