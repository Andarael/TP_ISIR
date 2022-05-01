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

        // todo the scene should be able to give its own camera and render settings
        RenderSettings render_settings;
        render_settings.integratorType = IntegratorType::WHITTED;
        render_settings.sampler = Sampler::GRID_SAMPLER;
        render_settings.backgroundColor = GREY;
        render_settings.samplesPerPixel = 2;
        render_settings.shadowSamples = 1;
        render_settings.nbBounces = 5;

        /* ============================
         * ====== Camera Settings =====
         * ============================ */
        // Create a perspective camera if scene didn't provide one.
        // if (render_settings.camera == nullptr)

        // cam conference
        render_settings.camera = new PerspectiveCamera(Vec3f(0, 2, 0), Vec3f(3, 2, 0), 60, aspectRatio);

        // cam TP5
        render_settings.camera = new PerspectiveCamera(Vec3f(0, 2, -6), Vec3f(0, 2, 3), 60, aspectRatio);

        // cam tp6
        Vec3f lookAt = Vec3f(0, 350, 100);
        Vec3f pos = Vec3f(-250, 500, 330);
        // render_settings.camera = new PerspectiveCamera(pos, lookAt, 60, aspectRatio);

        // cam TP7
        // bulb
        render_settings.camera = new PerspectiveCamera(Vec3f(0, 0, 1), Vec3f(0, 0, 3), 60, aspectRatio);

        // scene
        // render_settings.camera = new PerspectiveCamera(Vec3f(0, 0, -6), Vec3f(0, 0, 0), 30, aspectRatio);

        // far
        // render_settings.camera = new PerspectiveCamera(Vec3f(0, 8, -24), Vec3f(0, 0, 0), 20, aspectRatio);

        /* ============================
         * ====== Scene Init ==========
         * ============================ */
        // Create and init scene.
        Scene scene;
        scene.init(SceneType::TP7);

        // Create a texture to render the scene.
        Texture img = Texture(imgWidth, imgHeight);

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

        /* =================================
         * ====== Saving the image(s) ======
         * ================================= */
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
