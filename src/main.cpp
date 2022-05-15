#include "Renderer.hpp"
#include "cameras/PerspectiveCamera.hpp"
#include "defines.hpp"
#include "integrators/BaseIntegrator.hpp"

#include "Scene.hpp"
#include "scene_setup.hpp"

namespace RT_ISICG
{
    // Render Settings
    RenderSettings getSettings()
    {
        RenderSettings render_settings;

        render_settings.useHDR = true;
        render_settings.backgroundColor = GREY * 10.f;

        render_settings.integratorType = IntegratorType::PATH;
        render_settings.sampler = Sampler::GRID_SAMPLER;

        render_settings.samplesPerPixel = 32;
        render_settings.shadowSamples = 1;

        render_settings.maxBouncesTotal = 5;
        render_settings.maxBouncesTransmission = 5;
        render_settings.maxBounceReflection = 5;
        render_settings.maxBouncesDiffuse = 4;

        render_settings.tmax = 10000;

        return render_settings;
    }

    int main(int argc, char **argv)
    {
        // todo use argc and argv to load file

        // ============================ Image ================================ //
        // Output Image parameters
        const int imgWidth = 1200;
        const int imgHeight = 800;
        float aspectRatio = float(imgWidth) / imgHeight;
        Texture img = Texture(imgWidth, imgHeight); // Create a texture to render the scene.

        // ============================ Scene Init ============================== //
        Scene scene;
        SceneType sceneType = SceneType::TP6_Sponza; // <----- Change this to change the scene
        BaseCamera *camera = &scene.init(sceneType);

        if (camera == nullptr)
            camera = new PerspectiveCamera(aspectRatio);
        camera->setAspectRatio(aspectRatio);
        camera->displayCamera();

        // ============================ Render parameters ============================ //
        Renderer renderer;
        RenderSettings render_settings = getSettings();
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
