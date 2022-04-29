#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "BaseObject.hpp"

namespace RT_ISICG
{
    class ImplicitSurface : public BaseObject
    {
    public:
        ImplicitSurface() = delete;

        virtual ~ImplicitSurface() = default;

        ImplicitSurface(const std::string &p_name)
            : BaseObject(p_name){};

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override
        {
            float t = 0.f;             // distance from ray origin
            float d = 0.f;             // distance to surface
            float D = p_tMax - p_tMin; // maximum traversal distance

            while (t < D)
            {
                Vec3f point = p_ray.pointAtT(t);
                d = _sdf(point);

                if (d < _minDistance)
                {
                    Vec3f normal = _evaluateNormal(point);
                    p_hitRecord.fill(p_ray, normal, t, this);
                    return true;
                }

                t = t + d;
            }
            return false;
        }

        // Check for any intersection between p_tMin and p_tMax.
        bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {
            // todo
            HitRecord hitRecord;
            return intersect(p_ray, p_tMin, p_tMax, hitRecord);
        }

    private:
        // Signed Distance Function
        virtual float _sdf(const Vec3f &p_point) const = 0;

        // Evaluate normal by computing gradient at 'p_point'
        virtual Vec3f _evaluateNormal(const Vec3f &p_point) const
        {
            const float h = _minDistance;

            Vec3f h100 = Vec3f(h, 0, 0);
            Vec3f h010 = Vec3f(0, h, 0);
            Vec3f h001 = Vec3f(0, 0, h);
            Vec3f p = p_point;
            return glm::normalize(Vec3f(_sdf(p + h100) - _sdf(p - h100),
                                        _sdf(p + h010) - _sdf(p - h010),
                                        _sdf(p + h001) - _sdf(p - h001)));
            // https://iquilezles.org/articles/normalsSDF/
        }

    private:
        const float _minDistance = 1e-6f;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
