#include "Renderer.hpp"
#include "integrators/RayCastIntegrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

#include "glm/gtx/string_cast.hpp"
#include "integrators/DirectLightingIntegrator.hpp"

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
            _integrator = new DirectLightingIntegrator();
            break;
        }
        case IntegratorType::COUNT:
            break;
        }
    }

    void Renderer::setBackgroundColor(const Vec3f &p_color)
    {
        if (_integrator == nullptr)
            std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl;
        else
            _integrator->setBackgroundColor(p_color);
    }

    float Renderer::renderImage(const Scene &p_scene, const BaseCamera *p_camera, Texture &p_texture)
    {
        const int width = p_texture.getWidth();
        const int height = p_texture.getHeight();

        Chrono chrono;
        ConsoleProgressBar progressBar;

        progressBar.start(height, 50);
        chrono.start();

        float pixelSizeX = 1.f / (width - 1);
        float pixelSizeY = 1.f / (height - 1);
#pragma omp parallel for
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                // no need to have the center of a pixel, as with AA a value will be added from the top lef of each pixels

                float sx = i * pixelSizeX;
                float sy = j * pixelSizeY;

                Vec3f color = VEC3F_ZERO; // todo return color in multisample

                multiSample(p_camera, sx, sy, color, p_scene, pixelSizeY, pixelSizeX, _nbPixelSamples);

                // color = ray.getDirection();
                // color = (color + 1.0f) * 0.5f;
                // color.z = 0;
                // color.y = 0;

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
        // gamma correction
        // color = glm::pow(color, Vec3f(1 / 2.2f));

        // color = Vec3f(0.5f, 0.5f, 1.f);
        // color = color * 0.5f + 0.5f;
        color = glm::clamp(color, 0.0f, 1.0f);
        return color;
    }

    void Renderer::multiSample(const BaseCamera *p_camera, float sx, float sy, Vec3f &color, const Scene &p_scene, const float pixelSizeY, float pixelSizeX, int nbPixelSamples) const
    {
        float offsetX;
        float offsetY;
        bool gridSampling = true; // todo move to hpp

        if (!gridSampling)
        {
            // Always sample the center of the pixel first
            offsetX = pixelSizeX * 0.5f;
            offsetY = pixelSizeY * 0.5f;
            for (int sample = 0; sample < nbPixelSamples; sample++)
            {
                Ray ray = p_camera->generateRay(sx + offsetX, sy + offsetY);
                color += _integrator->Li(p_scene, ray, 0, 1000);

                offsetY = pixelSizeY * randomFloat();
                offsetX = pixelSizeX * randomFloat();
            }
            color /= _nbPixelSamples;
        }
        else
        {
            for (int i = 1; i <= nbPixelSamples; i++)
            {
                for (int j = 1; j <= nbPixelSamples; j++)
                {
                    float subsampleX = (float(2 * i) - 1) / float(nbPixelSamples * 2);
                    float subsampleY = (float(2 * j) - 1) / float(nbPixelSamples * 2);

                    // random extremely slow here
                    offsetX = pixelSizeX * subsampleX;
                    offsetY = pixelSizeY * subsampleY;

                    Ray ray = p_camera->generateRay(sx + offsetX, sy + offsetY);
                    color += _integrator->Li(p_scene, ray, 0, FLT_INFINITY);
                }
            }
            color /= (nbPixelSamples * nbPixelSamples);
        }
    }
} // namespace RT_ISICG
