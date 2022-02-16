#include "sphere_geometry.hpp"

namespace RT_ISICG
{
    bool SphereGeometry::intersect(const Ray &p_ray, float &p_t1, float &p_t2) const
    {
        p_t1 = -1.f;
        p_t2 = -1.f;

        Vec3f direction = p_ray.getDirection();
        Vec3f origin = p_ray.getOrigin();

        float a = glm::dot(direction, direction);
        float b = 2.0f * glm::dot(direction, origin - _center);
        float c = glm::dot(origin - _center, origin - _center) - _radius * _radius;

        float delta = b * b - 4 * a * c;

        if (delta < 0)
            return false;

        p_t1 = (-b + glm::sqrt(delta)) / 2 * a;
        p_t2 = (-b - glm::sqrt(delta)) / 2 * a;

        if (p_t1 > p_t2)
        {
            float temp = p_t1;
            p_t1 = p_t2;
            p_t2 = temp;
        }

        return true;
    }

} // namespace RT_ISICG
