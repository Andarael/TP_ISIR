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
            /// TODO
        }

        // Extends the AABB with another AABB
        void extend(const AABB &p_aabb)
        {
            /// TODO
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

        bool intersect(const Ray &p_ray, float p_tMin, float p_tMax) const;

    private:
        Vec3f _min = Vec3f(FLT_MAX);
        Vec3f _max = Vec3f(-FLT_MAX);
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_AABB__
