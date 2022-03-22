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
            : BaseObject(p_name), _geometry(p_center, p_radius)
        {
        }

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        virtual bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override;

        virtual bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            float t1, t2;
            if (_geometry.intersect(p_ray, t1, t2))
            {
                if (t1 > p_tMax) // first intersection too far
                    return false;
                if (t1 < p_tMin) // first intersection too near, check second one
                    t1 = t2;
                if (t1 < p_tMin || t1 > p_tMax) // not in range
                    return false;
                return true;
            }
            return false;
        }

    private:
        SphereGeometry _geometry;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__
