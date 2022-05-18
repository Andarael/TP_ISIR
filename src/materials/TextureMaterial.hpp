#ifndef __RT_ISICG_TEXTURE_MATERIAL__
#define __RT_ISICG_TEXTURE_MATERIAL__

#include "brdfs/lambert_brdf.hpp"
#include "materials/BaseMaterial.hpp"
#include "texture.hpp"

namespace RT_ISICG
{
    class TextureMaterial : public BaseMaterial
    {
    public:
        TextureMaterial(const std::string &p_name, const std::string &p_texture_path)
            : BaseMaterial(p_name) { _texture = new Texture(p_texture_path); }

        ~TextureMaterial() override { delete _texture; }

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            if (_texture == nullptr || _texture->getPixels().empty())
                return MAGENTA;

            Vec3f color = _texture->getPixel(p_hitRecord._uv);
            return color * _brdf.evaluate();
        }

        Vec3f getFlatColor(const HitRecord &p_hitRecord) const override
        {
            if (_texture == nullptr || _texture->getPixels().empty())
                return MAGENTA;

            return Vec3f(_texture->getPixel(p_hitRecord._uv));
        }

        bool hasTexture() const override { return true; }

        float getAlpha(const Vec2f &uv) const override { return _texture->getAlpha(uv); }

    protected:
        LambertBRDF _brdf;
        Texture *_texture = nullptr;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_TEXTURE_MATERIAL__
