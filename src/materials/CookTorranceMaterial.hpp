#ifndef __COOK_TORRANCE_MATERIAL__
#define __COOK_TORRANCE_MATERIAL__

#include "BaseMaterial.hpp"
#include "brdfs/CookTorranceBRDF.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
    class CookTorranceMaterial : public BaseMaterial
    {
    public:
        CookTorranceMaterial(const std::string &p_name, const Vec3f &p_color, const float p_metalness, const float p_roughness)
            : BaseMaterial(p_name),
              _metalness(p_metalness),
              _diffuseBrdf(p_color, p_roughness),
              _specularBrdf(p_color, p_roughness){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            Vec3f diffuse = _diffuseBrdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection());
            Vec3f specular = _specularBrdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection());
            return glm::mix(diffuse, specular, _metalness);
        }

        const Vec3f &getFlatColor() const override
        {
            return _diffuseBrdf.getKd();
        }

    private:
        float _metalness;
        OrenNayarBRDF _diffuseBrdf;
        CookTorranceBrdf _specularBrdf;
    };
}
#endif // __COOK_TORRANCE_MATERIAL__