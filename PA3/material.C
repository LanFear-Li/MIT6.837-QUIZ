#include "material.h"

Material::Material() = default;

Material::Material(const Vec3f &d_color) {
    this->diffuseColor = d_color;
}

Vec3f Material::getDiffuseColor() const {
    return this->diffuseColor;
}

Material::~Material() = default;


PhongMaterial::PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent)
        : Material(diffuseColor) {
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->exponent = exponent;
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f normal = hit.getNormal();

    // implement phong shading model: ambient + diffuse + specular
    Vec3f diffuse_color = this->diffuseColor * fmax(dirToLight.Dot3(normal), 0);

    Vec3f reflect_dir = 2 * (normal.Dot3(dirToLight)) * normal - dirToLight;
    reflect_dir.Normalize();
    Vec3f view_dir = ray.getOrigin() - hit.getIntersectionPoint();
    view_dir.Normalize();
    Vec3f specular_color = this->specularColor * pow(reflect_dir.Dot3(view_dir), this->exponent);

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

Vec3f PhongMaterial::getSpecularColor() const {
    return this->specularColor;
}

PhongMaterial::~PhongMaterial() = default;