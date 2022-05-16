#ifndef __RT_ISICG_IMPLICIT_JULIA__
#define __RT_ISICG_IMPLICIT_JULIA__

#include "objects/implicits/ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitJulia : public ImplicitSurface
    {
    public:
        ImplicitJulia(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.0f)
            : ImplicitSurface(p_name, p_position, p_scale)
        {
            _aabb.extend(_position - _scale * 1.1f);
            _aabb.extend(_position + _scale * 1.1f);
        };

        float _sdf(const Vec3f &p_point) const override
        {
            return _sdfBulb1(p_point);
        }

    private:
        int _steps = 100;
        const Vec4f kC = Vec4f(-2, 6, 15, -6) / 22.f;
        bool cut = true;
        bool orbitalTraps = false;
        float trap_size = 0.03f;
        float cutPoint = 0.1f;

        static Vec4f qSquare(const Vec4f &q)
        {
            return Vec4f(q.x * q.x - q.y * q.y - q.z * q.z - q.w * q.w,
                         2.f * q.x * Vec3f(q.y, q.z, q.w));
        }

        static Vec4f qCube(const Vec4f &q)
        {
            Vec4f q2 = q * q;
            return Vec4f(q.x * (q2.x - 3.f * q2.y - 3.f * q2.z - 3.f * q2.w),
                         Vec3f(q.y, q.z, q.w) * (3.f * q2.x - q2.y - q2.z - q2.w));
        }

        float _sdfBulb1(const Vec3f &p_point) const
        {
            Vec3f p = p_point;
            Vec4f z = Vec4f(p, 0);
            float dz2 = 1;
            float m2 = 0;
            float n = 0;
            float o = 1e10f;

            for (int i = 0; i < _steps; i++)
            {
                // z' = 3z² -> |z'|² = 9|z²|²
                dz2 *= 9.f * glm::length2(qSquare(z));

                // z = z³ + c
                z = qCube(z) + kC;

                // stop under divergence
                m2 = glm::length2(z);

                if (orbitalTraps)
                    o = glm::min(o, glm::length(Vec2f(z.x, z.z) - Vec2f(0.45, 0.55)) - trap_size);

                // exit condition
                if (m2 > 256.f)
                    break;

                n += 1.f;
            }

            // sdf(z) = log|z|·|z|/|dz| : https://iquilezles.org/articles/distancefractals
            float d = 0.25f * log(m2) * sqrt(m2 / dz2);

            if (orbitalTraps)
                d = glm::min(o, d);

            if (cut)
            {
                d = glm::max(d, p.y - cutPoint);
            }

            return glm::min(d, 4.f); // this is to avoid numerical errors when camera is far away
            // return Vec2f(d, n);
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_JULIA__