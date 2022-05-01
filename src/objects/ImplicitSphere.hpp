#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitSphere : public ImplicitSurface
    {
    public:
        ImplicitSphere(const std::string &p_name)
            : ImplicitSurface(p_name){};

        ImplicitSphere(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.f)
            : ImplicitSurface(p_name, p_position, p_scale)

        {
            _aabb.extend(_position + _scale);
            _aabb.extend(_position - _scale);
        }

        float _sdf(const Vec3f &p) const override
        {
            return glm::length(p) - 1.f;
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__