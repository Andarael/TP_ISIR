#ifndef __RT_ISICG_BASE_GEOMETRY__
#define __RT_ISICG_BASE_GEOMETRY__

#include "HitRecord.hpp"
#include "Ray.hpp"

namespace RT_ISICG
{
    class BaseGeometry
    {
    public:

        BaseGeometry() = delete;

        explicit BaseGeometry(const Vec3f &p_position)
            : _position(p_position){};

        virtual ~BaseGeometry() = default;

        virtual Vec3f getPosition() { return _position; }

        virtual void setPosition(const Vec3f &p_position) { _position = p_position; }

    protected:
        Vec3f _position = VEC3F_ZERO;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_GEOMETRY__
