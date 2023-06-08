#ifndef _PA_INTEGRATOR_H_
#define _PA_INTEGRATOR_H_

#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
    virtual void Update(Particle *particle, ForceField *force_field, float t, float dt) = 0;

    virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *force_field, float t, float dt) override;

    Vec3f getColor() override;
};

class MidpointIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *force_field, float t, float dt) override;

    Vec3f getColor() override;
};

class RungeKuttaIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *force_field, float t, float dt) override;

    Vec3f getColor() override;
};

#endif
