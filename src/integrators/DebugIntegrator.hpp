#ifndef __RT_ISICG_DEBUG_INTEGRATOR__
#define __RT_ISICG_DEBUG_INTEGRATOR__

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class DebugIntegrator : public BaseIntegrator
    {
    public:
        IntegratorType getType() const override { return IntegratorType::DEBUG; }

        static float zPass(const float z, const float min, const float max, const bool quadratic = true)
        {
            float depth = linearize_depth(z, min, max);
            if (quadratic)
                depth *= depth;
            return depth;
        }

        // Return incoming luminance.
        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            Vec3f output = _backgroundColor;

            Vec3f direction = normalize(p_ray.getDirection());
            Vec3f flatColor = _backgroundColor;
            Vec3f normal = VEC3F_ZERO;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                normal = hitRecord._normal;
                output = Vec3f(glm::dot(hitRecord._normal, -p_ray.getDirection()));
                output = hitRecord._point;
                flatColor = hitRecord._object->getMaterial()->getFlatColor();
            }
            return Vec3f(zPass(hitRecord._distance, 1.f, 5.f, false));
            return flatColor;
            return normal;
            return _setColorInRange(output);
        }

        static float linearize_depth(const float z, const float p_tmin, const float p_tmax)
        {
            return p_tmin * p_tmax / (z + p_tmax + p_tmin * (p_tmax - p_tmin));
        }

    private:
        static Vec3f _setColorInRange(Vec3f const p_color) { return (p_color + 1.f) / 2.f; }
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_CAST_INTEGRATOR__
#pragma once
