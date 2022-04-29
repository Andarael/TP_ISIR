#ifndef __RT_ISICG_AABB__
#define __RT_ISICG_AABB__

#include "Ray.hpp"
#include "geometry/TriangleMeshGeometry.hpp"

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
            _min = glm::min(p_point, _min);
            _max = glm::max(p_point, _max);
        }

        // Extends the AABB with another AABB
        void extend(const AABB &p_aabb)
        {
            _min = glm::min(p_aabb.getMin(), _min);
            _max = glm::max(p_aabb.getMax(), _max);
        }

        void extend(const TriangleMeshGeometry &triangle)
        {
            Vec3f a = triangle.getVertex(0);
            Vec3f b = triangle.getVertex(1);
            Vec3f c = triangle.getVertex(2);
            extend(a);
            extend(b);
            extend(c);
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

        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax) const
        {
            Vec3f origin = p_ray.getOrigin();
            Vec3f invDirection = p_ray.getInvDirection();

            //direction.x = direction.x < 0 && direction.x > -EPSILON ? -direction.x : direction.x;

            float tmin = (_min.x - origin.x) * invDirection.x;
            float tmax = (_max.x - origin.x) * invDirection.x;

            if (tmin > tmax)
                std::swap(tmin, tmax);

            float tymin = (_min.y - origin.y) * invDirection.y;
            float tymax = (_max.y - origin.y) * invDirection.y;

            if (tymin > tymax)
                std::swap(tymin, tymax);

            if ((tmin > tymax) || (tymin > tmax))
                return false;

            if (tymin > tmin)
                tmin = tymin;

            if (tymax < tmax)
                tmax = tymax;

            float tzmin = (_min.z - origin.z) * invDirection.z;
            float tzmax = (_max.z - origin.z) * invDirection.z;

            if (tzmin > tzmax)
                std::swap(tzmin, tzmax);

            if ((tmin > tzmax) || (tzmin > tmax))
                return false;

            if (tzmin > tmin)
                tmin = tzmin;
            if (tzmax < tmax)
                tmax = tzmax;

            return (tmin < p_tMax && tmax > p_tMin);

            // todo implement https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
            // from http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.64.7663&rep=rep1&type=pdf
        }

    private:
        Vec3f _min = Vec3f(FLT_MAX);
        Vec3f _max = Vec3f(-FLT_MAX);
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_AABB__
