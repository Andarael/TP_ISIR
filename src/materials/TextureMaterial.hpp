#ifndef __RT_ISICG_TEXTURE_MATERIAL__
#define __RT_ISICG_TEXTURE_MATERIAL__

#include "brdfs/lambert_brdf.hpp"
#include "materials/BaseMaterial.hpp"
#include "texture.hpp"
#include "utils/utils.hpp"

namespace RT_ISICG
{
    class TextureMaterial : public BaseMaterial
    {
    public:
        TextureMaterial(const std::string &p_name, const std::string &p_texture_path, const std::string &p_texture_normal)
            : BaseMaterial(p_name)
        {
            _textureDiffuse = new Texture(p_texture_path);
            _textureNormal = new Texture(p_texture_normal);
        }

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            if (_textureDiffuse == nullptr || _textureDiffuse->getPixels().empty())
                return MAGENTA;

            Vec3f color = _textureDiffuse->getPixel(p_hitRecord._uv);
            return color * _brdf.evaluate();
        }

        Vec3f getFlatColor(const HitRecord &p_hitRecord) const override
        {
            if (_textureDiffuse == nullptr || _textureDiffuse->getPixels().empty())
                return MAGENTA;

            return Vec3f(_textureDiffuse->getPixel(p_hitRecord._uv));
        }

        bool hasTexture() const override { return true; }

        float getAlpha(const Vec2f &uv) const override { return _textureDiffuse->getAlpha(uv); }

        // Vec3f getNormal(const HitRecord &p_hitRecord) const override
        // {
            // if (_textureNormal == nullptr || _textureNormal->getPixels().empty())
                // return p_hitRecord._normal;

            // return _textureNormal->getPixel(p_hitRecord._uv);
        // }

    protected:
        LambertBRDF _brdf;
        Texture *_textureDiffuse = nullptr;
        Texture *_textureNormal = nullptr;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_TEXTURE_MATERIAL__
