#ifndef __RT_ISICG_BASE_MATERIAL__
#define __RT_ISICG_BASE_MATERIAL__

#include "HitRecord.hpp"
#include "Ray.hpp"
#include "lights/LightSample.hpp"

namespace RT_ISICG
{
    class BaseMaterial
    {
    public:
        BaseMaterial() = delete;

        BaseMaterial(const std::string &p_name)
            : _name(p_name){};

        virtual ~BaseMaterial() = default;

        virtual Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const = 0;

        virtual inline const Vec3f &getFlatColor() const = 0;

        virtual const std::string &getName() const final { return _name; }

        virtual Vec3f getEmit() const { return VEC3F_ZERO; }

        virtual float getIOR() const { return 1.f; }

        virtual bool isMirror() const { return false; }

        virtual bool isTransparent() const { return false; }

    protected:
        std::string _name;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_MATERIAL__
