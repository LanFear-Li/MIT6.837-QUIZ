#include "material.h"

#include <cmath>

#include "glCanvas.h"
#include "perlin_noise.h"

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
    Vec3f diffuse_color = diffuseColor * fmax(normal.Dot3(dirToLight), 0);

    Vec3f v = ray.getDirection();
    v.Negate();
    Vec3f half_vec = v + dirToLight;
    half_vec.Normalize();
    Vec3f specular_color = specularColor * powf(normal.Dot3(half_vec), exponent);
    return (diffuse_color + specular_color) * lightColor;
}

void PhongMaterial::glSetMaterial() const {
    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
            specularColor.r(),
            specularColor.g(),
            specularColor.b(),
            1.0};
    GLfloat diffuse[4] = {
            diffuseColor.r(),
            diffuseColor.g(),
            diffuseColor.b(),
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

Vec3f Material::getDiffuseColor(const Vec3f &coord) const {
    return diffuseColor;
}

Vec3f PhongMaterial::getSpecularColor(const Vec3f &coord) const {
    return specularColor;
}

Vec3f PhongMaterial::getReflectiveColor(const Vec3f &coord) const {
    return reflectiveColor;
}

Vec3f PhongMaterial::getTransparentColor(const Vec3f &coord) const {
    return transparentColor;
}

float PhongMaterial::getIndexOfRefraction(const Vec3f &coord) const {
    return indexOfRefraction;
}

PhongMaterial::~PhongMaterial() = default;


CheckerBoard::CheckerBoard() = default;

CheckerBoard::CheckerBoard(Matrix *m, Material *mat1, Material *mat2) {
    this->world_to_texture_mat = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
}

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f intersect_point = hit.getIntersectionPoint();
    return getMaterial(intersect_point)->Shade(ray, hit, dirToLight, lightColor);
}

void CheckerBoard::glSetMaterial() const {
    // OpenGL has no procedural texturing, just call the first material's method
    this->material_ptr_a->glSetMaterial();
}

Material *CheckerBoard::getMaterial(const Vec3f &coord) const {
    Vec3f point = getTextureCoord(coord);

    int index = int(floor(point.x()) + floor(point.y()) + floor(point.z())) % 2;
    if (!index) {
        return this->material_ptr_a;
    } else {
        return this->material_ptr_b;
    }
}

Vec3f CheckerBoard::getTextureCoord(const Vec3f &coord) const {
    Vec3f point = coord;
    this->world_to_texture_mat->Transform(point);
    return point;
}

Vec3f CheckerBoard::getSpecularColor(const Vec3f &coord) const {
    return getMaterial(coord)->getSpecularColor(coord);
}

Vec3f CheckerBoard::getReflectiveColor(const Vec3f &coord) const {
    return getMaterial(coord)->getReflectiveColor(coord);
}

Vec3f CheckerBoard::getTransparentColor(const Vec3f &coord) const {
    return getMaterial(coord)->getTransparentColor(coord);
}

float CheckerBoard::getIndexOfRefraction(const Vec3f &coord) const {
    return getMaterial(coord)->getIndexOfRefraction(coord);
}

CheckerBoard::~CheckerBoard() = default;


Noise::Noise() = default;

Noise::Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) {
    this->world_to_texture_mat = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
    this->octaves = octaves;

    this->range = 1.0f;
    this->offset = 0.5f;
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f color_a = this->material_ptr_a->Shade(ray, hit, dirToLight, lightColor);
    Vec3f color_b = this->material_ptr_b->Shade(ray, hit, dirToLight, lightColor);

    Vec3f intersect = getTextureCoord(hit.getIntersectionPoint());
    float noise = noise_clamp(intersect);
    return color_lerp(color_a, color_b, noise);
}

void Noise::glSetMaterial() const {
    // OpenGL has no procedural texturing, just call the first material's method
    this->material_ptr_a->glSetMaterial();
}

Vec3f Noise::getTextureCoord(const Vec3f &coord) const {
    Vec3f point = coord;
    this->world_to_texture_mat->Transform(point);
    return point;
}

float Noise::noise_calculate(const Vec3f &coord) const {
    Vec3f cur_coord = coord;
    float noise = 0.0f;
    float cur_octave = 1.0f;

    for (int i = 0; i < this->octaves; i++) {
        noise += PerlinNoise::noise(cur_coord.x(), cur_coord.y(), cur_coord.z()) / cur_octave;

        cur_coord *= 2.0f;
        cur_octave *= 2.0f;
    }

    return noise;
}

float Noise::noise_clamp(const Vec3f &coord) const {
    float noise = (offset - noise_calculate(coord)) / range;
    return min(1.0f, max(0.0f, noise));
}

Vec3f Noise::color_lerp(const Vec3f &x, const Vec3f &y, float noise) const {
    return x + noise * (y - x);
}

Vec3f Noise::getSpecularColor(const Vec3f &coord) const {
    Vec3f p = getTextureCoord(coord);
    return color_lerp(material_ptr_a->getDiffuseColor(p), material_ptr_b->getDiffuseColor(p), noise_clamp(coord));
}

Vec3f Noise::getReflectiveColor(const Vec3f &coord) const {
    Vec3f p = getTextureCoord(coord);
    return color_lerp(material_ptr_a->getReflectiveColor(p), material_ptr_b->getReflectiveColor(p), noise_clamp(coord));
}

Vec3f Noise::getTransparentColor(const Vec3f &coord) const {
    Vec3f p = getTextureCoord(coord);
    return color_lerp(material_ptr_a->getTransparentColor(p), material_ptr_b->getTransparentColor(p), noise_clamp(coord));
}

float Noise::getIndexOfRefraction(const Vec3f &coord) const {
    Vec3f p = getTextureCoord(coord);
    float x = material_ptr_a->getIndexOfRefraction(coord);
    float y = material_ptr_b->getIndexOfRefraction(coord);
    return x + noise_clamp(coord) * (y - x);
}

Noise::~Noise() = default;


Marble::Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) {
    this->world_to_texture_mat = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
    this->frequency = frequency;
    this->amplitude = amplitude;

    this->range = 2.0f;
    this->offset = 1.0f;
}

float Marble::noise_calculate(const Vec3f &coord) const {
    return sinf(this->frequency * coord.x() + this->amplitude * Noise::noise_calculate(coord));
}


Wood::Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) {
    this->world_to_texture_mat = m;
    this->material_ptr_a = mat1;
    this->material_ptr_b = mat2;
    this->frequency = frequency;
    this->amplitude = amplitude;

    this->range = 2.0f;
    this->offset = 1.0f;
}

float Wood::noise_calculate(const Vec3f &coord) const {
    // TODO: Wood Procedural Material Implement
    return sinf(this->frequency * coord.x() + this->amplitude * Noise::noise_calculate(coord));
}
