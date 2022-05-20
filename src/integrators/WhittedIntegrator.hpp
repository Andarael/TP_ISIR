#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "defines.hpp"
#include "integrators/DirectLightingIntegrator.hpp"
#include "utils/utils.hpp"

namespace RT_ISICG
{
    class WhittedIntegrator : public DirectLightingIntegrator
    {

    public:
        WhittedIntegrator(const int p_shadowSamples, const int p_bouncestransmission, const int p_bouncesReflexion, const int p_maxBounces)
            : DirectLightingIntegrator(p_shadowSamples),
              _maxBounces(p_maxBounces),
              _transmissionBounces(p_bouncestransmission),
              _reflexionBounces(p_bouncesReflexion) {};

        WhittedIntegrator(const int p_shadowSamples, const int p_maxBounces)
            : DirectLightingIntegrator(p_shadowSamples),
              _maxBounces(p_maxBounces),
              _transmissionBounces(p_maxBounces),
              _reflexionBounces(p_maxBounces) {};

        IntegratorType getType() const override { return IntegratorType::WHITTED; }

        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            return trace(p_scene, p_ray);
        }

    protected:
        virtual bool stopCondition(const Ray &p_ray) const
        {
            if (p_ray.getDepth() > _maxBounces)
                return true;
            if (p_ray._lightPath.depthReflected > _reflexionBounces)
                return true;
            if (p_ray._lightPath.depthTransmitted > _transmissionBounces)
                return true;

            return false;
        }

        virtual Vec3f trace(const Scene &p_scene, const Ray &p_ray) const
        {
            if (stopCondition(p_ray))
                return BLACK;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_ray.getTmin(), p_ray.getTmax(), hitRecord))
            {
                // check if ray can continue here instead
                if (hitRecord._object->getMaterial()->isMirror())
                    return reflectRay(p_scene, p_ray, hitRecord);

                if (hitRecord._object->getMaterial()->isTransparent())
                    return reflectRefractRay(p_scene, p_ray, hitRecord);

                return directLighting(p_scene, p_ray, hitRecord);
            }
            return _backgroundColor;
        }

        Vec3f refractRay(const Scene &p_scene, const Ray &p_ray, const HitRecord &hitRecord, const float n1, const float n2) const
        {
            // ray creation (spawns a relection ray and increase the depth)
            float r = n1 / n2;
            Vec3f refractedDirection = glm::refract(p_ray.getDirection(), hitRecord._normal, r);
            Ray refractedRay = Ray(hitRecord._point, refractedDirection, RayType::transmission, &p_ray);
            refractedRay.incrRefreact();
            refractedRay.offset(-hitRecord._normal); // we always reflect in a different medium than the ray origin

            // trace
            Vec3f transparentColor = hitRecord._object->getMaterial()->getFlatColor(hitRecord);
            return transparentColor * trace(p_scene, refractedRay);
        }

        Vec3f reflectRay(const Scene &p_scene, const Ray &p_ray, const HitRecord &hitRecord) const
        {
            // ray creation (spawns a transmission ray and increase the depth)
            Vec3f refractionDirection = glm::reflect(p_ray.getDirection(), -hitRecord._normal);
            Ray reflectedRay = Ray(hitRecord._point, refractionDirection, RayType::reflection, &p_ray);
            reflectedRay.incrRefrlect();
            reflectedRay.offset(hitRecord._normal); // we always reflect in the same medium as the ray origin

            // trace
            Vec3f mirrorColor = hitRecord._object->getMaterial()->getFlatColor(hitRecord);
            return mirrorColor * trace(p_scene, reflectedRay);
        }

        virtual Vec3f reflectRefractRay(const Scene &p_scene, const Ray &p_ray, const HitRecord &hitRecord) const
        {
            // setup n1 & n2
            float n1 = 1.0f;
            float n2 = hitRecord._object->getMaterial()->getIOR();
            if (hitRecord._backFacing) // we already are inside the object, n2 should be ior of air
                std::swap(n1, n2);

            float fresnelFactor = fresnel(p_ray.getDirection(), hitRecord._trueNormal, n1, n2);

            Vec3f reflectedColor = BLACK;
            reflectedColor = reflectRay(p_scene, p_ray, hitRecord);

            Vec3f refractedColor = BLACK;
            if (fresnelFactor <= 1.0f)
                refractedColor = refractRay(p_scene, p_ray, hitRecord, n1, n2);

            return glm::mix(refractedColor, reflectedColor, fresnelFactor);
        }

    protected:
        int _maxBounces = 5;

        int _transmissionBounces = 5;
        int _reflexionBounces = 5;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__