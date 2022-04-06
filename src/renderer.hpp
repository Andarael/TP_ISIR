#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/BaseCamera.hpp"
#include "defines.hpp"
#include "integrators/BaseIntegrator.hpp"
#include "texture.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
    // define sampler structure
    // todo integrate
    enum class Sampler
    {
        RANDOM_SAMPLER = 0,
        GRID_SAMPLER = 1
    };

    class Renderer
    {
    public:
        Renderer();

        ~Renderer();

        void setIntegrator(IntegratorType p_integratorType);

        void setSampler(Sampler p_sampler)
        {
            _sampler = p_sampler;
        }

        void setBackgroundColor(const Vec3f &p_color) const;

        void setNbPixelSamples(const int p_nbPixelSamples)
        {
            _nbPixelSamples = p_nbPixelSamples;
        }

        float renderImage(const Scene &p_scene, const BaseCamera *p_camera, Texture &p_texture) const;

        Vec3f colorTransform(Vec3f &color) const;

        /**
         * @brief Render multiple sample per pixel and fill the given color
         *
         * @param p_camera The camera to use
         * @param sx pixel pos in x
         * @param sy pixel pos in y
         * @param color color to fill with the average color of the samples
         * @param p_scene scene to render
         * @param pixelSizeY size of pixel in X
         * @param pixelSizeX size of pixel in Y
         */
        Vec3f multiSample(const BaseCamera *p_camera, float sx, float sy, const Scene &p_scene, const float pixelSizeY, float pixelSizeX) const;

    private:
        BaseIntegrator *_integrator = nullptr;
        Sampler _sampler = Sampler::RANDOM_SAMPLER;
        int _nbPixelSamples = 1;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
