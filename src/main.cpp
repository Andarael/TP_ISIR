#include "Renderer.hpp"
#include "cameras/PerspectiveCamera.hpp"
#include "defines.hpp"
#include "integrators/BaseIntegrator.hpp"

#include "Scene.hpp"
#include "scene_setup.hpp"

namespace RT_ISICG
{
    // Render Settings
    void useSettings(RenderSettings &render_settings) {
        render_settings.sampler = Sampler::GRID_SAMPLER;
        render_settings.backgroundColor = GREY;
        render_settings.samplesPerPixel = 2;
        render_settings.shadowSamples = 2;
        render_settings.maxBouncesTransmission = 5;
        render_settings.maxBouncesTotal = 5;
        render_settings.maxBounceReflection = 5;
        render_settings.maxBouncesDiffuse = 5;
        render_settings.tmax = 10000;
    }

    int main(int argc, char **argv)
    {
        // todo use argc and argv to load file

        bool overrideIntegrator = true;

        // ============================ Image ============================ //
        // Output Image parameters
        const int imgWidth = 1200;
        const int imgHeight = 800;
        float aspectRatio = float(imgWidth) / imgHeight;
        Texture img = Texture(imgWidth, imgHeight); // Create a texture to render the scene.

        // ============================ Scene Init ============================ //
        Scene scene;
        SceneType sceneType = SceneType::TP5; // <----- Change this to change the scene
        RenderSettings render_settings = scene.init(sceneType);

        // ============================ Render parameters ============================ //
        Renderer renderer;
        if (overrideIntegrator)
            render_settings.integratorType = IntegratorType::WHITTED;

        useSettings(render_settings);

        if (render_settings.camera == nullptr) // scene contain camera, not rendersettings
            render_settings.camera = new PerspectiveCamera(aspectRatio);

        render_settings.camera->setAspectRatio(aspectRatio);
        render_settings.camera->displayCamera();

        renderer.setSettings(render_settings);

        // ============================ Rendering the image ============================ //
        std::cout << "Rendering..." << std::endl;
        std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

        float renderingTime = renderer.renderImage(scene, render_settings.camera, img);

        std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

        // ============================ Saving the image(s) ============================ //
        const std::string imgName = "image.jpg";
        const std::string imgNameHDR = "image.hdr";
        img.saveJPG(RESULTS_PATH + imgName);
        // img.saveHDR(RESULTS_PATH + imgNameHDR);
        // todo add saving condition
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
