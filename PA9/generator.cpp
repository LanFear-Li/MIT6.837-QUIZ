//
// Created by user on 6/8/23.
//

#include "generator.h"

HoseGenerator::HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness) {

}

void HoseGenerator::SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {

}

void HoseGenerator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {

}

void HoseGenerator::SetMass(float mass, float mass_randomness) {

}

int HoseGenerator::numNewParticles(float current_time, float dt) const {
    return 0;
}

Particle *HoseGenerator::Generate(float current_time, int i) {
    return nullptr;
}

void HoseGenerator::Paint() const {

}

void HoseGenerator::Restart() {

}


RingGenerator::RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness) {

}

void RingGenerator::SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {

}

void RingGenerator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {

}

void RingGenerator::SetMass(float mass, float mass_randomness) {

}

int RingGenerator::numNewParticles(float current_time, float dt) const {
    return 0;
}

Particle *RingGenerator::Generate(float current_time, int i) {
    return nullptr;
}

void RingGenerator::Paint() const {

}

void RingGenerator::Restart() {

}
