#include "Renderer.hpp"

#include "cameras/BaseCamera.hpp"
#include "glm/gtx/string_cast.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

#include "integrators/DebugIntegrator.hpp"
#include "integrators/DirectLightingIntegrator.hpp"
#include "integrators/RayCastIntegrator.hpp"
#include "integrators/WhittedIntegrator.hpp"

namespace RT_ISICG
{
    Renderer::Renderer()
        : _integrator(new RayCastIntegrator()){};

    Renderer::~Renderer()
    {
        delete _integrator;
    }

    void Renderer::setIntegrator(const IntegratorType p_integratorType)
    {
        delete _integrator;

        switch (p_integratorType)
        {
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
        case IntegratorType::DEBUG:
        {
            _integrator = new DebugIntegrator();
            break;
        }
        case IntegratorType::WHITTED:
        {
            _integrator = new WhittedIntegrator(_settings.shadowSamples, _settings.nbBounces);
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

        progressBar.start(height, 50);
        chrono.start();

        float pixelSizeX = 1.f / float(width - 1);
        float pixelSizeY = 1.f / float(height - 1);

#pragma omp parallel for schedule(dynamic)

        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
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

    Vec3f Renderer::colorTransform(Vec3f &color)
    {
        // gamma correction
        // color = glm::pow(color, Vec3f(1 / 2.2f));
        // color = color * 0.5f + 0.5f;

        // clamp // todo no clamp in hdr
        color = glm::clamp(color, 0.0f, 1.0f);
        return color;
    }

    Vec3f Renderer::multiSample(const BaseCamera *p_camera, float sx, float sy, const Scene &p_scene, float pixelSizeY, float pixelSizeX) const
    {
        Vec3f color = VEC3F_ZERO;
        int samplesPerPixel = _settings.samplesPerPixel;

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
                color += _integrator->Li(p_scene, ray, 0, FLT_INFINITY);

                Vec2f randomOffset = randomVec2f();
                offsetY = pixelSizeY * randomOffset.x;
                offsetX = pixelSizeX * randomOffset.y;
            }
            color /= samplesPerPixel;

            break;
        case Sampler::GRID_SAMPLER:
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
                    color += _integrator->Li(p_scene, ray, 0, FLT_INFINITY);
                }
            }
            color /= (samplesPerPixel * samplesPerPixel);
            break;
        }

        return color;
    }
} // namespace RT_ISICG
