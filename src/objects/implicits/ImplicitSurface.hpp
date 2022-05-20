#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "acceleration_structures/aabb.hpp"
#include "objects/BaseObject.hpp"

// todo menger sponge
// todo torus
// todo Juila

// References
// Inigo Quilez www.iquilezles.org

namespace RT_ISICG
{
    class ImplicitSurface : public BaseObject
    {
    public:
        ImplicitSurface() = delete;

        ~ImplicitSurface() override = default;

        ImplicitSurface(const std::string &p_name)
            : BaseObject(p_name){};

        ImplicitSurface(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.f)
            : BaseObject(p_name, p_position), _scale(p_scale){};

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const override
        {
            if (_aabb.isValid() && !_aabb.intersect(p_ray, p_tMin, p_tMax))
                return false;

            int step = 0;
            double t = double(p_tMin); // distance from ray origin
            while (t < double(p_tMax))
            {
                Vec3f point = p_ray.pointAtT(float(t));
                double d = double(sdf(point)); // distance to surface

                if (d <= _minDistance)
                {
                    Vec3f normal = _evaluateNormal(point);
                    p_hitRecord.fill(p_ray, normal, float(t), this);
                    return true;
                }

                if (step > _maxSteps)
                    return false;

                step++;
                t = t + d;
            }
            return false;
        }

        // Check for any intersection between p_tMin and p_tMax.
        bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const override
        {

            // return _aabb.isValid() && _aabb.intersect(p_ray, p_tMin, p_tMax);
            if (_aabb.isValid() && !_aabb.intersect(p_ray, p_tMin, p_tMax))
                return false;

            int step = 0;
            double t = double(p_tMin); // distance from ray origin
            while (t < double(p_tMax))
            {
                Vec3f point = p_ray.pointAtT(float(t));
                double d = double(sdf(point)); // distance to surface

                if (d <= _minDistance)
                    return true;

                if (step > _maxSteps)
                    return false;

                step++;
                t = t + d;
            }
            return false;
        }

        // sdf with scale and translate
        virtual float sdf(const Vec3f &p_point) const final
        {
            Vec3f p = p_point;
            p -= _position;
            return _sdf(p / _scale) * _scale;
        }

    private:
        // Signed Distance Function on origin
        virtual float _sdf(const Vec3f &p) const = 0;

        // Evaluate normal by computing gradient at 'p_point'
        virtual Vec3f _evaluateNormal(const Vec3f &p_point) const
        {
            const float h = 1e-5f;

            Vec3f hx = h * VEC3F_X;
            Vec3f hy = h * VEC3F_Y;
            Vec3f hz = h * VEC3F_Z;

            return glm::normalize(Vec3f(sdf(p_point + hx) - sdf(p_point - hx),
                                        sdf(p_point + hy) - sdf(p_point - hy),
                                        sdf(p_point + hz) - sdf(p_point - hz)));

            // https://iquilezles.org/articles/normalsSDF/
            // todo h en fonction de la taille du pixel
        }

    private:
        int _maxSteps = 1000;
        const double _minDistance = 1e-6;

        // todo is it necessary to use double for distance ? (avoid inifinite loop when reaching far plane and going back)

    protected:
        float _scale = 1.f;
        AABB _aabb;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
