#ifndef __RT_ISICG_RAY_CAST_INTEGRATOR__
#define __RT_ISICG_RAY_CAST_INTEGRATOR__

#include "BaseIntegrator.hpp"

namespace RT_ISICG
{
    class RayCastIntegrator : public BaseIntegrator
    {
    public:
        IntegratorType getType() const override { return IntegratorType::RAY_CAST; }

        // Return incoming luminance.
        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            HitRecord hitRecord;
            if (p_scene.intersect(p_ray, p_ray.getTmin(), p_ray.getTmax(), hitRecord))
            {
                float alpha_threshold = 0.5f;
                alphaPassthrough(p_scene, p_ray, hitRecord, alpha_threshold);

                Vec3f color = hitRecord._object->getMaterial()->getFlatColor(hitRecord);
                Vec3f direction = glm::normalize(p_ray.getDirection());
                Vec3f normal = hitRecord._normal; // we assume normal is normalized
                float factor = glm::abs(glm::dot(normal, direction));
                return glm::max(color * factor, VEC3F_ZERO);
            }
            return _backgroundColor;
        }

        static void alphaPassthrough(const Scene &p_scene, const Ray &p_ray, HitRecord &hitRecord, float alpha_threshold)
        {
            if (alpha_threshold >= 1.f)
                return;

            while (hitRecord._object->getMaterial()->getAlpha(hitRecord._uv) < alpha_threshold)
            {
                HitRecord newHitrecord;

                Vec3f newOrigin = hitRecord._point;
                Vec3f direction = p_ray.getDirection();
                Ray newRay = Ray(newOrigin, direction, &p_ray);
                newRay._lightPath.tmax = p_ray.getTmax() - hitRecord._distance;
                newRay.offset(-hitRecord._normal);

                bool interection = p_scene.intersect(newRay, newRay.getTmin(), newRay.getTmax() - hitRecord._distance, newHitrecord);
                hitRecord = newHitrecord;
                if (!interection)
                    break;
            }
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_CAST_INTEGRATOR__
