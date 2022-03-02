#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/base_camera.hpp"
#include "defines.hpp"
#include "integrators/base_integrator.hpp"
#include "texture.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() { delete _integrator; }

        void setIntegrator(const IntegratorType p_integratorType);
        void setBackgroundColor(const Vec3f &p_color);

        inline void setNbPixelSamples(const int p_nbPixelSamples) { _nbPixelSamples = p_nbPixelSamples; }

        float renderImage(const Scene &p_scene, const BaseCamera *p_camera, Texture &p_texture);

        /**
         * @brief Render multiple sample per pixel and fill the given color
         *
         * @param p_camera
         * @param sx pixel pos in x
         * @param sy pixel pos in y
         * @param color color to fill
         * @param p_scene scene to render
         * @param pixelSizeY size of pixel in X
         * @param pixelSizeX size of pixel in Y
         * @param nbPixelSamples number of samples
         */
        void multiSample(const RT_ISICG::BaseCamera *p_camera, float sx, float sy, RT_ISICG::Vec3f &color, const RT_ISICG::Scene &p_scene, float pixelSizeY, float pixelSizeX, int nbPixelSamples);

    private:
        BaseIntegrator *_integrator = nullptr;
        int _nbPixelSamples = 1;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
