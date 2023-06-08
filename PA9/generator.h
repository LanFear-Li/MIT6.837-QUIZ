#ifndef _PA_GENERATOR_H_
#define _PA_GENERATOR_H_

#include "vectors.h"
#include "particle.h"

class Generator {
public:
    // initialization
    virtual void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) = 0;

    virtual void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) = 0;

    virtual void SetMass(float mass, float mass_randomness) = 0;

    // on each time step, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;

    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const = 0;

    virtual void Restart() = 0;
};

class HoseGenerator : public Generator {
public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness);

    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) override;

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) override;

    void SetMass(float mass, float mass_randomness) override;

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

    void Restart() override;
};

class RingGenerator : public Generator {
public:
    RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness);

    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) override;

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) override;

    void SetMass(float mass, float mass_randomness) override;

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

    void Restart() override;
};

#endif
