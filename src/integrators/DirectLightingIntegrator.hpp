#ifndef __RT_ISICG_DIRECT_LIGHT_INTEGRATOR__
#define __RT_ISICG_DIRECT_LIGHT_INT

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class DirectLightingIntegrator : public BaseIntegrator
    {
    public:
        DirectLightingIntegrator()
            : BaseIntegrator()
        {
        }

        IntegratorType getType() const override
        {
            return IntegratorType::DIRECT_LIGHT; // todo add type  ?
        }

        // todo move top cpp
        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                Vec3f li = VEC3F_ZERO;
                LightList lights = p_scene.getLights();
                for (BaseLight *light : lights)
                    li += _directLighting(hitRecord, light);
                return li;
            }
            return _backgroundColor;
        }

    private:
        Vec3f _directLighting(const HitRecord hitRecord, const BaseLight *light) const
        {
            LightSample sample = light->sample(hitRecord._point);
            Vec3f lightRadiance = sample._radiance;
            Vec3f color = hitRecord._object->getMaterial()->getFlatColor();
            float factor = dot(sample._direction, hitRecord._normal);
            return color * factor * lightRadiance;
        }
    };
}

#endif // __RT_ISICG_DIRECT_LIGHT_INT
