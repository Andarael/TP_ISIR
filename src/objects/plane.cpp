#include "Plane.hpp"

namespace RT_ISICG
{
    /**
     * @brief fill hitrecord with first interesction if found between  tmin & tmax
     */
    bool Plane::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        float t;
        if (_geometry.intersect(p_ray, t, t))
        {
            if (!intersectionInRange(t, t, p_tMin, p_tMax))
                return false;

            Vec3f compute_normal = _geometry.computeNormal(p_ray.pointAtT(t));

            fillHitRecord(p_hitRecord, p_ray, compute_normal, t);

            return true;
        }
        return false;
    }
    bool Plane::intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        float t1, t2;
        if (_geometry.intersect(p_ray, t1, t2))
            return intersectionInRange(t1, t2, p_tMin, p_tMax);
        return false;
    }
} // namespace RT_ISICG
