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
            : BaseObject(p_name), _geometry(p_center, p_normal)
        {
        }

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        virtual bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override;

    private:
        PlaneGeometry _geometry;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_PLANE__