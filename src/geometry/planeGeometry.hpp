#ifndef __RT_ISICG_PLANE_GEOMETRY__
#define __RT_ISICG_PLANE_GEOMETRY__

#include "BaseGeometry.hpp"

namespace RT_ISICG
{
    class PlaneGeometry : public BaseGeometry
    {
    public:
        PlaneGeometry() = delete;

        PlaneGeometry(const Vec3f &p_center, const Vec3f &p_normal)
            : _position(p_center), _normal(p_normal)
        {
        }

        const Vec3f &getCenter() const
        {
            return _position;
        }

        bool intersect(const Ray &p_ray, float &p_t1, float &p_t2) const;

        Vec3f computeNormal(const Vec3f &p_point) const
        {
            return glm::normalize(_normal);
        }

    private:
        Vec3f _position = VEC3F_ZERO;
        Vec3f _normal = Vec3f(0, 1, 0);
    };
}

#endif