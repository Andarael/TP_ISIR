#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitSphere : public ImplicitSurface
    {
    public:
        ImplicitSphere(const std::string &p_name, const Vec3f &position, const float radius)
            : ImplicitSurface(p_name), _radius(radius), _position(position) {}

        float _sdf(const Vec3f &p_point) const override
        {
            return glm::distance(p_point, _position) - _radius;
        }

    private:
        float _radius = 1.f;
        Vec3f _position = VEC3F_ZERO;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__