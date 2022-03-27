#ifndef __RT_ISICG_PLANE__
#define __RT_ISICG_PLANE__

#include "BaseObject.hpp"
#include "geometry/PlaneGeometry.hpp"

namespace RT_ISICG
{
    class Plane : public BaseObject
    {
    public:
        Plane() = delete;

        Plane(const std::string &p_name, const Vec3f &p_center, const Vec3f &p_normal)
            : BaseObject(p_name), _geometry(p_center, p_normal){};

        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override
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

        virtual bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            float t1, t2;
            if (_geometry.intersect(p_ray, t1, t2))
                return intersectionInRange(t1, t2, p_tMin, p_tMax);
            return false;
        }

    private:
        PlaneGeometry _geometry;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_PLANE__