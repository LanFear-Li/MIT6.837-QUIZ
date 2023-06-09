#include "generator.h"

void Generator::SetColors(const Vec3f &c, const Vec3f &dead_c, float c_randomness) {
    this->color = c;
    this->dead_color = dead_c;
    this->color_randomness = c_randomness;
}

void Generator::SetLifespan(float ls, float ls_randomness, int desired_num) {
    this->lifespan = ls;
    this->lifespan_randomness = ls_randomness;
    this->desired_num_particles = desired_num;
}

void Generator::SetMass(float m, float m_randomness) {
    this->mass = m;
    this->mass_randomness = m_randomness;
}

int Generator::numNewParticles(float current_time, float dt) const {
    return int(dt * this->desired_num_particles / this->lifespan);
}

void Generator::Restart() {
    this->random_generator = Random();
}


HoseGenerator::HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity,
                             float velocity_randomness) {
    this->position = position;
    this->position_randomness = position_randomness;
    this->velocity = velocity;
    this->velocity_randomness = velocity_randomness;
}


Particle *HoseGenerator::Generate(float current_time, int i) {
    Vec3f pos = this->position + this->position_randomness * this->random_generator.randomVector();
    Vec3f vel = this->velocity + this->velocity_randomness * this->random_generator.randomVector();

    auto *p = new Particle(pos, vel, this->color, this->dead_color, this->mass, this->lifespan);
    return p;
}

void HoseGenerator::Paint() const {
    // do nothing in hose generator
}


RingGenerator::RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) {
    this->position_randomness = position_randomness;
    this->velocity = velocity;
    this->velocity_randomness = velocity_randomness;
}

int RingGenerator::numNewParticles(float current_time, float dt) const {
    return int(this->desired_num_particles / this->lifespan * (this->expand_velocity * current_time));
}

Particle *RingGenerator::Generate(float current_time, int i) {
    float radius = current_time;
    float angle = this->random_generator.next() * 2.0f * M_PI;

    Vec3f pos = radius * Vec3f(cosf(angle), 0, sinf(angle));
    Vec3f vel = this->velocity + this->velocity_randomness * this->random_generator.randomVector();

    auto *p = new Particle(pos, vel, this->color, this->dead_color, this->mass, this->lifespan);
    return p;
}

void RingGenerator::Paint() const {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(10.f, 0.f, 10.f);
    glVertex3f(-10.f, 0.f, 10.f);
    glVertex3f(-10.f, 0.f, -10.f);
    glVertex3f(10.f, 0.f, -10.f);
    glEnd();
}