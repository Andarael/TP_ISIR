#ifndef __RT_ISICG_SUN_LIGHT__
#define __RT_ISICG_SUN_LIGHT__

#include "BaseLight.hpp"
#include "defines.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
    class SunLight : public BaseLight
    {

    public:
        SunLight(const Vec3f &p_color, const float p_power, const Vec3f &p_direction, const float p_size = 0.0f)
            : BaseLight(p_color, p_power), _direction(glm::normalize(p_direction)), _size(p_size)
        {
            _isSurface = _size > 0.001f;
        }

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec3f direction = getDirection(VEC3F_ZERO, VEC3F_ZERO);
            float distance = getDistance(p_point, VEC3F_ZERO);

            float pdf = 1.f;
            Vec3f radiance = (_color * _power) / pdf;

            return LightSample(direction, distance, radiance, pdf);
        }

        float getDistance(const Vec3f &p_point, const Vec3f &samplePoint) const override { return TMAX; }

        Vec3f getDirection(const Vec3f &p_point, const Vec3f &samplePoint) const override
        {
            Vec3f random = glm::normalize((randomVec3f() - .5f) * 2.f);
            return glm::normalize(-_direction + random * _size);
        }

    private:
        Vec3f _direction;
        float _size;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SUN_LIGHT__