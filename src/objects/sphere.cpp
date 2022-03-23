#include "Sphere.hpp"

namespace RT_ISICG
{
    bool Sphere::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {

        //if (intersectAny(p_ray, p_tMin, p_tMax))
        //{
        //    Vec3f compute_normal = _geometry.computeNormal(p_ray.pointAtT(t1));

        //    fillHitRecord(p_hitRecord, p_ray, compute_normal, t1);
        //}
        // todo make work
        float t1;
        float t2;
        if (_geometry.intersect(p_ray, t1, t2))
        {
            if (t1 > p_tMax) // first intersection too far
                return false;
            if (t1 < p_tMin) // first intersection too near, check second one
                t1 = t2;
            if (t1 < p_tMin || t1 > p_tMax) // not in range
                return false;

            Vec3f compute_normal = _geometry.computeNormal(p_ray.pointAtT(t1));

            fillHitRecord(p_hitRecord, p_ray, compute_normal, t1);

            return true;
        }
        return false;
    }
} // namespace RT_ISICG
