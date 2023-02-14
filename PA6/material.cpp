#include "material.h"
#include "glCanvas.h"

Material::Material() = default;

Material::Material(const Vec3f &d_color) {
    diffuseColor = d_color;
}

Material::~Material() = default;


PhongMaterial::PhongMaterial() = default;

PhongMaterial::PhongMaterial(const Vec3f &diffuse, const Vec3f &specular, float e, const Vec3f &reflective,
                             const Vec3f &transparent, float refraction) : Material(diffuse) {
    diffuseColor = diffuse;
    specularColor = specular;
    exponent = e;
    reflectiveColor = reflective;
    transparentColor = transparent;
    indexOfRefraction = refraction;
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f normal = hit.getNormal();

    // implement phong shading model: ambient + diffuse + specular
    Vec3f diffuse_color = getDiffuseColor() * fmax(normal.Dot3(dirToLight), 0);

    Vec3f v = ray.getDirection();
    v.Negate();
    Vec3f half_vec = v + dirToLight;
    half_vec.Normalize();
    Vec3f specular_color = getSpecularColor() * powf(normal.Dot3(half_vec), exponent);
    return (diffuse_color + specular_color) * lightColor;
}

void PhongMaterial::glSetMaterial() const {
    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
            getSpecularColor().r(),
            getSpecularColor().g(),
            getSpecularColor().b(),
            1.0};
    GLfloat diffuse[4] = {
            getDiffuseColor().r(),
            getDiffuseColor().g(),
            getDiffuseColor().b(),
            1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0) glexponent = 0;
    if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}

Vec3f Material::getDiffuseColor() const {
    return diffuseColor;
}

Vec3f PhongMaterial::getSpecularColor() const {
    return specularColor;
}

Vec3f PhongMaterial::getReflectiveColor() const {
    return reflectiveColor;
}

Vec3f PhongMaterial::getTransparentColor() const {
    return transparentColor;
}

float PhongMaterial::getIndexOfRefraction() const {
    return indexOfRefraction;
}

PhongMaterial::~PhongMaterial() = default;


CheckerBoard::CheckerBoard() {

}

CheckerBoard::CheckerBoard(Matrix *m, Material *mat1, Material *mat2) {
    this->world_to_texture = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
}

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    return Vec3f();
}

void CheckerBoard::glSetMaterial() const {

}

Vec3f CheckerBoard::getSpecularColor() const {
    return Vec3f();
}

Vec3f CheckerBoard::getReflectiveColor() const {
    return Vec3f();
}

Vec3f CheckerBoard::getTransparentColor() const {
    return Vec3f();
}

float CheckerBoard::getIndexOfRefraction() const {
    return 0;
}

CheckerBoard::~CheckerBoard() {

}


Noise::Noise() {

}

Noise::Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) {
    this->world_to_texture = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
    this->octaves = octaves;
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    return Vec3f();
}

void Noise::glSetMaterial() const {

}

Vec3f Noise::getSpecularColor() const {
    return Vec3f();
}

Vec3f Noise::getReflectiveColor() const {
    return Vec3f();
}

Vec3f Noise::getTransparentColor() const {
    return Vec3f();
}

float Noise::getIndexOfRefraction() const {
    return 0;
}

Noise::~Noise() {

}


Marble::Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) {

}


Wood::Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) {

}
