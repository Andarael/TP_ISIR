#ifndef __RT_ISICG_PATH_INTEGRATOR__
#define __RT_ISICG_PATH_INTEGRATOR__

#include "defines.hpp"
#include "integrators/WhittedIntegrator.hpp"

namespace RT_ISICG
{
    class PathIntegrator : public WhittedIntegrator
    {

    public:
        PathIntegrator(const int p_shadowSamples, const int p_bouncesDiffuse, const int p_bouncestransmission, const int p_bouncesReflexion, const int p_maxBounces)
            : WhittedIntegrator(p_shadowSamples, p_bouncestransmission, p_bouncesReflexion, p_maxBounces),
              _maxDiffuseBounces(p_bouncesDiffuse){};

        IntegratorType getType() const override { return IntegratorType::PATH; }

        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            return trace(p_scene, p_ray);
        }

    protected:
        Vec3f bounceDiffuse(const Scene &scene, const Ray &p_ray, const HitRecord &hitRecord) const
        {
            Vec3f directlightColor = directLighting(scene, p_ray, hitRecord);

            if (p_ray._lightPath.depthDiffuse == _maxDiffuseBounces)
                return directlightColor;

            Vec3f bounceDirection = getRandomDirection(hitRecord._normal);
            Ray bounceRay = Ray(hitRecord._point, bounceDirection, RayType::diffuse, &p_ray);
            bounceRay.incrDiffuse();
            bounceRay.offset(hitRecord._normal);

            Vec3f materialColor = hitRecord._object->getMaterial()->shade(p_ray.getDirection(), hitRecord, bounceDirection);
            Vec3f emitColor = hitRecord._object->getMaterial()->getEmit();
            

            Vec3f bouncedColor = trace(scene, bounceRay);

            Vec3f col = VEC3F_ZERO;
            float pdf = INV_2PIf; // naive uniform sampling pdf
            float factor = glm::clamp(glm::dot(hitRecord._normal, bounceDirection), 0.f, 1.f) / pdf;
            col = bouncedColor * materialColor * factor;
            col += directlightColor;
            col += emitColor;
            return col;
        }

        Vec3f trace(const Scene &p_scene, const Ray &p_ray) const override
        {

            if (stopCondition(p_ray))
                return BLACK;

            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_ray.getTmin(), p_ray.getTmax(), hitRecord))
            {
                if (hitRecord._object->getMaterial()->isMirror())
                    return reflectRay(p_scene, p_ray, hitRecord);

                if (hitRecord._object->getMaterial()->isTransparent())
                    return reflectRefractRay(p_scene, p_ray, hitRecord);

                return bounceDiffuse(p_scene, p_ray, hitRecord);
            }
            return _backgroundColor;
        }

    public:
        int _maxDiffuseBounces = 5;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__