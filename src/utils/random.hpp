#ifndef __RT_ISICG_RANDOM__
#define __RT_ISICG_RANDOM__

#include <random>

namespace RT_ISICG
{
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dis(0.f, 1.f);

    static void setRandomSeed(const int seed)
    {
        std::mt19937 newgen(seed);
        gen = newgen;
    }

    static float randomFloat()
    {
        return dis(gen);
    }

    static Vec2f randomVec2f()
    {
        return Vec2f(randomFloat(), randomFloat());
    }

    static Vec3f randomVec3f()
    {
        return Vec3f(randomFloat(), randomFloat(), randomFloat());
    }

} // namespace RT_ISICG

#endif // __RT_ISICG_RANDOM__
