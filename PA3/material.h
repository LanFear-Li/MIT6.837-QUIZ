#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray_caster.h"
#include "glCanvas.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class Material {
public:
    Material();

    Material(const Vec3f &d_color);

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) {}

    virtual void glSetMaterial() {}

    Vec3f getDiffuseColor() const;

    ~Material();

protected:
    Vec3f diffuseColor;

};

class PhongMaterial : public virtual Material {
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent);

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;

    void glSetMaterial() const;

    Vec3f getSpecularColor() const;

    ~PhongMaterial();

protected:
    Vec3f specularColor;

    float exponent;
};

#endif
