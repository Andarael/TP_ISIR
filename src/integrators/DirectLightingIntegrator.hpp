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

        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                Vec3f li = VEC3F_ZERO;
                LightList lights = p_scene.getLights();
                for (BaseLight *light : lights)
                {
                    LightSample sample = light->sample(hitRecord._point);
                    Vec3f lightRadiance = sample._radiance;
                    Vec3f color = hitRecord._object->getMaterial()->getFlatColor();
                    Vec3f normal = hitRecord._normal;
                    float factor = dot(sample._direction, normal);

                    Vec3f temp = color * factor * lightRadiance;

                    li += temp; // todo calculer dans methode privée
                }
                return li;
            }
            return _backgroundColor;
        }
    };
}

#endif // __RT_ISICG_DIRECT_LIGHT_INT
