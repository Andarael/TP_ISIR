#include "PlaneGeometry.hpp"

namespace RT_ISICG
{
    bool PlaneGeometry::intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
    {
        p_t1 = -1.f;
        p_t2 = -1.f;

        Vec3f rayDirection = p_ray.getDirection();
        Vec3f rayOrigin = p_ray.getOrigin();

        p_t1 = glm::dot(_position - rayOrigin, _normal) / glm::dot(rayDirection, _normal);

        p_t2 = p_t1;

        return p_t1 > 0;
        // si t == 0 then ray is parallel to plane
    }
} // namespace RT_ISICG