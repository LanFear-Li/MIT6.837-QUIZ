#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray_tracer.h"
#include "glCanvas.h"

// #ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
// extern int SPECULAR_FIX_WHICH_PASS;
// #endif

class Material {
public:
    Material();

    Material(const Vec3f &d_color);

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;

    virtual void glSetMaterial() const = 0;

    Vec3f getDiffuseColor() const;

    virtual ~Material();

protected:
    Vec3f diffuseColor;

};

class PhongMaterial : public virtual Material {
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent, const Vec3f &reflectiveColor,
                  const Vec3f &transparentColor, float indexOfRefraction);

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);

    bool
    transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted);

    void glSetMaterial() const override;

    Vec3f getSpecularColor() const;

    Vec3f getReflectiveColor() const;

    Vec3f getTransparentColor() const;

    ~PhongMaterial() override;

protected:
    Vec3f specularColor;

    Vec3f reflectiveColor;

    Vec3f transparentColor;

    float exponent;

    float indexOfRefraction;
};

#endif
