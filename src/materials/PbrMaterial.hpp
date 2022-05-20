#ifndef __RT_ISICG_PBR_MATERIAL__
#define __RT_ISICG_PBR_MATERIAL__

#include "TextureMaterial.hpp"
#include "brdfs/CookTorranceBRDF.hpp"
#include "brdfs/OrenNayarBRDF.hpp"
#include "materials/BaseMaterial.hpp"
#include "texture.hpp"

namespace RT_ISICG
{
    class PbrMaterial : public BaseMaterial
    {
    public:
        // construct from texturemaps
        PbrMaterial(const std::string &p_name,
                    const std::string &p_texture_diff,
                    const std::string &p_texture_normal,
                    const std::string &p_texture_emit,
                    const std::string &p_texture_rough)
            : BaseMaterial(p_name), _diffuseBrdf(0.5f), _glossyBrdf(0.5f)
        {
            _textureDiffuse = new Texture(p_texture_diff);
            _textureNormal = new Texture(p_texture_normal);
            _textureEmission = new Texture(p_texture_emit);
            _textureRoughness = new Texture(p_texture_rough);
        }

        // construct from values
        PbrMaterial(const std::string &p_name, const Vec3f p_color, const Vec3f p_emit, const float p_roughness, const float p_metalness, const float p_ior = 1.45f)
            : BaseMaterial(p_name), _color(p_color), _emit(p_emit), _roughness(p_roughness), _metalness(p_metalness), _ior(p_ior){};

        // to set values in case texturemap is missing
        void setColor(const Vec3f &color) { _color = color; }
        void setEmit(const Vec3f &emit) { _emit = emit; }
        void setRoughness(const float roughness) { _roughness = roughness; }
        void setMetalness(const float metalness) { _metalness = metalness; }

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            float roughness = getRoughness(p_hitRecord);
            float metalness = _metalness;
            Vec3f color = getFlatColor(p_hitRecord);

            Vec3f normalForFresnel = glm::mix(p_hitRecord._normal, -p_rayDirection, glm::clamp(roughness, 0.f, 1.f));
            float fresnelFactor = fresnel(p_rayDirection, normalForFresnel, 1.f, _ior);

            if (_textureDiffuse == nullptr || _textureDiffuse->getPixels().empty())
                return MAGENTA;

            float metalMix = (1.f - fresnelFactor) * metalness;
            Vec3f glossyColor = glm::mix(WHITE, color, metalMix);

            Vec3f diffuse = VEC3F_ZERO;
            if (metalness != 1.0f)
                diffuse = color * _diffuseBrdf.evaluate(p_hitRecord._normal, p_lightDirection, -p_rayDirection, orenRoughness(roughness));

            Vec3f glossy = _glossyBrdf.evaluate(p_hitRecord._normal, p_lightDirection, -p_rayDirection, glossyColor, roughness);

            return glm::mix(diffuse, glossy, fresnelFactor);
        }

        Vec3f getFlatColor(const HitRecord &p_hitRecord) const override
        {
            if (_textureDiffuse == nullptr || _textureDiffuse->getPixels().empty())
                return _color;

            return Vec3f(_textureDiffuse->getPixel(p_hitRecord._uv));
        }

        bool hasTexture() const override { return true; }

        float getAlpha(const Vec2f &uv) const override { return _textureDiffuse->getAlpha(uv); }

        Vec3f getNormal(const HitRecord &p_hitRecord) const override
        {
            if (_textureNormal == nullptr || _textureNormal->getPixels().empty())
                return p_hitRecord._normal;

            return _textureNormal->getPixel(p_hitRecord._uv);
        }

        float getRoughness(const HitRecord &p_hitRecord) const
        {
            if (_textureRoughness == nullptr || _textureRoughness->getPixels().empty())
                return _roughness;

            return _textureRoughness->getPixel(p_hitRecord._uv).x;
        }

        Vec3f getEmit(const HitRecord &p_hitRecord) const override
        {
            if (_textureEmission == nullptr || _textureEmission->getPixels().empty())
                return _emit;

            return _textureEmission->getPixel(p_hitRecord._uv);
        }

        // convert UE4 sigma2 to oren roughness
        static float orenRoughness(const float roughness)
        {
            // todo maybe rough squared ?
            return glm::atan(roughness) * 0.707f; // atan(r)/sqrt(2)
        }

    protected:
        Texture *_textureDiffuse = nullptr;
        Texture *_textureNormal = nullptr;
        Texture *_textureEmission = nullptr;
        Texture *_textureRoughness = nullptr;

        Vec3f _color = Vec3f(1);
        Vec3f _emit = VEC3F_ZERO;
        float _roughness = 0.5f;
        float _metalness = 0.0f;
        float _ior = 1.45f;

        // brdfs
        OrenNayarBRDF _diffuseBrdf;
        CookTorranceBrdf _glossyBrdf;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_PBR_MATERIAL__
