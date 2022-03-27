#ifndef __RT_ISICG_DIRECT_LIGHT_INTEGRATOR__
#define __RT_ISICG_DIRECT_LIGHT_INT

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class DirectLightingIntegrator : public BaseIntegrator
    {
    public:
        IntegratorType getType() const override
        {
            return IntegratorType::DIRECT_LIGHT;
        }

        // todo move to cpp
        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                Vec3f li = VEC3F_ZERO;
                LightList lights = p_scene.getLights();
                for (BaseLight *light : lights)
                {
                    Vec3f point = hitRecord._point;
                    LightSample lightSample = light->sample(point);
                    if (!isShadow(p_scene, lightSample, point))
                        li += _directLighting(hitRecord, lightSample);
                }
                return li;
            }
            return _backgroundColor;
        }

    private:
        // todo create only one light sample for double performances
        bool isShadow(const Scene &p_scene, const LightSample lightSample, Vec3f point) const
        {
            Vec3f direction = lightSample._direction;
            Ray ray = Ray(point, direction);
            ray.offset(ray.getDirection());

            return p_scene.intersectAny(ray, 0, lightSample._distance);
        }

        Vec3f _directLighting(const HitRecord hitRecord, LightSample lightSample) const
        {
            Vec3f sampleRadiance = lightSample._radiance;
            Vec3f materialColor = hitRecord._object->getMaterial()->getFlatColor();
            // todo use material.shade ?
            float factor = dot(lightSample._direction, hitRecord._normal);

            return materialColor * factor * sampleRadiance;
        }
    };
}

#endif // __RT_ISICG_DIRECT_LIGHT_INT

// 0.97