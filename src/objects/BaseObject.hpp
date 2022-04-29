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

        virtual const std::string &getName() const final { return _name; }

        virtual BaseMaterial *getMaterial() const final { return _material; }

        virtual void setMaterial(BaseMaterial *p_material) final { _material = p_material; }

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
