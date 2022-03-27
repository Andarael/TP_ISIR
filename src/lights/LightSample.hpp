#ifndef __RT_ISICG_LIGHT_SAMPLE__
#define __RT_ISICG_LIGHT_SAMPLE__

#include "defines.hpp"

namespace RT_ISICG
{
    /**
     * @details
     * LightSample is a struct that contains the result of a light sample.
     * It contains the direction to the light, the distance to the light, the color of the light,
     * and the probability density function of the light sample.
     */
    struct LightSample
    {
    public:
        /**
         * @brief Construct a new Light Sample object,
         * @param p_direction the direction to the light
         * @param p_distance the distance to the light
         * @param p_radiance The radiance of the light (attenuation is already taken into account).
         * @param p_pdf The probability density function of the light sample.
         */
        LightSample(const Vec3f &p_direction, const float p_distance, const Vec3f &p_radiance, const float p_pdf)
            : _direction(p_direction), _distance(p_distance), _radiance(p_radiance), _pdf(p_pdf){};

        Vec3f _direction = VEC3F_ZERO; // normalized direction toward light source
        float _distance = 0.f;         // distance to light source
        Vec3f _radiance = WHITE;       // radiance at given point divided by pdf
        float _pdf = 1.f;              // probability density function of the light sampled
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_LIGHT_SAMPLE__
