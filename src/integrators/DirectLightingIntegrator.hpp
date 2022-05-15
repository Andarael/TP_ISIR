#ifndef __RT_ISICG_DIRECT_LIGHT_INTEGRATOR__
#define __RT_ISICG_DIRECT_LIGHT_INTEGRATOR__

#include "integrators/BaseIntegrator.hpp"

namespace RT_ISICG
{
    class DirectLightingIntegrator : public BaseIntegrator
    {
    public:
        DirectLightingIntegrator(const int p_shadowSamples)
            : BaseIntegrator(), _nbShadowSamples(p_shadowSamples){};

        IntegratorType getType() const override { return IntegratorType::DIRECT_LIGHT; }

        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_ray.getTmin(), p_ray.getTmax(), hitRecord))
                return directLighting(p_scene, p_ray, hitRecord);
            return _backgroundColor;
        }

    protected:
        Vec3f directLighting(const Scene &p_scene, const Ray &p_ray, HitRecord hitRecord) const
        {
            Vec3f li = VEC3F_ZERO;
            LightList lights = p_scene.getLights();
            for (BaseLight *light : lights)
            {
                int targetSamples = 1;
                if (light->isSurface())
                    targetSamples = _nbShadowSamples;

                Vec3f hitPoint = hitRecord._point;
                Vec3f lightContribution = VEC3F_ZERO;
                for (int i = 0; i < targetSamples; i++)
                {
                    LightSample lightSample = light->sample(hitPoint);
                    if (!_isShadow(p_scene, lightSample, hitPoint, hitRecord._normal))
                        lightContribution += shadeLighting(hitRecord, lightSample, p_ray);
                }
                li += lightContribution / float(targetSamples);
            }
            return li + hitRecord._object->getMaterial()->getEmit();
        }

    private:
        static bool _isShadow(const Scene &p_scene, const LightSample &lightSample, const Vec3f &point, const Vec3f &p_normal)
        {
            Vec3f direction = lightSample._direction;
            Ray ray = Ray(point, direction, RayType::shadow);
            ray.offset(p_normal);

            return p_scene.intersectAny(ray, SHADOW_EPSILON, lightSample._distance - SHADOW_EPSILON);
        }

        static Vec3f shadeLighting(const HitRecord &hitRecord, const LightSample &lightSample, const Ray &p_ray)
        {
            Vec3f sampleRadiance = lightSample._radiance;
            Vec3f materialColor = hitRecord._object->getMaterial()->shade(p_ray.getDirection(), hitRecord, lightSample._direction);
            Vec3f emissionColor = hitRecord._object->getMaterial()->getEmit();
            float factor = dot(lightSample._direction, hitRecord._normal);

            return max(VEC3F_ZERO, materialColor * factor * sampleRadiance);
        }

        int _nbShadowSamples;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_DIRECT_LIGHT_INTEGRATOR__
