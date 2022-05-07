#ifndef __RT_ISICG_LAMBERT_MATERIAL__
#define __RT_ISICG_LAMBERT_MATERIAL__

#include "materials/BaseMaterial.hpp"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
    class LambertMaterial : public BaseMaterial
    {
    public:
        LambertMaterial(const std::string &p_name, const Vec3f &p_diffuse)
            : BaseMaterial(p_name), _color(p_diffuse){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            return _color * _brdf.evaluate();
        }

        const Vec3f &getFlatColor() const override
        {
            return _color;
        }

    protected:
        Vec3f _color = GREY;
        LambertBRDF _brdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
