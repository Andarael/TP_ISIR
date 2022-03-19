#include "ray_cast_integrator.hpp"

namespace RT_ISICG
{
    Vec3f RayCastIntegrator::Li(const Scene &p_scene,
                                const Ray &p_ray,
                                const float p_tMin,
                                const float p_tMax) const
    {
        HitRecord hitRecord;
        if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
        {
            Vec3f color = hitRecord._object->getMaterial()->getFlatColor();

            Vec3f direction = glm::normalize(p_ray.getDirection());
            Vec3f normal = (hitRecord._normal); // we assume normal is normalized

            float factor = glm::dot(direction, normal);
            factor = glm::abs(factor);

            return color * factor;
        }
        else
        {
            return _backgroundColor;
        }
    }
} // namespace RT_ISICG
