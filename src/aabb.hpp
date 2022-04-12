#ifndef __RT_ISICG_AABB__
#define __RT_ISICG_AABB__

#include "ray.hpp"

namespace RT_ISICG
{
    struct AABB
    {
    public:
        AABB() = default;

        AABB(const Vec3f &p_min, const Vec3f &p_max)
            : _min(p_min), _max(p_max){};

        ~AABB() = default;

        const Vec3f &getMin() const
        {
            return _min;
        }

        const Vec3f &getMax() const
        {
            return _max;
        }

        // Returns true if the AABB is degenerated.
        bool isValid() const
        {
            return ((_min.x <= _max.x) && (_min.y <= _max.y) && (_min.z <= _max.z));
        }

        // Extends the AABB with a point
        void extend(const Vec3f &p_point)
        {
            _min = glm::min(_min, p_point);
            _max = glm::max(_max, p_point);
        }

        // Extends the AABB with another AABB
        void extend(const AABB &p_aabb)
        {
            _min = glm::min(_min, p_aabb.getMin());
            _max = glm::max(_max, p_aabb.getMax());
        }

        // Returns the AABB diagonal vector.
        Vec3f diagonal() const
        {
            return _max - _min;
        }

        // Returns the AABB centroid.
        Vec3f centroid() const
        {
            return (_min + _max) * 0.5f;
        }

        // Returns the largest axis, 0 -> x, 1 -> y, 2 -> z
        size_t largestAxis() const
        {
            const Vec3f d = diagonal();
            if (d.x > d.y && d.x > d.z)
                return 0;
            if (d.y > d.z)
                return 1;
            return 2;
        }

        bool intersect(const Ray &p_ray, float p_tMin, float p_tMax) const
        {
            Vec3f origin = p_ray.getOrigin();
            Vec3f direction = p_ray.getDirection();

            // test Y plane intersection (up)
            Vec3f y_normal = Vec3f(0, 1, 0);
            float den = dot(y_normal, direction);

            float t1 = -1;
            float t2 = -1;

            if (den != 0.f)
                t2 = dot(_max - p_ray.getOrigin(), y_normal) / den;
                t2 = dot(_min - p_ray.getOrigin(), y_normal) / den;

            // intersection point of y top and y bottom
            Vec3f intersection1 = origin + direction * t1; // max y
            Vec3f intersection1 = origin + direction * t2; // min y

            // test if intersection point is inside the AABB
            if (intersection1.x > )
        }

    private:
        Vec3f _min = Vec3f(FLT_MAX);
        Vec3f _max = Vec3f(-FLT_MAX);
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_AABB__
