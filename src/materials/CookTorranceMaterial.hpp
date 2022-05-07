#ifndef __COOK_TORRANCE_MATERIAL__
#define __COOK_TORRANCE_MATERIAL__

#include "materials/BaseMaterial.hpp"
#include "brdfs/CookTorranceBRDF.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
    class CookTorranceMaterial : public BaseMaterial
    {
    public:
        CookTorranceMaterial(const std::string &p_name, const Vec3f &p_diffuse, const float p_metalness, const float p_roughness)
            : BaseMaterial(p_name),
              _diffuse(p_diffuse),
              _metalness(p_metalness),
              _diffuseBrdf(p_roughness),
              _specularBrdf(p_roughness){};

        Vec3f shade(const Ray &p_ray, const HitRecord &p_hitRecord, const LightSample &p_lightSample) const override
        {
            Vec3f diffuse = _diffuse * _diffuseBrdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection());
            Vec3f specular = _diffuse * _specularBrdf.evaluate(p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection(), _diffuse);
            return glm::mix(diffuse, specular, _metalness);
        }

        const Vec3f &getFlatColor() const override { return _diffuse; }

    private:
        Vec3f _diffuse = GREY;
        float _metalness;
        OrenNayarBRDF _diffuseBrdf;
        CookTorranceBrdf _specularBrdf;
    };
}
#endif // __COOK_TORRANCE_MATERIAL__