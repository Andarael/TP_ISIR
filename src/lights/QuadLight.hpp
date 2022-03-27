#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "BaseLight.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
    class QuadLight : public BaseLight
    {

        // todo the ligt is too dimm compared to the TP

        // todo simpler quadlight avec juste area, pos, et lookat

    public:
        QuadLight() = delete;

        QuadLight(const Vec3f &p_color, Vec3f p_position, const float p_power, Vec3f p_u, Vec3f p_v)
            : BaseLight(p_color, p_power), _position(p_position), _u(p_u), _v(p_v)
        {
            float theta = glm::angle(p_u, p_v);

            // todo investigate
            _area = 1.41f * glm::length(glm::cross(p_u, p_v));

            _normal = glm::normalize(glm::cross(p_u, p_v));

            printf("area = %f\n", _area); // mon gros debug

            _isSurface = true;
        };

        // getters and setters
        Vec3f getPosition() const
        {
            return _position;
        }

        void setPosition(const Vec3f &p_position)
        {
            _position = p_position;
        }

        // methods

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec3f randomSamplePoint = _position + _u * randomFloat() + _v * randomFloat();

            Vec3f direction = glm::normalize(randomSamplePoint - p_point);
            float distance = glm::distance(randomSamplePoint, p_point);

            float cosTheta = glm::dot(_normal, direction);
            float pdf = 1.f / _area * distance * distance / cosTheta;

            Vec3f radiance = (_color * _power) / pdf;

            return LightSample(direction, distance, radiance, pdf);
        }

    private:
        Vec3f _position = VEC3F_ZERO;
        Vec3f _u = Vec3f(1, 0, 0);
        Vec3f _v = Vec3f(0, 0, 1);
        Vec3f _normal = Vec3f(0, 1, 0);

        float _area = 1.0f;
    };

} // namespace RT_ISICG

#endif //__RT_ISICG_QUAD_LIGHT__