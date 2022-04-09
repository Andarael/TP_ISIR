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

        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                Vec3f li = VEC3F_ZERO;
                LightList lights = p_scene.getLights();
                for (BaseLight *light : lights)
                {
                    int targetSamples = 1;
                    if (light->isSurface())
                        targetSamples = _nbLightSamples;

                    Vec3f lightContribution = VEC3F_ZERO;
                    for (int i = 0; i < targetSamples; i++)
                    {
                        Vec3f hitPoint = hitRecord._point;
                        LightSample lightSample = light->sample(hitPoint);
                        if (!_isShadow(p_scene, lightSample, hitPoint, hitRecord._normal))
                            lightContribution += _directLighting(hitRecord, lightSample, p_ray);
                    }
                    li += lightContribution / float(lights.size());
                }
                return li;
            }
            return _backgroundColor;
        }

    private:
        // todo multiple sample importance in light ?
        int _nbLightSamples = 1;

        // todo create only one light sample for double performances
        static bool _isShadow(const Scene &p_scene, const LightSample &lightSample, const Vec3f &point, const Vec3f &p_normal)
        {
            Vec3f direction = lightSample._direction;
            Ray ray = Ray(point, direction);
            ray.offset(p_normal);

            return p_scene.intersectAny(ray, 0, lightSample._distance);
        }

        Vec3f _directLighting(const HitRecord &hitRecord, const LightSample &lightSample, const Ray &p_ray) const
        {
            Vec3f sampleRadiance = lightSample._radiance;
            Vec3f materialColor = hitRecord._object->getMaterial()->shade(p_ray, hitRecord, lightSample);
            float factor = dot(lightSample._direction, hitRecord._normal);

            return max(VEC3F_ZERO, materialColor * factor * sampleRadiance);
        }
    };
}

#endif // __RT_ISICG_DIRECT_LIGHT_INT
