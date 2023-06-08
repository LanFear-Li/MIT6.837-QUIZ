#ifndef _PA_PARTICLE_SET_H_
#define _PA_PARTICLE_SET_H_

#include <cassert>

#include "particle.h"

// ============================================================
//
// A dynamically resizable array of particles.
// 
// Contains a special RemoveDead procedure to filter out 
// particles whose life clock has reached zero.

class ParticleSet {

public:
    // CONSTRUCTOR & DESTRUCTOR
    explicit ParticleSet(int n) {
        size = n;
        num_particles = 0;
        particles = new Particle *[size];
        for (int i = 0; i < size; i++) {
            particles[i] = nullptr;
        }
    }

    ~ParticleSet() {
        for (int i = 0; i < num_particles; i++) {
            delete particles[i];
        }
        delete[] particles;
    }

    // ACCESSORS
    int getNumParticles() const { return num_particles; }

    Particle *Get(int i) const {
        assert (i >= 0 && i < num_particles);
        assert (particles[i] != nullptr);
        return particles[i];
    }

    // MODIFIERS
    void Add(Particle *p) {
        assert (p != nullptr);
        if (num_particles == size) {
            // double the space & copy
            auto **new_particles = new Particle *[size * 2];
            for (int i = 0; i < size; i++) {
                new_particles[i] = particles[i];
                new_particles[i + size] = nullptr;
            }
            delete[] particles;
            particles = new_particles;
            size *= 2;
        }
        assert (num_particles < size);
        particles[num_particles] = p;
        num_particles++;
    }

    void RemoveDead() {
        int i = 0;
        while (true) {
            if (i == num_particles) break;
            assert (particles[i] != nullptr);
            if (particles[i]->isDead()) {
                // delete this particle
                delete particles[i];
                num_particles--;
                // & copy the last particle to this spot
                particles[i] = particles[num_particles];
                particles[num_particles] = nullptr;
            } else {
                i++;
            }
        }
        // verify cleanup
        for (i = 0; i < size; i++) {
            if (i < num_particles) {
                assert (particles[i] != nullptr);
                assert (!particles[i]->isDead());
            } else {
                assert (particles[i] == nullptr);
            }
        }
    }


private:

    // don't use this constructor
    ParticleSet() { assert(0); }

    // REPRESENTATION
    int num_particles{};
    int size{};
    Particle **particles{};
};

// ============================================================

#endif
