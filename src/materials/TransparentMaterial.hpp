#ifndef __RT_ISICG_TRANSPARENT_MATERIAL__
#define __RT_ISICG_TRANSPARENT_MATERIAL__

#include "BaseMaterial.hpp"

namespace RT_ISICG
{
    class TransparentMaterial : public BaseMaterial
    {
    public:
        TransparentMaterial(const std::string &p_name, const Vec3f &p_diffuse, const float p_ior = 1.45f)
            : BaseMaterial(p_name), _color(p_diffuse), _ior(p_ior){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            return _color * INV_PIf;
        }

        const Vec3f &getFlatColor() const override
        {
            return _color;
        }

        bool isTransparent() const override
        {
            return true;
        }

        float getIOR() const override
        {
            return _ior;
        }

    protected:
        Vec3f _color;
        float _ior = 1.3f;
    };
}

#endif // __RT_ISICG_TRANSPARENT_MATERIAL__
