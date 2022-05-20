#include "Renderer.hpp"
#include "cameras/PerspectiveCamera.hpp"
#include "defines.hpp"
#include "integrators/BaseIntegrator.hpp"
#include "utils/random.hpp"

#include "Scene.hpp"
#include "scene_setup.hpp"

namespace RT_ISICG
{
    // Render Settings
    RenderSettings getSettings()
    {
        RenderSettings render_settings;

        render_settings.seed = 0;
        render_settings.useHDR = false;
        render_settings.tmax = TMAX;

        render_settings.sampler = Sampler::GRID_SAMPLER;
        render_settings.samples = 1;
        render_settings.shadowSamples = 10;

        render_settings.maxBouncesTotal = 10;
        render_settings.maxBouncesTransmission = 8;
        render_settings.maxBounceReflection = 8;
        render_settings.maxBouncesDiffuse = 4;

        render_settings.backgroundColor = GREY * 1.0f;
        render_settings.integratorType = IntegratorType::PATH_TRACING;

        return render_settings;
    }

    int main(int argc, char **argv)
    {
        // todo use argc and argv to load file

        float resolution_multiplier = 1.f;
        // ============================ Image ================================ //
        // Output Image parameters
        const int imgWidth = 1200 * resolution_multiplier;
        const int imgHeight = 800 * resolution_multiplier;
        float aspectRatio = float(imgWidth) / float(imgHeight);
        Texture img = Texture(imgWidth, imgHeight); // Create a texture to render the scene.

        // ============================ Scene Init ============================== //
        Scene scene;
        SceneType sceneType = SceneType::APPLE; // <----- Change this to change the scene
        BaseCamera *camera = &scene.init(sceneType);

        if (camera == nullptr)
            camera = new PerspectiveCamera(aspectRatio);
        camera->setAspectRatio(aspectRatio);
        camera->displayCamera();

        // ============================ Render parameters ============================ //
        Renderer renderer;
        RenderSettings render_settings = getSettings();
        printSettings(render_settings);
        renderer.setSettings(render_settings);

        // ============================ Rendering the image ============================ //
        std::cout << "Rendering..." << std::endl;

        std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;
        float renderingTime = renderer.renderImage(scene, camera, img);
        std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

        // ============================ Saving the image(s) ============================ //
        const std::string imgName = "image.jpg";
        img.saveJPG(RESULTS_PATH + imgName);

        if (render_settings.useHDR)
        {
            const std::string imgNameHDR = "image.hdr";
            img.saveHDR(RESULTS_PATH + imgNameHDR);
        }

        // todo multiple canvas (direct, indirect etc ...)

        return EXIT_SUCCESS;
    }
} // namespace RT_ISICG

int main(const int argc, char **argv)
{
    try
    {
        return RT_ISICG::main(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught:" << std::endl
                  << e.what() << std::endl;
    }
}
