#ifndef __RT_ISICG_ORENNAYAR_MATERIAL__
#define __RT_ISICG_ORENNAYAR_MATERIAL__

#include "BaseMaterial.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
    class MatteMaterial : public BaseMaterial
    {
    public:
        MatteMaterial(const std::string &p_name, const Vec3f &p_diffuse, const float p_roughness)
            : BaseMaterial(p_name), _brdf(p_diffuse, p_roughness){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            // return p_lightSample._direction;
            return _brdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection());
        }

        const Vec3f &getFlatColor() const override
        {
            return _brdf.getKd();
        }

    private:
        OrenNayarBRDF _brdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_ORENNAYAR_MATERIAL__