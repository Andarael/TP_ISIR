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

        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override;

    private:
        SphereGeometry _geometry;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__
