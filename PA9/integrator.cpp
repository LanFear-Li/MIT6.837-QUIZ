#include "integrator.h"

void EulerIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {
    Vec3f acceleration = force_field->getAcceleration(particle->getPosition(), particle->getMass(), t);

    Vec3f position = particle->getPosition();
    Vec3f velocity = particle->getVelocity();

    position += velocity * dt;
    particle->setPosition(position);
    velocity += acceleration * dt;
    particle->setVelocity(velocity);

    particle->increaseAge(dt);
}

Vec3f EulerIntegrator::getColor() {
    return {};
}


void MidpointIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {
    Vec3f acceleration = force_field->getAcceleration(particle->getPosition(), particle->getMass(), t);

    Vec3f position_n = particle->getPosition();
    Vec3f velocity_n = particle->getVelocity();

    Vec3f position_m = position_n + velocity_n * (dt / 2);
    Vec3f velocity_m = velocity_n + acceleration * (dt / 2);

    Vec3f acceleration_next = force_field->getAcceleration(position_m, particle->getMass(), t + dt / 2);
    Vec3f position_next = position_n + velocity_m * dt;
    Vec3f velocity_next = velocity_n + acceleration_next * dt;
    particle->setPosition(position_next);
    particle->setVelocity(velocity_next);

    particle->increaseAge(dt);
}

Vec3f MidpointIntegrator::getColor() {
    return {};
}


void RungeKuttaIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {

}

Vec3f RungeKuttaIntegrator::getColor() {
    return {};
}
