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
        Vec3f trace(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth) const
        {
            if (depth > _nbBounces)
                return BLACK;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                if (hitRecord._object->getMaterial()->isMirror())
                    return reflectRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord);

                if (hitRecord._object->getMaterial()->isTransparent())
                    return reflectRefractRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord);

                return directLighting(p_scene, p_ray, hitRecord);
            }
            return _backgroundColor;
        }

        float fresnel(const Vec3f &p_I, const Vec3f &p_N, const float n1 = 1.0f, const float n2 = 1.45f) const
        {
            float cosI = glm::dot(p_N, p_I); // cos of incoming

            float r = n1 / n2;
            float sinT = r * r * (1.0f - cosI * cosI); // sin of Transmitted (sin = 1-cos�)

            if (sinT > 1.0f) // no refraction
                return 1.0f;

            float cosT = glm::sqrt(glm::max(0.0f, 1.0f - sinT * sinT)); // cos of Transmitted
            cosI = glm::abs(cosI);

            float Rs = ((n2 * cosI) - (n1 * cosT)) / ((n2 * cosI) + (n1 * cosT));
            float Rp = ((n1 * cosI) - (n2 * cosT)) / ((n1 * cosI) + (n2 * cosT));

            return (Rs * Rs + Rp * Rp) / 2.0f;
        }

        Vec3f refractRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord, const float n1, const float n2, const float fresnelFactor) const
        {
            float r = n1 / n2;
            Vec3f refractedDirection = glm::refract(p_ray.getDirection(), hitRecord._normal, r);
            Ray refractedRay = Ray(hitRecord._point, refractedDirection);
            refractedRay.offset(-hitRecord._normal); // we always reflect in a different medium than the ray origin

            Vec3f transparentColor = hitRecord._object->getMaterial()->getFlatColor();
            return transparentColor * trace(p_scene, refractedRay, p_tMin, p_tMax, depth + 1);
        }

        Vec3f reflectRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord) const
        {
            Ray reflectedRay = Ray(hitRecord._point, glm::reflect(p_ray.getDirection(), -hitRecord._normal));
            reflectedRay.offset(hitRecord._normal); // we always reflect in the same medium as the ray origin

            Vec3f mirrorColor = hitRecord._object->getMaterial()->getFlatColor();
            return mirrorColor * trace(p_scene, reflectedRay, p_tMin, p_tMax, depth + 1);
        }

        Vec3f reflectRefractRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord) const
        {
            float n1 = 1.0f;
            float n2 = hitRecord._object->getMaterial()->getIOR();
            if (hitRecord._backFacing) // we already are inside the object, n2 should be ior of air
            {
                n1 = n2;
                n2 = 1.0f;
            }

            float fresnelFactor = fresnel(p_ray.getDirection(), hitRecord._trueNormal, n1, n2);
            Vec3f reflectedColor = reflectRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord);

            Vec3f refractedColor = BLACK;
            if (fresnelFactor <= 1.0f)
                refractedColor = refractRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord, n1, n2, fresnelFactor);

             //return Vec3f(hitRecord._backFacing);

            return glm::mix(refractedColor, reflectedColor, fresnelFactor);
        }

    private:
        int _nbBounces = 5;
    };
}

#endif // __RT_ISICG_WHITTED_INTEGRATOR__