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

    virtual Vec3f getSpecularColor() const = 0;

    virtual Vec3f getReflectiveColor() const = 0;

    virtual Vec3f getTransparentColor() const = 0;

    virtual float getIndexOfRefraction() const = 0;

    virtual ~Material();

protected:
    Vec3f diffuseColor;

};

class PhongMaterial : public virtual Material {
public:
    PhongMaterial(const Vec3f &diffuse, const Vec3f &specular, float e, const Vec3f &reflective,
                  const Vec3f &transparent, float refraction);

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    Vec3f getSpecularColor() const override;

    Vec3f getReflectiveColor() const override;

    Vec3f getTransparentColor() const override;

    float getIndexOfRefraction() const override;

    ~PhongMaterial() override;

protected:
    Vec3f specularColor;

    Vec3f reflectiveColor;

    Vec3f transparentColor;

    float exponent;

    float indexOfRefraction;
};

#endif
