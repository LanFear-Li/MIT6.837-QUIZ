#include "camera.h"

Camera::Camera() = default;

Camera::~Camera() = default;


OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) {
    Vec3f h;

    origin_up = u;
    origin_up.Normalize();

    d.Normalize();
    Vec3f::Cross3(h, d, u);
    h.Normalize();
    Vec3f::Cross3(u, h, d);
    u.Normalize();

    center = c;
    direction = d;
    up = u;
    horizontal = h;
    size = s;
}

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================

void OrthographicCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h) {
        glOrtho(-size / 2.0, size / 2.0, -size * (float) h / (float) w / 2.0, size * (float) h / (float) w / 2.0, 0.5,
                40.0);
    } else {
        glOrtho(-size * (float) w / (float) h / 2.0, size * (float) w / (float) h / 2.0, -size / 2.0, size / 2.0, 0.5,
                40.0);
    }
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void) {
    gluLookAt(center.x(), center.y(), center.z(),
              center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
              up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at the end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist) {
    center += direction * dist;
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy) {
    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal * dx + screenUp * dy;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry) {
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(origin_up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(origin_up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);

    Vec3f::Cross3(horizontal, direction, origin_up);
    horizontal.Normalize();
    Vec3f::Cross3(up, horizontal, direction);
    up.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5f, 0.5f);
    Vec3f origin = center;

    origin += horizontal * size * point.x();
    origin += up * size * point.y();

    return Ray(origin, direction);
}

float OrthographicCamera::getTMin() const {
    return -100.0f;
}

OrthographicCamera::~OrthographicCamera() = default;


PerspectiveCamera::PerspectiveCamera(Vec3f &c, Vec3f &d, Vec3f &u, float fov_angle) {
    Vec3f h;

    origin_up = u;
    origin_up.Normalize();

    d.Normalize();
    Vec3f::Cross3(h, d, u);
    h.Normalize();
    Vec3f::Cross3(u, h, d);
    u.Normalize();

    center = c;
    direction = d;
    up = u;
    horizontal = h;

    fov = fov_angle;
    near = 2 * tan(fov / 2);
}

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov * 180.0 / 3.14159, (float) w / float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void) {
    gluLookAt(center.x(), center.y(), center.z(),
              center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
              up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist) {
    center += direction * dist;
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy) {
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal * dx + screenUp * dy;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry) {
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(origin_up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(origin_up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    direction.Normalize();

    Vec3f::Cross3(horizontal, direction, origin_up);
    horizontal.Normalize();
    Vec3f::Cross3(up, horizontal, direction);
    up.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5f, 0.5f);
    Vec3f screen_loc = center + direction;
    screen_loc += (point.x() * horizontal + point.y() * up) * near;

    Vec3f origin = center;
    Vec3f ray_dir = screen_loc - center;
    ray_dir.Normalize();
    return Ray(origin, ray_dir);
}

float PerspectiveCamera::getTMin() const {
    return 0.0f;
}

PerspectiveCamera::~PerspectiveCamera() = default;