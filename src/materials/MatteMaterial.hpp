#ifndef __RT_ISICG_ORENNAYAR_MATERIAL__
#define __RT_ISICG_ORENNAYAR_MATERIAL__

#include "materials/BaseMaterial.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
    class MatteMaterial : public BaseMaterial
    {
    public:
        MatteMaterial(const std::string &p_name, const Vec3f &p_diffuse, const float p_roughness)
            : BaseMaterial(p_name), _diffuse(p_diffuse), _brdf(p_roughness){};

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            // todo brdf return color
            return _diffuse * _brdf.evaluate(p_hitRecord._normal, p_lightDirection, -p_rayDirection);
        }

        // todo passer un pointeur vers une fonction qui prend hitrecord (ou point), kd... et retourne une couleur

        const Vec3f &getFlatColor() const override { return _diffuse; }

    private:
        Vec3f _diffuse;
        OrenNayarBRDF _brdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_ORENNAYAR_MATERIAL__