#ifndef __RT_ISICG_HIT_RECORD__
#define __RT_ISICG_HIT_RECORD__

#include "Ray.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
    class BaseObject; // todo utiliser la même technique que ça pour le scene_setup et le render_setup

    struct HitRecord
    {
        HitRecord() = default;

        HitRecord(const Vec3f &p_point, const Vec3f &p_normal, const float p_distance, const bool p_backFacing, const BaseObject *p_object)
            : _point(p_point), _normal(p_normal), _distance(p_distance), _backFacing(p_backFacing), _object(p_object){};


        // all filling methods

        void fill(const Ray p_ray, const Vec3f normal, const Vec3f trueNormal, const float distance)
        {
            _point = p_ray.pointAtT(distance);
            _normal = normal;
            _trueNormal = trueNormal;
            _distance = distance;
            _object = nullptr;
            faceNormal(p_ray.getDirection());
        }

        void fill(const Ray p_ray, const Vec3f normal, const Vec3f trueNormal, const float distance, const BaseObject *p_object)
        {
            fill(p_ray, normal, trueNormal, distance);
            _object = p_object;
        }

        void fill(const Ray p_ray, const Vec3f normal, const float distance, const BaseObject *p_object)
        {
            fill(p_ray, normal, normal, distance, p_object);
        }

        void fill(const Ray p_ray, const Vec3f normal, const float distance)
        {
            fill(p_ray, normal, normal, distance);
        }

        void faceNormal(const Vec3f p_direction)
        {
            _backFacing = glm::dot(p_direction, _normal) > 0.f;
            _normal = _backFacing ? -_normal : _normal;
        }

        Vec3f _point = VEC3F_ZERO;
        Vec3f _normal = VEC3F_ZERO;
        Vec3f _trueNormal = VEC3F_ZERO;
        float _distance = FLT_MAX;
        bool _backFacing = false;
        const BaseObject *_object = nullptr;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_HIT_RECORD__
