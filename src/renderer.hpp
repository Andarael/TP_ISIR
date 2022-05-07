#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "defines.hpp"
#include "texture.hpp"

#include "cameras/BaseCamera.hpp"
#include "integrators/BaseIntegrator.hpp"

namespace RT_ISICG
{
    // forward declarations
    class Scene;

    // define sampler structure
    // todo integrate

    enum class Sampler
    {
        RANDOM_SAMPLER = 0,
        GRID_SAMPLER = 1
    };

    // define rendersettings structure
    struct RenderSettings
    {
        BaseCamera *camera = nullptr;
        IntegratorType integratorType = IntegratorType::RAY_CAST;
        Sampler sampler = Sampler::RANDOM_SAMPLER;
        Vec3f backgroundColor = GREY;
        int samplesPerPixel = 2;
        int shadowSamples = 1;
        int nbBounces = 5;
        float tmax = TMAX;
    };

    class Renderer
    {
    public:
        Renderer();

        Renderer(RenderSettings p_settings);

        ~Renderer();

        void setIntegrator(IntegratorType p_integratorType);

        void setSettings(const RenderSettings &p_settings)
        {
            _settings = p_settings;
            setIntegrator(p_settings.integratorType);
            setBackgroundColor(p_settings.backgroundColor);
        }

        void setBackgroundColor(const Vec3f &p_color) const;

        float renderImage(const Scene &p_scene, const BaseCamera *p_camera, Texture &p_texture) const;

        static Vec3f colorTransform(Vec3f &color);

        static Vec3f tonemapping(Vec3f &color);

        Vec3f multiSample(const BaseCamera *p_camera, float sx, float sy, const Scene &p_scene, float pixelSizeY, float pixelSizeX) const;

    private:
        RenderSettings _settings;
        BaseIntegrator *_integrator = nullptr;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
