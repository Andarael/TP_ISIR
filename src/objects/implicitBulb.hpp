#ifndef __RT_ISICG_IMPLICIT_BULB__
#define __RT_ISICG_IMPLICIT_BULB__

#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitBulb : public ImplicitSurface
    {
    public:
        ImplicitBulb(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.0f)
            : ImplicitSurface(p_name, p_position, p_scale)
        {
            _aabb.extend(_position - _scale * 1.3f);
            _aabb.extend(_position + _scale * 1.3f);
        };

        float _sdf(const Vec3f &p_point) const override
        {
            return _sdfBulb1(p_point);
        }

    private:
        float _power = 3.f;
        int _steps = 3;

        float _sdfBulb1(const Vec3f &p_point) const
        {
            Vec3f w = p_point;
            float m = glm::dot(w, w);

            Vec4f trap = Vec4f(glm::abs(w), m);
            float dz = 1.f;

            for (int i = 0; i < _steps; i++)
            {
                if (m > 1.5f) // no need for unnecessary details
                    break;

                // dz = 8*z^7*dz
                dz = 8.0 * glm::pow(m, 3.5f) * dz + 1.f;

                // z = z^8+z
                float r = glm::length(w);
                float theta = 8.f * glm::acos(w.y / r);
                float phi = 8.f * glm::atan(w.x, w.z);
                float temp = r * r * r * r * r * r * r * r; // glm::pow(r, 8.f);
                w = p_point + temp * Vec3f(glm::sin(theta) * glm::sin(phi), glm::cos(theta), sin(theta) * glm::cos(phi));

                trap = glm::min(trap, Vec4f(glm::abs(w), m));

                m = glm::dot(w, w);
            }

            return 0.25f * glm::log(m) * glm::sqrt(m) / dz;
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BULB__