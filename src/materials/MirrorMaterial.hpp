#ifndef __RT_ISICG_MIRROR_MATERIAL__
#define __RT_ISICG_MIRROR_MATERIAL__

#include "ColorMaterial.hpp"

namespace RT_ISICG
{
    class MirrorMaterial : public ColorMaterial
    {
    public:
        MirrorMaterial(const std::string &p_name, const Vec3f &p_diffuse)
            : ColorMaterial(p_name, p_diffuse){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            return _color * INV_PIf; // todo miror brdf, not use lightSample
        }

        bool isMirror() const override { return true; }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_MIRROR_MATERIAL__
