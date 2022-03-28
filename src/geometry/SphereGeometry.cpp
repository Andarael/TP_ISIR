#include "SphereGeometry.hpp"

namespace RT_ISICG
{
    bool SphereGeometry::intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
    {
        p_t1 = -1.f;
        p_t2 = -1.f;

        Vec3f rayDirection = p_ray.getDirection();
        Vec3f rayOrigin = p_ray.getOrigin();
        Vec3f OC = rayOrigin - _position;

        // cours - td fig
        // float a = glm::dot(rayDirection, rayDirection);
        // float a = 1; // because rayDirection is normalized
        float b = 2.0f * dot(rayDirection, OC);
        float c = dot(OC, OC) - (_radius * _radius);

        float delta = b * b - 4 * c;

        if (delta < 0)
            return false;

        float sqrtDelta = glm::sqrt(delta);

        p_t1 = (-b + sqrtDelta) * 0.5f;
        p_t2 = (-b - sqrtDelta) * 0.5f;
        // todo check interior of sphere

        if (p_t1 > p_t2)
        {
            float temp = p_t1;
            p_t1 = p_t2;
            p_t2 = temp;
        }

        return true;
    }

} // namespace RT_ISICG
