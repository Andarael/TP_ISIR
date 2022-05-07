#ifndef __RT_ISICG_IMPLICIT_BULB__
#define __RT_ISICG_IMPLICIT_BULB__

#include "objects/implicits/ImplicitSurface.hpp"

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
        int _steps = 5;

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
                // todo test with 8.0f instead of 8.0, does the bug disappear ?
                dz = 8.0f * glm::pow(m, 3.5f) * dz + 1.f;

                // z = z^8+z
                float r = glm::length(w);
                float theta = 8.f * glm::acos(w.y / r);
                float phi = 8.f * glm::atan(w.x, w.z);
                float temp = glm::pow(r, 8.f); // r * r * r * r * r * r * r * r; //
                w = p_point + temp * Vec3f(glm::sin(theta) * glm::sin(phi), glm::cos(theta), sin(theta) * glm::cos(phi));

                trap = glm::min(trap, Vec4f(glm::abs(w), m));

                m = glm::dot(w, w);
            }

            float value = 0.25f * glm::log(m) * glm::sqrt(m) / dz;

            return glm::min(value, 4.f); // this is to avoid numerical errors when camera is far away
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BULB__