#include "integrator.h"

void EulerIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {

}

Vec3f EulerIntegrator::getColor() {
    return Vec3f();
}


void MidpointIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {

}

Vec3f MidpointIntegrator::getColor() {
    return Vec3f();
}


void RungeKuttaIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {

}

Vec3f RungeKuttaIntegrator::getColor() {
    return Vec3f();
}
