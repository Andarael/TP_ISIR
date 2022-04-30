#ifndef __RT_ISICG_IMPLICIT_BULB__
#define __RT_ISICG_IMPLICIT_BULB__

#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitBulb : public ImplicitSurface
    {
    public:
        ImplicitBulb(const std::string &p_name, const Vec3f &position, const float radius)
            : ImplicitSurface(p_name), _radius(radius), _position(position)
        {
            _aabb.extend(Vec3f(1));
            _aabb.extend(Vec3f(-1));
        }

        float _sdf(const Vec3f &p_point) const override
        {
            // Vec3f z = p_point;
            // float dr = 1.f;
            // float r = 0.f;
            // for (int i = 0; i < _steps; i++)
            //{
            //     r = length(z);

            //    if (r > 2.5f) break;

            //    // convert to polar coordinates
            //    float theta = acos(z.z / r);
            //    float phi = glm::atan(z.y, z.x);
            //    dr = pow(r, _power - 1.0) * _power * dr + 1.f;

            //    // scale and rotate the point
            //    float zr = pow(r, _power);
            //    theta = theta * _power;
            //    phi = phi * _power;

            //    // convert back to cartesian coordinates
            //    z = zr * Vec3f(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
            //    z += p_point;
            //}

            // return .5f * glm::log(r) * r / dr;

            Vec3f w = p_point;
            float m = dot(w, w);

            Vec4f trap = Vec4f(glm::abs(w), m);
            float dz = 1.f;

            for (int i = 0; i < _steps; i++)
            {
                if (m > 1.5f) // no need for unnecessary details
                    break;

                // dz = 8*z^7*dz
                dz = 8.0 * pow(m, 3.5f) * dz + 1.f;

                // z = z^8+z
                float r = glm::length(w);
                float theta = 8.f * acos(w.y / r);
                float phi = 8.f * glm::atan(w.x, w.z);
                w = p_point + float(glm::pow(r, 8.f)) * Vec3f(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));

                trap = glm::min(trap, Vec4f(abs(w), m));

                m = glm::dot(w, w);
            }

            return 0.25f * glm::log(m) * glm::sqrt(m) / dz;
        }

    private:
        float _power = 3.f;
        int _steps = 6;
        float _radius = 1.f;
        Vec3f _position = VEC3F_ZERO;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BULB__