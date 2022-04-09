#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "DirectLightingIntegrator.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
    class WhittedIntegrator : public DirectLightingIntegrator
    {

    public:
        WhittedIntegrator(const int p_shadowSamples, const int p_nbBounces)
            : DirectLightingIntegrator(p_shadowSamples), _nbBounces(p_nbBounces){};

        IntegratorType getType() const override
        {
            return IntegratorType::WHITTED;
        }

        Vec3f Li(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            return trace(p_scene, p_ray, p_tMin, p_tMax, 0);
        }

    protected:
        Vec3f trace(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth = 0) const
        {
            if (depth > _nbBounces)
                return BLACK;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                if (hitRecord._object->getMaterial()->isMirror())
                {
                    Ray reflectedRay = Ray(hitRecord._point, glm::reflect(p_ray.getDirection(), hitRecord._normal));
                    reflectedRay.offset(hitRecord._normal);
                    return trace(p_scene, reflectedRay, p_tMin, p_tMax, depth + 1);
                }
            }

            return DirectLightingIntegrator::Li(p_scene, p_ray, p_tMin, p_tMax);
        }

    private:
        int _nbBounces = 5;
    };
}

#endif // __RT_ISICG_WHITTED_INTEGRATOR__