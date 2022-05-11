

#include "texture.hpp"
#include "io/stb_include.hpp"

namespace RT_ISICG
{
    void Texture::setPixel(const int p_i, const int p_j, const Vec3f &p_color)
    {
        assert(_nbChannels == 3);
        const int pixelId = (p_i + p_j * _width) * _nbChannels;
        _pixelsFloat[pixelId] = p_color.r;
        _pixelsFloat[pixelId + 1] = p_color.g;
        _pixelsFloat[pixelId + 2] = p_color.b;

        _pixelsFloat[pixelId] = p_color.r;
        _pixelsFloat[pixelId + 1] = p_color.g;
        _pixelsFloat[pixelId + 2] = p_color.b;
    }

    void Texture::setPixel(const int p_i, const int p_j, const Vec4f &p_color)
    {
        assert(_nbChannels == 4);
        const int pixelId = (p_i + p_j * _width) * _nbChannels;
        _pixelsFloat[pixelId] = p_color.r;
        _pixelsFloat[pixelId + 1] = p_color.g;
        _pixelsFloat[pixelId + 2] = p_color.b;
        _pixelsFloat[pixelId + 3] = p_color.b;
    }

    void Texture::saveJPG(const std::string &p_path, const int p_quality) const
    {

        std::vector<unsigned char> pixels(_width * _height * _nbChannels, 0);

#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < _height; ++i)
        {
            for (int j = 0; j < _width; ++j)
            {
                Vec3f color = getPixel(i, j);
                const int pixelId = (i * _width + j) * _nbChannels;
                pixels[pixelId] = static_cast<unsigned char>(color.r * 255);
                pixels[pixelId + 1] = static_cast<unsigned char>(color.g * 255);
                pixels[pixelId + 2] = static_cast<unsigned char>(color.b * 255);
                // todo gérer l'alpha (avec Vec4f)
                if (_nbChannels == 4) { pixels[pixelId + 3] = static_cast<unsigned char>(color.b * 255); }
            }
        }

        stbi_write_jpg(p_path.c_str(), _width, _height, _nbChannels, pixels.data(), p_quality);
    }

    void Texture::saveHDR(const std::string &p_path) const
    {
        stbi_write_hdr(p_path.c_str(), int(_width), _height, _nbChannels, _pixelsFloat.data());
    }
} // namespace RT_ISICG
