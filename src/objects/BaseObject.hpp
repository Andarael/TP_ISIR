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
            : _name(p_name)
        {
        }

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

        // Check for nearest intersection: if found fill p_hitRecord.
        virtual bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const = 0;

        void fillHitRecord(HitRecord &p_hitRecord, const Ray &p_ray, const Vec3f normal, const float distance) const
        {
            p_hitRecord._point = p_ray.pointAtT(distance);
            p_hitRecord._normal = normal;
            p_hitRecord.faceNormal(p_ray.getDirection());
            p_hitRecord._distance = distance;
            p_hitRecord._object = this;
        }

        virtual bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const = 0;

        /**
         * @brief check if intersection t1 and t2 are in range tmin tmax
         *
         * @param p_t1 first intersection point
         * @param p_t2 second intersection point
         * @param p_tMin minimum value of t
         * @param p_tMax maximum value of t
         * @return true if intersection points are in range, false otherwise
         */
        virtual bool intersectionInRange(float &p_t1, float &p_t2, float p_tMin, float p_tMax) const
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
