#ifndef __RT_ISICG_RAY_CAST_INTEGRATOR__
#define __RT_ISICG_RAY_CAST_INTEGRATOR__

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class RayCastIntegrator : public BaseIntegrator
    {
    public:
        IntegratorType getType() const override { return IntegratorType::RAY_CAST; }

        // Return incoming luminance.
        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_ray.getTmin(), p_ray.getTmax(), hitRecord))
            {
                Vec3f color = hitRecord._object->getMaterial()->getFlatColor(hitRecord);
                Vec3f direction = glm::normalize(p_ray.getDirection());
                Vec3f normal = hitRecord._normal; // we assume normal is normalized
                float factor = glm::abs(glm::dot(normal, direction));
                return color * factor;
            }
            return _backgroundColor;
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_CAST_INTEGRATOR__
