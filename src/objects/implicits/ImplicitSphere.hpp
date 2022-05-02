#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitSphere : public ImplicitSurface
    {
    public:
        ImplicitSphere(const std::string &p_name)
            : ImplicitSurface(p_name) { _computeAABB(); };

        ImplicitSphere(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.f)
            : ImplicitSurface(p_name, p_position, p_scale) { _computeAABB(); }

        float _sdf(const Vec3f &p) const override
        {
            return glm::length(p) - 1.f;
        }

    private:
        void _computeAABB()
        {
            _aabb.extend(_position + _scale + 0.1f);
            _aabb.extend(_position - _scale - 0.1f);
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__