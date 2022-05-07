#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "lights/BaseLight.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
    class QuadLight : public BaseLight
    {

        // todo the ligt is too dimm compared to the TP

        // todo simpler quadlight avec juste area, pos, et lookat

    public:
        QuadLight() = delete;

        QuadLight(const Vec3f &p_color, const float p_power, Vec3f p_position, Vec3f p_u, Vec3f p_v)
            : BaseLight(p_color, p_power), _position(p_position), _u(p_u), _v(p_v)
        {
            Vec3f crossUV = glm::cross(p_u, p_v);
            _area = glm::length(crossUV);
            _normal = glm::normalize(crossUV);
            _isSurface = true;
        };

        // getters and setters
        Vec3f getPosition() const { return _position; }

        void setPosition(const Vec3f &p_position) { _position = p_position; }

        // methods

        LightSample sample(const Vec3f &p_point) const override
        {
            Vec2f rand2D = randomVec2f();
            Vec3f randomUV = _u * rand2D.x + _v * rand2D.y;
            Vec3f randomSamplePoint = _position + randomUV;

            Vec3f direction = glm::normalize(randomSamplePoint - p_point);
            float distance = glm::distance(randomSamplePoint, p_point);

            float cosTheta = abs(glm::dot(_normal, direction));
            float pdf = 1.0f / _area * (distance * distance) / cosTheta;

            Vec3f radiance = (_color * _power) / pdf;

            return LightSample(direction, distance, radiance, pdf);
        }

        void displayLight() const
        {
            std::cout << "QuadLight" << std::endl;
            std::cout << "position: " << glm::to_string(_position) << std::endl;
            std::cout << "u: " << glm::to_string(_u) << std::endl;
            std::cout << "v: " << glm::to_string(_v) << std::endl;
            std::cout << "area: " << _area << std::endl;
            std::cout << "normal: " << glm::to_string(_normal) << std::endl;
            std::cout << "color: " << glm::to_string(_color) << std::endl;
            std::cout << "power: " << _power << std::endl;
        }

    protected:
        Vec3f _position = VEC3F_ZERO;
        Vec3f _u = VEC3F_X;
        Vec3f _v = VEC3F_Z;
        Vec3f _normal = -VEC3F_Y;

        float _area = 1.0f;
    };

} // namespace RT_ISICG

#endif //__RT_ISICG_QUAD_LIGHT__