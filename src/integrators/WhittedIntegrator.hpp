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
        Vec3f trace(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth = 0, const float currentIOR = 1.0f) const
        {
            if (depth > _nbBounces)
                return BLACK;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
            {
                if (hitRecord._object->getMaterial()->isMirror())
                    return reflectRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord, 1.0f);

                if (hitRecord._object->getMaterial()->isTransparent())
                    return reflectRefractRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord, currentIOR);

                return directLighting(p_scene, p_ray, hitRecord);
            }
            return _backgroundColor;
        }

        float fresnel(const Vec3f &wi, const Vec3f &N, float n1, float n2) const
        {
            float cosI = glm::dot(wi, N);
            Vec3f n = N;
            /*     if (cosI > 0.0f)
                 {
                     float tmp = n1;
                     n1 = n2;
                     n2 = tmp;
                 }*/

            float sinT = n1 / n2 * (1.0f - cosI * cosI);

            if (sinT > 1.0f)
                return 1.0f;

            float cosT = glm::sqrt(1.0f - sinT * sinT);
            cosI = glm::abs(cosI);

            float Rs = ((n2 * cosI) - (n1 * cosT)) / ((n2 * cosI) + (n1 * cosT));
            float Rp = ((n1 * cosI) - (n2 * cosT)) / ((n1 * cosI) + (n2 * cosT));

            return (Rs * Rs + Rp * Rp) / 2.0f;
        }

        Vec3f refractRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord, const float n1, const float n2, const float fresnelFactor) const
        {
            if (fresnelFactor > 1.f)
                return Vec3f(1, 0, 1);

            Vec3f wi = -p_ray.getDirection();

            bool isInside = glm::dot(wi, hitRecord._normal) < 0.0f;

            Vec3f normal = isInside ? -hitRecord._normal : hitRecord._normal;
            float r = isInside ? n1 / n2 : n2 / n1;

            Vec3f refractedDirection = glm::refract(-wi, normal, r);
            Ray refractedRay = Ray(hitRecord._point, refractedDirection);

            refractedRay.offset(-normal);

            Vec3f transparentColor = hitRecord._object->getMaterial()->getFlatColor();
            // return refractedDirection;
            return glm::max(VEC3F_ZERO, transparentColor * trace(p_scene, refractedRay, p_tMin, p_tMax, depth + 1, n2)); // todo why n2² ?
        }

        Vec3f reflectRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord, const float ior) const
        {
            Ray reflectedRay = Ray(hitRecord._point, glm::reflect(p_ray.getDirection(), hitRecord._normal));
            reflectedRay.offset(hitRecord._normal);
            Vec3f mirrorColor = hitRecord._object->getMaterial()->getFlatColor();
            return mirrorColor * trace(p_scene, reflectedRay, p_tMin, p_tMax, depth + 1, ior);
        }

        Vec3f reflectRefractRay(const Scene &p_scene, const Ray &p_ray, const float p_tMin, const float p_tMax, const int depth, const HitRecord &hitRecord, const float currentIOR) const
        {
            float n1 = currentIOR;
            float n2 = hitRecord._object->getMaterial()->getIOR();

            float fresnelFactor = fresnel(-p_ray.getDirection(), hitRecord._normal, n1, n2); // todo use wi = -ray.direction
            Vec3f reflectedColor = reflectRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord, n1);
            Vec3f refractedColor = refractRay(p_scene, p_ray, p_tMin, p_tMax, depth, hitRecord, n1, n2, fresnelFactor);

            // return Vec3f(fresnelFactor);

            return glm::mix(refractedColor, reflectedColor, fresnelFactor);
        }

    private:
        int _nbBounces = 5;
    };
}

#endif // __RT_ISICG_WHITTED_INTEGRATOR__