#include "forcefield.h"

ForceField::ForceField() = default;


GravityForceField::GravityForceField(const Vec3f &gravity) {
    this->gravity = gravity;
}

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f result = this->gravity;
    result /= mass;
    return result;
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
    float distance = position.Length();
    Vec3f f(-position.x(), -position.y(), -position.z());
    f.Normalize();

    f *= (distance / mass);
    return f;
}


VerticalForceField::VerticalForceField(const float &magnitude) {
    this->magnitude = magnitude;
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    float distance = abs(position.y());
    Vec3f f(0, -position.y(), 0);
    f.Normalize();

    f *= (distance / mass);
    return f;
}


WindForceField::WindForceField(const float &magnitude) {
    this->magnitude = magnitude;
}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


