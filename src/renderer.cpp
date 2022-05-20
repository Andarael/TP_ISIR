#include "Renderer.hpp"

#include "cameras/BaseCamera.hpp"
#include "utils/chrono.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

// integrators
#include "integrators/DebugIntegrator.hpp"
#include "integrators/DirectLightingIntegrator.hpp"
#include "integrators/PathTracingIntegrator.hpp"
#include "integrators/RayCastIntegrator.hpp"
#include "integrators/WhittedIntegrator.hpp"

#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
    void printSettings(RenderSettings render_settings)
    {

        std::cout << " ===== Render Settings : ====== " << std::endl;
        std::cout << "    Seed: " << render_settings.seed << std::endl;
        std::cout << "    Use HDR: " << render_settings.useHDR << std::endl;
        std::cout << "    Tmax: " << render_settings.tmax << std::endl;
        std::cout << std::endl;
        std::cout << "    Sampler: " << int(render_settings.sampler) << std::endl;
        std::cout << "    Samples : " << render_settings.samples << std::endl;
        std::cout << "    Shadow samples: " << render_settings.shadowSamples << std::endl;
        std::cout << std::endl;
        std::cout << "    Max bounces total: " << render_settings.maxBouncesTotal << std::endl;
        std::cout << "    Max bounces transmission: " << render_settings.maxBouncesTransmission << std::endl;
        std::cout << "    Max bounces reflection: " << render_settings.maxBounceReflection << std::endl;
        std::cout << "    Max bounces diffuse: " << render_settings.maxBouncesDiffuse << std::endl;
        std::cout << std::endl;
        std::cout << "    Background color: " << glm::to_string(render_settings.backgroundColor) << std::endl;
        std::cout << "    Integrator: " << int(render_settings.integratorType) << std::endl;
        std::cout << " ============================== " << std::endl;
    }

    Renderer::Renderer()
        : _integrator(new RayCastIntegrator()) {}

    Renderer::Renderer(const RenderSettings p_settings) { setSettings(p_settings); };

    Renderer::~Renderer() { delete _integrator; }

    void Renderer::setIntegrator(const IntegratorType p_integratorType)
    {
        delete _integrator;

        switch (p_integratorType)
        {
        case IntegratorType::DEBUG:
        {
            _integrator = new DebugIntegrator();
            break;
        }
        case IntegratorType::RAY_CAST:
        {
            _integrator = new RayCastIntegrator();
            break;
        }
        case IntegratorType::DIRECT_LIGHT:
        {
            _integrator = new DirectLightingIntegrator(_settings.shadowSamples);
            break;
        }
        case IntegratorType::WHITTED:
        {
            _integrator = new WhittedIntegrator(_settings.shadowSamples,
                                                _settings.maxBouncesTransmission,
                                                _settings.maxBounceReflection,
                                                _settings.maxBouncesTotal);
            break;
        }
        case IntegratorType::PATH_TRACING:
        {
            _integrator = new PathTracingIntegrator(_settings.shadowSamples,
                                                    _settings.maxBouncesDiffuse,
                                                    _settings.maxBouncesTransmission,
                                                    _settings.maxBounceReflection,
                                                    _settings.maxBouncesTotal);
            break;
        }
        default:;
        }
    }

    void Renderer::setBackgroundColor(const Vec3f &p_color) const
    {
        if (_integrator == nullptr)
            std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl;
        else
            _integrator->setBackgroundColor(p_color);
    }

    float Renderer::renderImage(const Scene &p_scene, const BaseCamera *p_camera, Texture &p_texture) const
    {
        const int width = p_texture.getWidth();
        const int height = p_texture.getHeight();

        Chrono chrono;
        ConsoleProgressBar progressBar;

        float pixelSizeX = 1.f / float(width - 1);
        float pixelSizeY = 1.f / float(height - 1);

        progressBar.start(width, 50);
        chrono.start();

#pragma omp parallel for schedule(dynamic)

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                // no need to have the center of a pixel, as with AA a value will be added from the top lef of each pixels
                float sx = float(i) * pixelSizeX;
                float sy = float(j) * pixelSizeY;

                Vec3f color = multiSample(p_camera, sx, sy, p_scene, pixelSizeY, pixelSizeX);

                color = colorTransform(color);

                p_texture.setPixel(i, j, color);
            }
            progressBar.next();
        }

        chrono.stop();
        progressBar.stop();
        return chrono.elapsedTime();
    }

    Vec3f Renderer::colorTransform(Vec3f &color) const
    {
        float gamma = 1.f;
        if (!_settings.useHDR)
        {
            color = tonemapping(color);            // tonemapp to srgb
            color = glm::clamp(color, 0.0f, 1.0f); // no negative luminance & no
        }
        color = glm::pow(color, Vec3f(gamma));

        return color;
    }

    Vec3f Renderer::tonemapping(Vec3f &color)
    {
        // filmic curve (http://filmicworlds.com/blog/filmic-tonemapping-with-piecewise-power-curves/)
        float a = 2.5f;
        float b = 0.03f;
        float c = 2.4f;
        float d = 0.6f;
        float e = 0.15f;
        color = glm::min(color, 0.f);
        color *= (a * color + b) / (color * (c * color + d) + e);
        return color;
    }

    Vec3f Renderer::multiSample(const BaseCamera *p_camera, float sx, float sy, const Scene &p_scene, float pixelSizeY, float pixelSizeX) const
    {
        Vec3f color = VEC3F_ZERO;
        int samplesPerPixel = _settings.samples;

        // Always sample the center of the pixel first if sampler is random
        float offsetY = pixelSizeX * 0.5f;
        float offsetX = pixelSizeY * 0.5f;

        // switch sampler
        switch (_settings.sampler)
        {
        case Sampler::RANDOM_SAMPLER:
            for (int sample = 0; sample < samplesPerPixel; sample++)
            {
                Ray ray = p_camera->generateRay(sx + offsetX, sy + offsetY);
                ray._lightPath.tmin = 0;
                ray._lightPath.tmax = _settings.tmax;
                color += _integrator->Li(p_scene, ray);

                Vec2f randomOffset = randomVec2f();
                offsetY = pixelSizeY * randomOffset.x;
                offsetX = pixelSizeX * randomOffset.y;
            }
            color /= samplesPerPixel;

            break;
        case Sampler::GRID_SAMPLER:
            float spp2 = float(samplesPerPixel * samplesPerPixel);
            float factor = float(samplesPerPixel * 2);
            for (int i = 0; i < samplesPerPixel; i++)
            {
                for (int j = 0; j < samplesPerPixel; j++)
                {
                    float subsampleX = float(2 * i) / factor;
                    float subsampleY = float(2 * j) / factor;

                    Vec2f randomOffset = randomVec2f();
                    offsetY = pixelSizeY * subsampleY * randomOffset.x * 2.f;
                    offsetX = pixelSizeX * subsampleX * randomOffset.y * 2.f;

                    Ray ray = p_camera->generateRay(sx + offsetX, sy + offsetY);
                    ray._lightPath.tmin = 0;
                    ray._lightPath.tmax = _settings.tmax;
                    color += _integrator->Li(p_scene, ray);
                }
            }
            color /= spp2;
            break;
        }

        return color;
    }
} // namespace RT_ISICG
