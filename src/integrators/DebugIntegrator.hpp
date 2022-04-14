#ifndef __RT_ISICG_DEBUG_INTEGRATOR__
#define __RT_ISICG_DEBUG_INTEGRATOR__

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class DebugIntegrator : public BaseIntegrator
    {
    public:
        IntegratorType getType() const override
        {
            return IntegratorType::DEBUG;
        }

        // Return incoming luminance.
        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            Vec3f output = _backgroundColor;

            Vec3f direction = normalize(p_ray.getDirection());

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                Vec3f normal = hitRecord._normal; // we assume normal is normalized
                output = Vec3f(glm::dot(hitRecord._normal, -p_ray.getDirection()));
                output = hitRecord._point;
                output = hitRecord._object->getMaterial()->getFlatColor();
                output = normal;
            }
            return output;
        }

    private:
        static Vec3f _setColorInRange(Vec3f const p_color)
        {
            return (p_color + 1.f) / 2.f;
        }
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_CAST_INTEGRATOR__
#pragma once
