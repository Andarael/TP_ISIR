#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/BaseCamera.hpp"
#include "defines.hpp"
#include "integrators/BaseIntegrator.hpp"
#include "texture.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
    class Renderer
    {
    public:
        Renderer();

        ~Renderer();

        void setIntegrator(IntegratorType p_integratorType);

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
        void multiSample(const BaseCamera *p_camera, float sx, float sy, Vec3f &color, const Scene &p_scene, float pixelSizeY, float pixelSizeX) const;

    private:
        BaseIntegrator *_integrator = nullptr;
        int _nbPixelSamples = 1;
        bool _gridSampling = true;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
