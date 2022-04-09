#include "texture.hpp"
#include "io/stb_include.hpp"

namespace RT_ISICG
{
    void Texture::saveJPG(const std::string &p_path, const int p_quality) const
    {
        stbi_write_jpg(p_path.c_str(), int(_width), _height, _nbChannels, _pixels.data(), p_quality);
    }

    void Texture::saveHDR(const std::string &p_path) const
    {
        stbi_write_hdr(p_path.c_str(), int(_width), _height, _nbChannels, _pixelsFloat.data());
    }
} // namespace RT_ISICG
