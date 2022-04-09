#include "Renderer.hpp"
#include "Scene.hpp"
#include "cameras/PerspectiveCamera.hpp"
#include "defines.hpp"
#include "scene_setup.hpp"

namespace RT_ISICG
{
    int main(int argc, char **argv)
    {
        /* ==============================
         * ====== Render parameters =====
         * ============================== */
        // Output Image parameters
        const int imgWidth = 1200;
        const int imgHeight = 800;
        float aspectRatio = float(imgWidth) / imgHeight;

        RenderSettings render_settings;
        render_settings.integratorType = IntegratorType::WHITTED;
        render_settings.sampler = Sampler::GRID_SAMPLER;
        render_settings.backgroundColor = GREY;
        render_settings.samplesPerPixel = 4;
        render_settings.shadowSamples = 4;
        render_settings.nbBounces = 256;

        /* ============================
         * ====== Initialization ======
         * ============================ */
        // Create and init scene.
        Scene scene;
        scene.init(SceneType::TP5);

        // Create a texture to render the scene.
        Texture img = Texture(imgWidth, imgHeight);

        // Create a perspective camera if scene didn't provide one.
        if (render_settings.camera == nullptr)
            render_settings.camera = new PerspectiveCamera(Vec3f(0, 2, -6), Vec3f(0, 2, 3), 60, aspectRatio);

        // Create and setup the renderer.
        Renderer renderer;
        renderer.setSettings(render_settings);

        /* =================================
         * ====== Rendering the image ======
         * ================================= */
        std::cout << "Rendering..." << std::endl;
        std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

        float renderingTime = renderer.renderImage(scene, render_settings.camera, img);

        std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

        /* ==============================
         * ====== Saving the image ======
         * ============================== */
        const std::string imgName = "image.jpg";
        const std::string imgNameHDR = "image.hdr";
        img.saveJPG(RESULTS_PATH + imgName);
        img.saveHDR(RESULTS_PATH + imgNameHDR);

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
