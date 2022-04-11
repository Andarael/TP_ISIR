#ifndef __RT_ISICG_BASE_OBJECT__
#define __RT_ISICG_BASE_OBJECT__

#include "HitRecord.hpp"
#include "Ray.hpp"
#include "defines.hpp"
#include "materials/BaseMaterial.hpp"
#include <vector>

namespace RT_ISICG
{
    class BaseObject
    {
    public:
        BaseObject(const std::string &p_name)
            : _name(p_name){};

        virtual ~BaseObject() = default;

        virtual const std::string &getName() const final
        {
            return _name;
        }

        virtual inline BaseMaterial *getMaterial() const final
        {
            return _material;
        }

        virtual inline void setMaterial(BaseMaterial *p_material) final
        {
            _material = p_material;
        }

        /**
         * @brief checks for intersection between ray and object, if found fills p_hitRecord
         *
         * @param p_ray ray to check for intersection
         * @param p_tMin minimum t value to check for intersection
         * @param p_tMax maximum t value to check for intersection
         * @param p_hitRecord hit-record to fill if intersection is found
         * @return true if intersection is found, false otherwise
         */
        virtual bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const = 0;

        /**
         * @brief fill given hitRecord with intersection information
         *
         * @param p_hitRecord hitRecord to fill
         * @param p_ray ray that intersected the object
         * @param normal normal of the object at the intersection point
         * @param distance distance from the ray origin to the intersection point
         */
        void fillHitRecord(HitRecord &p_hitRecord, const Ray &p_ray, const Vec3f normal, const float distance) const
        {
            // p_hitRecord.faceNormal(p_ray.getDirection());
            p_hitRecord._point = p_ray.pointAtT(distance);
            p_hitRecord._normal = normal;
            p_hitRecord._backFacing = dot(p_ray.getDirection(), normal) > 0.f;
            p_hitRecord._distance = distance;
            p_hitRecord._object = this;
        }

        virtual bool intersectAny(const Ray &p_ray, float p_tMin, float p_tMax) const = 0;

        /**
         * @brief check if intersection t1 and t2 are in range tmin tmax
         *
         * @param p_t1 first intersection point
         * @param p_t2 second intersection point
         * @param p_tMin minimum value of t
         * @param p_tMax maximum value of t
         * @return true if intersection points are in range, false otherwise
         */
        virtual bool intersectionInRange(float &p_t1, float &p_t2, const float p_tMin, const float p_tMax) const
        {
            sortIntersections(p_t1, p_t2);
            if (p_t1 > p_tMax) // first intersection too far
                return false;
            if (p_t1 < p_tMin) // first intersection too near, check second one
                p_t1 = p_t2;
            if (p_t1 < p_tMin || p_t1 > p_tMax) // not in range
                return false;
            return true;
        }

        virtual bool intersectionInRange(float const p_t, const float p_tMin, const float p_tMax) const
        {
            return (p_t < p_tMax && p_t > p_tMin); // first intersection too far
        }

        /**
         * @brief sort in place two intersections in ascending order
         *
         * @param p_t1 first intersection
         * @param p_t2 second intersection
         */
        virtual void sortIntersections(float &p_t1, float &p_t2) const
        {
            if (p_t1 > p_t2)
            {
                float temp = p_t1;
                p_t1 = p_t2;
                p_t2 = temp;
            }
        }

    protected:
        const std::string _name;
        BaseMaterial *_material = nullptr;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_OBJECT__
