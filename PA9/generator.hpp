#pragma once
#include <memory>
#include <cmath>
#include "vectors.hpp"
#include "particle.hpp"
#include "random.hpp"
#include "gl.hpp"

namespace particle_system
{
    inline constexpr auto pi = 3.14159265358979323846;

    template <class T, class R = float>
    struct Stochastic final
    {
        T value;

        Stochastic(T source_value = {}, R randomness = {})
            : source_value{source_value}
            , value_randomness{randomness}
        {}

        template<class Gen>
        auto randomize_vector(Gen&& gen) -> void
        {
            if (typeid(T) == typeid(Vec3f))
            {
                value = source_value + value_randomness * gen.random_vector();
            }
        }

        template<class Gen>
        auto randomize_float(Gen&& gen) -> void
        {
            if (typeid(T) == typeid(float))
            {
                value = source_value + value_randomness * gen.random_float();
            }
        }

    private:
        T source_value{};
        R value_randomness{};
    };

    struct Generator
    {
        virtual auto set_colors(const Vec3f& c, const Vec3f& dead_c, float c_randomness) -> void = 0;
        virtual auto set_lifespan(float ls, float ls_randomness, int desired_num) -> void = 0;
        virtual auto set_mass(float m, float m_randomness) -> void = 0;

        virtual auto new_particle_num(float current_time, float time_step) -> int = 0;
        virtual auto generate(float current_time, int i) noexcept -> std::unique_ptr<Particle> = 0;

        virtual auto paint() const -> void = 0;
        virtual auto restart() -> void = 0;

        virtual ~Generator() = default;
    };

    struct Hose_Generator final: Generator
    {
        Hose_Generator(const Vec3f& p, float p_randomness, const Vec3f& v, float v_randomness);

        auto set_colors(const Vec3f& c, const Vec3f& dead_c, float c_randomness) -> void override;
        auto set_lifespan(float ls, float ls_randomness, int desired_num) -> void override;
        auto set_mass(float m, float m_randomness) -> void override;

        auto new_particle_num(float current_time, float time_step) -> int override;
        auto generate(float current_time, int i) noexcept -> std::unique_ptr<Particle> override;

        auto restart() -> void override;
        auto paint() const -> void override;

    private:
        Stochastic<Vec3f, float> color{};
        Stochastic<Vec3f, float> position{};
        Stochastic<Vec3f, float> velocity{};

        Stochastic<float, float> lifespan{};
        Stochastic<float, float> mass{};

        Vec3f dead_color{};

        Random random_generator{};

        int desired_particle_num{};
    };

    struct Ring_Generator final: Generator
    {
        Ring_Generator(float p_randomness, const Vec3f& v, float v_randomness);

        auto set_colors(const Vec3f& c, const Vec3f& dead_c, float c_randomness) -> void override;
        auto set_lifespan(float ls, float ls_randomness, int desired_num) -> void override;
        auto set_mass(float m, float m_randomness) -> void override;

        auto new_particle_num(float current_time, float time_step) -> int override;
        auto generate(float current_time, int i) noexcept -> std::unique_ptr<Particle> override;

        auto restart() -> void override;
        auto paint() const -> void override;

    private:
        Stochastic<Vec3f, float> color{};
        Stochastic<Vec3f, float> velocity{};

        Stochastic<float, float> lifespan{};
        Stochastic<float, float> mass{};

        Random random_generator{};

        Vec3f dead_color{};

        int desired_particle_num{};
        float position_randomness{};
        float expand_velocity{0.1f};
    };
}

