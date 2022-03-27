#ifndef __RT_ISICG_SPHERE__
#define __RT_ISICG_SPHERE__

#include "BaseObject.hpp"
#include "geometry/SphereGeometry.hpp"

namespace RT_ISICG
{
    class Sphere : public BaseObject
    {
    public:
        Sphere() = delete;

        Sphere(const std::string &p_name, const Vec3f &p_center, const float p_radius)
            : BaseObject(p_name), _geometry(p_center, p_radius){};

        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override
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

        bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            float t1, t2;
            if (_geometry.intersect(p_ray, t1, t2))
                return intersectionInRange(t1, t2, p_tMin, p_tMax);
            return false;
        }

    private:
        SphereGeometry _geometry;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__
