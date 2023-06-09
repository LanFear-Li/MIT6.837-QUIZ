#include "forcefield.h"

ForceField::ForceField() = default;


GravityForceField::GravityForceField(const Vec3f &gravity) {
    this->gravity = gravity;
}

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return this->gravity;
}


ConstantForceField::ConstantForceField(const Vec3f &force) {
    this->force = force;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f result = this->force;
    result /= mass;
    return result;
}


RadialForceField::RadialForceField(const float &magnitude) {
    this->magnitude = magnitude;
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f f = position * this->magnitude;
    f.Negate();
    return f * (1.0f / mass);
}


VerticalForceField::VerticalForceField(const float &magnitude) {
    this->magnitude = magnitude;
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f f = Vec3f(0, -position.y(), 0) * this->magnitude;

    return f * (1.0f / mass);
}


