#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "defines.hpp"

#include "BaseMaterial.hpp"
#include "brdfs/PhongBRDF.hpp"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
    class PlasticMaterial : public BaseMaterial
    {
    public:
        PlasticMaterial(const std::string &p_name, const Vec3f &p_diffuse, const float p_factor, const float p_shininess)
            : BaseMaterial(p_name), _factor(p_factor), _lambert_brdf(p_diffuse * p_factor), _phong_brdf((1.f - p_factor) * p_diffuse, p_shininess){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            Vec3f diffuse = _lambert_brdf.evaluate();
            Vec3f specular = _phong_brdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection());
            return diffuse + specular;
        }

        const Vec3f &getFlatColor() const override
        {
            return _lambert_brdf.getKd();
        }

    private:
        float _factor;
        LambertBRDF _lambert_brdf;
        PhongBrdf _phong_brdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__