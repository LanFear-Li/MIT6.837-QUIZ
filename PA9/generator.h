#ifndef _PA_GENERATOR_H_
#define _PA_GENERATOR_H_

#include "vectors.h"
#include "particle.h"
#include "random.h"
#include "glCanvas.h"

class Generator {
public:
    // initialization
    void SetColors(const Vec3f &c, const Vec3f &dead_c, float c_randomness);

    void SetLifespan(float ls, float ls_randomness, int desired_num);

    void SetMass(float m, float m_randomness);

    // on each time step, create some particles
    virtual int numNewParticles(float current_time, float dt) const;

    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const = 0;

    void Restart();

protected:
    Vec3f color{};
    Vec3f dead_color{};
    float color_randomness{};

    float lifespan{};
    float lifespan_randomness{};
    int desired_num_particles{};

    float mass{};
    float mass_randomness{};

    Random random_generator{};
};

class HoseGenerator : public Generator {
public:
    HoseGenerator(const Vec3f& position, float position_randomness, const Vec3f& velocity, float velocity_randomness);

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

protected:
    Vec3f position{};
    float position_randomness{};
    Vec3f velocity{};
    float velocity_randomness{};
};

class RingGenerator : public Generator {
public:
    RingGenerator(float position_randomness, const Vec3f& velocity, float velocity_randomness);

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

protected:
    float position_randomness{};
    Vec3f velocity{};
    float velocity_randomness{};

    float expand_velocity = 0.1f;
    float radius_randomness = 0.2f;
};

#endif
