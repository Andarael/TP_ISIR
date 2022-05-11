

#ifndef __RT_ISICG_IMAGE__
#define __RT_ISICG_IMAGE__

#include "defines.hpp"
#include <string>
#include <vector>
#include <stb/stb_image.h>

namespace RT_ISICG
{
    class Texture
    {
    public:
        Texture() = delete;

        Texture(const int p_width, const int p_height, const int _nbChannels = 3)
            : _nbChannels(_nbChannels),
              _width(p_width), _height(p_height),
              // _pixels(_width * _height * _nbChannels, 0),
              _pixelsFloat(_width * _height * _nbChannels, 0.f)
        {
            // _pixels.shrink_to_fit();
            _pixelsFloat.shrink_to_fit();
        }

        Texture(const std::string &p_path)
        {
            stbi_uc *data = nullptr;
            data = stbi_load(p_path.c_str(), &_width, &_height, &_nbChannels, 0);
            if (data)
            {
                for (int i = 0; i < _width * _height * _nbChannels; i++)
                {
                    // _pixels[i] = data[i];
                    _pixelsFloat[i] = float(data[i]) / 255.f;
                }
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
        }

        Vec3f getPixel(const int p_i, const int p_j) const
        {
            assert(_nbChannels == 3);
            Vec3f pixel;
            int position = (p_i * _width + p_j) * _nbChannels;
            pixel.r = _pixelsFloat[position + 0];
            pixel.g = _pixelsFloat[position + 1];
            pixel.b = _pixelsFloat[position + 2];
            return pixel;
        }

        Vec3f &getPixel(const float p_x, const int p_y) const
        {
        }

        int getWidth() const { return _width; }

        int getHeight() const { return _height; }

        // std::vector<unsigned char> &getPixels() { return _pixels; }

        // const std::vector<unsigned char> &getPixels() const { return _pixels; }

        void setPixel(const int p_i, const int p_j, const Vec3f &p_color);

        void setPixel(const int p_i, const int p_j, const Vec4f &p_color);

        void saveJPG(const std::string &p_path, int p_quality = 100) const;

        void saveHDR(const std::string &p_path) const;

    private:
        int _nbChannels = 3;
        int _width;
        int _height;
        // std::vector<unsigned char> _pixels;
        std::vector<float> _pixelsFloat;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
