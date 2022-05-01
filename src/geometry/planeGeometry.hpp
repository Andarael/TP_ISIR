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
            : BaseGeometry(p_center), _normal(glm::normalize(p_normal)){};

        bool intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
        {
            p_t1 = -1.f;
            p_t2 = -1.f;

            float den = dot(_normal, p_ray.getDirection());

            if (den != 0.f) // if t == 0 then ray is parallel to plane
            {
                p_t1 = dot(_position - p_ray.getOrigin(), _normal) / den;
                p_t2 = p_t1;
            }

            return p_t1 > 0;
        }

        Vec3f computeNormal(const Vec3f &p_point) const { return _normal; }

    private:
        Vec3f _normal = VEC3F_Y;
    };
} // namespace RT_ISICG

#endif