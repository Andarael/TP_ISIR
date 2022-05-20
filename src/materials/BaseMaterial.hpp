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

        virtual const std::string &getName() const final { return _name; }

        virtual Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const = 0;

        virtual Vec3f getFlatColor(const HitRecord &p_hitRecord) const = 0;

        virtual float getAlpha(const Vec2f &uv) const { return 1.f; }

        virtual Vec3f getEmit(const HitRecord &p_hitRecord) const { return VEC3F_ZERO; }

        virtual float getIOR() const { return 1.f; }

        virtual bool isMirror() const { return false; }

        virtual bool isTransparent() const { return false; }

        virtual bool hasTexture() const { return false; }

        // virtual Vec3f getNormal(const HitRecord &p_hitRecord) const { return Vec3f(1); }

    protected:
        std::string _name;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_MATERIAL__
