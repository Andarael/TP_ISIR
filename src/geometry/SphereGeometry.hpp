#ifndef __RT_ISICG_SPHERE_GEOMETRY__
#define __RT_ISICG_SPHERE_GEOMETRY__

#include "BaseGeometry.hpp"

namespace RT_ISICG
{
    class SphereGeometry : public BaseGeometry
    {
    public:
        SphereGeometry() = delete;

        SphereGeometry(const Vec3f &p_center, const float p_radius)
            : BaseGeometry(p_center), _radius(p_radius){};

        float getRadius() const
        {
            return _radius;
        }

        bool intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
        {
            p_t1 = -1.f;
            p_t2 = -1.f;

            Vec3f rayDirection = p_ray.getDirection();
            Vec3f rayOrigin = p_ray.getOrigin();
            Vec3f OC = rayOrigin - _position;

            // td fig
            // float a = glm::dot(rayDirection, rayDirection) // because rayDirection is normalized = 1;
            float b = 2.0f * dot(rayDirection, OC);
            float c = dot(OC, OC) - (_radius * _radius);

            float delta = b * b - 4 * c;

            if (delta < 0)
                return false;

            float sqrtDelta = glm::sqrt(delta);

            p_t1 = (-b + sqrtDelta) * 0.5f;
            p_t2 = (-b - sqrtDelta) * 0.5f;
            // todo check interior of sphere

            // todo sort Intersection
            if (p_t1 > p_t2)
            {
                float temp = p_t1;
                p_t1 = p_t2;
                p_t2 = temp;
            }

            return true;
        }

        Vec3f computeNormal(const Vec3f &p_point) const
        {
            return glm::normalize(p_point - _position);
        }

    private:
        float _radius = 1.f;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE_GEOMETRY__
