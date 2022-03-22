#include "PlaneGeometry.hpp"

namespace RT_ISICG
{
    bool PlaneGeometry::intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
    {
        p_t1 = -1.f;
        p_t2 = -1.f;

        float den = dot(_normal, p_ray.getDirection());

        if (den != 0.f)
        {
            p_t1 = dot(_position - p_ray.getOrigin(), _normal) / den;
            p_t2 = p_t1;
        }

        return p_t1 > 0;
        // if t == 0 then ray is parallel to plane
    }
} // namespace RT_ISICG