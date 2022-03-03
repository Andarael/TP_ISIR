#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
    class PointLight : BaseLight
    {
    private:
        Vec3f _position;

    public:
        PointLight() = delete;

        PointLight(const Vec3f &p_color, Vec3f p_position, const float p_power = 1.f);

        ~PointLight();

        LightSample sample(const Vec3f &p_point) const;
    };

    LightSample PointLight::sample(const Vec3f &p_point) const
    {
        Vec3f direction = glm::normalize(_position - p_point); // todo maybe inverted
        float distance = glm::distance(_position, p_point);
        Vec3f radiance = _color * _power / (distance * distance);
        return LightSample(direction, distance, radiance, 1.0f);
    }


} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__