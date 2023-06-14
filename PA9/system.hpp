#ifndef _PA_SYSTEM_H_
#define _PA_SYSTEM_H_

#include <cassert>
#include "vectors.hpp"
#include "generator.hpp"

class Particle;

class ParticleSet;

struct Generator;

class Integrator;

class ForceField;

// ====================================================================
// ====================================================================
// The class for managing a particle system.

// This class keeps track of the list of particles, the 
// generator, iterator & force field of the system and
// has the main loop for updating particles.

class System {

public:

    // CONSTRUCTOR & DESTRUCTOR
    System(particle_system::Generator *g, Integrator *i, ForceField *f);

    ~System();

    // FUNCTIONALITY
    void Restart();

    void Update(float dt);

    // RENDERING
    void Paint(float dt, int integrator_color, int draw_vectors,
               float acceleration_scale, int motion_blur) const;

    void PaintGeometry() const;


private:
    // don't use this constructor
    System() { assert(0); }

    // REPRESENTATION
    ParticleSet *particles{};
    particle_system::Generator *generator{};
    Integrator *integrator{};
    ForceField *forcefield{};
    float current_time{};
};

// ====================================================================
// ====================================================================


#endif
