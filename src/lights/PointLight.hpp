#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "lights/BaseLight.hpp"

namespace RT_ISICG
{
    class PointLight : public BaseLight
    {

    public:
        PointLight() = delete;

        PointLight(const Vec3f &p_color, const Vec3f &p_position, const float p_power = 1.f, const float p_radius = 0.f)
            : BaseLight(p_color, p_power), _radius(p_radius), _position(p_position)
        {
            _isSurface = (p_radius > _min_radius); // we consider that the light is a point light if the radius is greater than 0.01
        }

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec3f samplePoint = getSamplePoint(p_point);

            Vec3f direction = getDirection(p_point, samplePoint);
            float distance = getDistance(p_point, samplePoint);

            float cosTheta = abs(glm::dot(getNormal(p_point), direction));
            float pdf = 1.f / cosTheta; // pdf stays contant with radius for easier light intensity control

            Vec3f radiance = (_color * _power) / (distance * distance) / pdf;

            return LightSample(direction, distance, radiance, pdf);
        }

    protected:
        float _radius;
        Vec3f _position;
        float _min_radius = 0.01f;

        Vec3f getNormal(const Vec3f &p_point) const { return glm::normalize(p_point - _position); }

        // sample a random hemisphere in the direction of the point
        virtual Vec3f getSamplePoint(const Vec3f &p_point) const
        {
            if (_radius < _min_radius)
                return _position;

            Vec3f dir = glm::normalize((randomVec3f() - .5f) * 2.f);
            if (glm::dot(dir, getNormal(p_point)) < 0)
                dir = -dir;

            return _position + (dir * _radius);
        }
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__