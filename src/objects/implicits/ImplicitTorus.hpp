#ifndef __RT_ISICG_IMPLICIT_TORUS__
#define __RT_ISICG_IMPLICIT_TORUS__

#include "objects/implicits/ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitTorus : public ImplicitSurface

    {
    public:
        /**
         * \param p_R Radius of the torus
         * \param p_r Radius of the tube
         */
        ImplicitTorus(const std::string &p_name, const float p_R, const float p_r, const Vec3f &p_position, const float p_scale = 1.0f)
            : ImplicitSurface(p_name, p_position, p_scale), _R(p_R), _r(p_r) { _compute_aabb(); }

        /**
         * \param p_R Radius of the torus
         * \param p_r Radius of the tube
         */
        ImplicitTorus(const std::string &p_name, const float p_R, const float p_r)
            : ImplicitSurface(p_name), _R(p_R), _r(p_r) { _compute_aabb(); }

    private:
        float _R = 0.2f; // major radius
        float _r = 1.f;  // minor radius

        void _compute_aabb()
        {
            Vec3f maxPoint = (_position + _scale * (Vec3f(1) * _R + _r + 0.1f));
            Vec3f minPoint = (_position - _scale * (Vec3f(1) * _R + _r + 0.1f));
            _aabb.extend(minPoint);
            _aabb.extend(maxPoint);
        };

        // Bravo, vous venez de trouver la sdf du blob du cours. félicitations !
        float _blob(const Vec3f &p) const
        {
            return p.x * p.x + p.y * p.y + p.z * p.z + glm::sin(4.f * p.x) - glm::cos(4.f * p.y) + glm::sin(4.f * p.z);
        }

        float _torus(const Vec3f &p) const
        {
            Vec2f q = Vec2f(glm::length(Vec2f(p.x, p.z)) - _R, p.y);
            return glm::length(q) - _r;
        }

        float _sdf(const Vec3f &p) const override
        {
            return _torus(p);
        }
    };
} // namespace RT_ISICG

#endif //__RT_ISICG_IMPLICIT_TORUS__
