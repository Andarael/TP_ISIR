#ifndef __RT_ISICG_MIRROR_MATERIAL__
#define __RT_ISICG_MIRROR_MATERIAL__

#include "BaseMaterial.hpp"

namespace RT_ISICG
{
    class MirrorMaterial : public BaseMaterial
    {
    public:
        MirrorMaterial(const std::string &p_name, const Vec3f &p_diffuse)
            : BaseMaterial(p_name), _color(p_diffuse){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            return BLACK;
        }

        const Vec3f &getFlatColor() const override
        {
            return BLACK;
        }

        const bool isMirror() const override
        {
            return true;
        }

        // todo mirror affect color

    protected:
        Vec3f _color;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_MIRROR_MATERIAL__
