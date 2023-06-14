#include "generator.hpp"

namespace particle_system
{
    Hose_Generator::Hose_Generator(const Vec3f& p, float p_randomness, const Vec3f& v, float v_randomness)
        : position{p, p_randomness}
        , velocity{v, v_randomness}
        , random_generator{Random{}}
    {}

    auto Hose_Generator::set_colors(const Vec3f& c, const Vec3f& dead_c, float c_randomness) -> void
    {
        color = Stochastic{c, c_randomness};
        dead_color = dead_c;
    }

    auto Hose_Generator::set_lifespan(float ls, float ls_randomness, int desired_num) -> void
    {
        lifespan = Stochastic{ls, ls_randomness};
        desired_particle_num = desired_num;
    }

    auto Hose_Generator::set_mass(float m, float m_randomness) -> void
    {
        mass = Stochastic{m, m_randomness};
    }

    auto Hose_Generator::new_particle_num(float current_time, float time_step) -> int
    {
        lifespan.randomize_float(random_generator);
        auto result = static_cast<int>(time_step * desired_particle_num / lifespan.value);
        return result;
    }

    auto Hose_Generator::generate(float current_time, int i) noexcept -> std::unique_ptr<Particle>
    {
        position.randomize_vector(random_generator);
        velocity.randomize_vector(random_generator);
        color.randomize_vector(random_generator);

        mass.randomize_float(random_generator);
        lifespan.randomize_float(random_generator);

        auto particle = Particle{position.value, velocity.value, color.value, dead_color, mass.value, lifespan.value};
        return std::make_unique<Particle>(particle);
    }

    auto Hose_Generator::restart() -> void
    {
        random_generator = Random{};
    }

    auto Hose_Generator::paint() const -> void
    {
        // do nothing in hose generator
    }

    Ring_Generator::Ring_Generator(float p_randomness, const Vec3f& v, float v_randomness)
        : position_randomness{p_randomness}
        , velocity{v, v_randomness}
        , random_generator{Random{}}
    {}

    auto Ring_Generator::set_colors(const Vec3f& c, const Vec3f& dead_c, float c_randomness) -> void
    {
        color = Stochastic{c, c_randomness};
        dead_color = dead_c;
    }

    auto Ring_Generator::set_lifespan(float ls, float ls_randomness, int desired_num) -> void
    {
        lifespan = Stochastic{ls, ls_randomness};
        desired_particle_num = desired_num;
    }

    auto Ring_Generator::set_mass(float m, float m_randomness) -> void
    {
        mass = Stochastic{m, m_randomness};
    }

    auto Ring_Generator::new_particle_num(float current_time, float time_step) -> int
    {
        lifespan.randomize_float(random_generator);
        auto result = static_cast<int>(desired_particle_num / lifespan.value * (expand_velocity * current_time));
        return result;
    }

    auto Ring_Generator::generate(float current_time, int i) noexcept -> std::unique_ptr<Particle>
    {
        auto radius = current_time;
        auto angle = random_generator.next() * 2.0f * pi;

        auto pos = radius * Vec3f{static_cast<float>(std::cos(angle)), 0.0f, static_cast<float>(std::sin(angle))};
        velocity.randomize_vector(random_generator);
        color.randomize_vector(random_generator);

        mass.randomize_float(random_generator);
        lifespan.randomize_float(random_generator);

        auto particle = Particle{pos, velocity.value, color.value, dead_color, mass.value, lifespan.value};
        return std::make_unique<Particle>(particle);
    }

    auto Ring_Generator::restart() -> void
    {
        random_generator = Random{};
    }

    auto Ring_Generator::paint() const -> void
    {
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_QUADS);
        glVertex3f(10.0f, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, -10.0f);
        glEnd();
    }
}

