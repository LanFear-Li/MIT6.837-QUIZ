#include "integrator.hpp"

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
    return {1.0f, 0, 0};
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
    return {0, 1.0f, 0};
}


void RungeKuttaIntegrator::Update(Particle *particle, ForceField *force_field, float t, float dt) {
    float mass = particle->getMass();

    Vec3f position_n = particle->getPosition();
    Vec3f velocity_n = particle->getVelocity();

    Vec3f position_k1 = dt * velocity_n;
    Vec3f velocity_k1 = dt * force_field->getAcceleration(position_n, mass, t);

    Vec3f position_k2 = dt * (velocity_n + velocity_k1 * 0.5f);
    Vec3f velocity_k2 = dt * force_field->getAcceleration(position_n + position_k1 * 0.5f, mass, dt * 0.5f);

    Vec3f position_k3 = dt * (velocity_n + velocity_k2 * 0.5f);
    Vec3f velocity_k3 = dt * force_field->getAcceleration(position_n + position_k2 * 0.5f, mass, dt * 0.5f);

    Vec3f position_k4 = dt * (velocity_n + velocity_k3);
    Vec3f velocity_k4 = dt * force_field->getAcceleration(position_n + position_k3, mass, dt);

    Vec3f position_next = position_n + (1.0f / 6.0f) * (position_k1 + position_k2 * 2 + position_k3 * 2 + position_k4);
    Vec3f velocity_next = velocity_n + (1.0f / 6.0f) * (velocity_k1 + velocity_k2 * 2 + velocity_k3 * 2 + velocity_k4);
    particle->setPosition(position_next);
    particle->setVelocity(velocity_next);

    particle->increaseAge(dt);
}

Vec3f RungeKuttaIntegrator::getColor() {
    return {0, 0, 1.0f};
}
