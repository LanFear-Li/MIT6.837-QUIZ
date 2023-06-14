#ifndef _PA_FORCEFIELD_H_
#define _PA_FORCEFIELD_H_

#include "vectors.hpp"

class ForceField {
public:
    ForceField();

    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;

protected:
    Vec3f gravity;
    Vec3f force;
    float magnitude{};
};

class GravityForceField : public ForceField {
public:
    explicit GravityForceField(const Vec3f &gravity);

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;
};

class ConstantForceField : public ForceField {
public:
    explicit ConstantForceField(const Vec3f &force);

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;
};

class RadialForceField : public ForceField {
public:
    explicit RadialForceField(const float &magnitude);

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;
};

class VerticalForceField : public ForceField {
public:
    explicit VerticalForceField(const float &magnitude);

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;
};

class WindForceField : public ForceField {
public:
    explicit WindForceField(const float &magnitude);

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;
};

#endif
