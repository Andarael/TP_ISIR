#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "defines.hpp"

#include "brdfs/PhongBRDF.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "materials/BaseMaterial.hpp"

namespace RT_ISICG
{
    class PlasticMaterial : public BaseMaterial
    {
    public:
        PlasticMaterial(const std::string &p_name, const Vec3f &p_diffuse, const Vec3f &p_specular, const float p_shininess)
            : BaseMaterial(p_name),
              _diffuse(p_diffuse),
              _specular(p_specular),
              _phong_brdf(p_shininess){};

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            Vec3f diffuse = _diffuse * _lambert_brdf.evaluate();
            Vec3f specular = _specular * _phong_brdf.evaluate(p_hitRecord._normal, p_lightDirection, -p_rayDirection);

            return diffuse + specular;
        }

        Vec3f getFlatColor(const HitRecord &p_hitRecord) const override { return _diffuse; }

    private:
        Vec3f _diffuse = GREY;
        Vec3f _specular = WHITE;

        LambertBRDF _lambert_brdf;
        PhongBrdf _phong_brdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__