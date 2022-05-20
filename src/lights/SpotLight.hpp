#ifndef __RT_ISICG_SPOT_LIGHT__
#define __RT_ISICG_SPOT_LIGHT__

#include "defines.hpp"
#include "lights/PointLight.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
    class SpotLight : public PointLight
    {

    public:
        SpotLight() = delete;

        SpotLight(const Vec3f &p_color, const Vec3f &p_position, const Vec3f &p_normal, const float p_spread, const float p_power, const float p_radius)
            : PointLight(p_color, p_position, p_power, p_radius), _normal(p_normal), _spread(p_spread) {}

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec3f sample = getSamplePoint(p_point);

            Vec3f direction = getDirection(p_point, sample);
            float distance = getDistance(p_point, sample);

            float cosTheta = abs(glm::dot(_normal, direction));

            // todo

            return PointLight::sample(p_point);
        }

    private:
        Vec3f _normal = -VEC3F_Y;
        float _spread = 1.0f;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SPOT_LIGHT__