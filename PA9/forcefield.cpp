#include "forcefield.h"

ForceField::ForceField() = default;


GravityForceField::GravityForceField(const Vec3f &gravity) {

}

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


ConstantForceField::ConstantForceField(const Vec3f &force) {

}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


RadialForceField::RadialForceField(const float &magnitude) {

}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


VerticalForceField::VerticalForceField(const float &magnitude) {

}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


WindForceField::WindForceField(const float &magnitude) {

}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return Vec3f();
}


