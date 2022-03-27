#include "Sphere.hpp"

namespace RT_ISICG
{
    bool Sphere::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        float t1;
        float t2;
        if (_geometry.intersect(p_ray, t1, t2))
        {
            if (!intersectionInRange(t1, t2, p_tMin, p_tMax))
                return false;

            Vec3f compute_normal = _geometry.computeNormal(p_ray.pointAtT(t1));

            fillHitRecord(p_hitRecord, p_ray, compute_normal, t1);

            return true;
        }
        return false;
    }
} // namespace RT_ISICG
