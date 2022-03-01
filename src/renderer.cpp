#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

#include "glm/gtx/string_cast.hpp"

namespace RT_ISICG
{
    Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

    void Renderer::setIntegrator(const IntegratorType p_integratorType)
    {
        if (_integrator != nullptr)
            delete _integrator;

        switch (p_integratorType)
        {
        case IntegratorType::RAY_CAST:
        default:
        {
            _integrator = new RayCastIntegrator();
            break;
        }
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

        #pragma opm parallel for
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                // no need to search the center of a pixel, as with AA a random value will be added from the top lef of each pixels
                float pixelSizeX = 1.f / (width - 1);
                float pixelSizeY = 1.f / (height - 1);

                float sx = i * pixelSizeX;
                float sy = j * pixelSizeY;
                float sz = 1.f;

                Vec3f color = VEC3F_ZERO;

                // Alaways sample the center of the pixel first
                float offsetX = pixelSizeX / 2;
                float offsetY = pixelSizeY / 2;

                /*AA loop*/
                for (int sample = 0; sample < _nbPixelSamples; sample++)
                {
                    /*
                    // "grid" AA // todo make work
                    offsetX = 1.0f / (sample + 1) / width;
                    offsetY = 1.0f / (sample + 1) / height;
                    // 1 -> (.5, .5)
                    // 2 -> (.25, .25) (.75, .75)
                    // 4 -> ...
                    */

                    Ray ray = p_camera->generateRay(sx + offsetX, sy + offsetY);
                    color += _integrator->Li(p_scene, ray, 0, 100);

                    // random AA
                    offsetY = pixelSizeY * randomFloat();
                    offsetX = pixelSizeX * randomFloat();
                }

                color *= 1.0f / _nbPixelSamples;

                // color = ray.getDirection();
                // color = (color + 1.0f) * 0.5f;
                // color.z = 0;
                // color.y = 0;

                p_texture.setPixel(i, j, color);
            }

            progressBar.next();
        }

        chrono.stop();
        progressBar.stop();

        return chrono.elapsedTime();
    }
} // namespace RT_ISICG
