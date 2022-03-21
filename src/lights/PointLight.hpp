#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "BaseLight.hpp"

namespace RT_ISICG
{
    class PointLight : public BaseLight
    {

    public:
        PointLight() = delete;

        PointLight(const Vec3f &p_color, Vec3f p_position, const float p_power = 1.f)
            : BaseLight(p_color, p_power), _position(p_position)
        {
        }

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec3f direction = glm::normalize(_position - p_point); // todo maybe inverted
            float distance = glm::distance(_position, p_point);
            Vec3f radiance = _color * _power / (distance * distance);
            float pdf = 1.f;
            return LightSample(direction, distance, radiance, pdf);
        }

    private:
        Vec3f _position;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__