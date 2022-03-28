#include "Renderer.hpp"
#include "cameras/PerspectiveCamera.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
    /**/
    int main(int argc, char **argv)
    {
        /* ==============================
         * ====== Render parameters =====
         * ============================== */
        // Output Image parameters
        const int imgWidth = 600;
        const int imgHeight = 400;

        // Render parameters
        const int samplesPerPixel = 1;
        const IntegratorType integratorType = IntegratorType::DIRECT_LIGHT;

        // Camera parameters
        Vec3f cameraPosition = Vec3f(0, 0, -2);
        Vec3f cameraLookAt = Vec3f(0, 0, 3);
        Vec3f worldUp = Vec3f(0, 1, 0);
        float aspectRatio = float(imgWidth) / imgHeight;

        /* ============================
         * ====== Initialization ======
         * ============================ */
        // Create and init scene.
        Scene scene;
        scene.init();

        // Create a texture to render the scene.
        Texture img = Texture(imgWidth, imgHeight);

        // Create a perspective camera.
        PerspectiveCamera camera(cameraPosition, cameraLookAt, worldUp, 60, aspectRatio);

        // Create and setup the renderer.
        Renderer renderer;
        renderer.setIntegrator(integratorType);
        renderer.setBackgroundColor(GREY);
        renderer.setNbPixelSamples(samplesPerPixel);

        /* =================================
         * ====== Rendering the image ======
         * ================================= */
        std::cout << "Rendering..." << std::endl;
        std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

        float renderingTime = renderer.renderImage(scene, &camera, img);

        std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

        /* ==============================
         * ====== Saving the image ======
         * ============================== */
        const std::string imgName = "image.jpg";
        img.saveJPG(RESULTS_PATH + imgName);

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
