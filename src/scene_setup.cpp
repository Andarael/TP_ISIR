#include "scene_setup.hpp"

#include "Renderer.hpp"
#include "Scene.hpp"
#include "defines.hpp"

// import materials
#include "materials/ColorMaterial.hpp"
#include "materials/CookTorranceMaterial.hpp"
#include "materials/MatteMaterial.hpp"
#include "materials/MirrorMaterial.hpp"
#include "materials/PlasticMaterial.hpp"
#include "materials/TransparentMaterial.hpp"
#include "materials/lambertMaterial.hpp"

// import lights
#include "lights/BaseLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/QuadLight.hpp"
#include "lights/SimpleQuadLight.hpp"

// import objects
#include "cameras/PerspectiveCamera.hpp"
#include "objects/BaseObject.hpp"
#include "objects/implicits/ImplicitBulb.hpp"
#include "objects/implicits/ImplicitSphere.hpp"
#include "objects/implicits/ImplicitTorus.hpp"
#include "objects/parametrics/Plane.hpp"
#include "objects/parametrics/Sphere.hpp"

namespace RT_ISICG
{
    // todo unify light types constructor (pos, power, etc...)
    void addMaterials(Scene &scene)
    {
        scene._addMaterial(new MatteMaterial("WhiteMatte", WHITE, 0.6f));
        scene._addMaterial(new MatteMaterial("RedMatte", RED, 0.6f));
        scene._addMaterial(new MatteMaterial("GreenMatte", GREEN, 0.6f));
        scene._addMaterial(new MatteMaterial("lightBlueMatte", lightBlue, 0.6f));
        scene._addMaterial(new MatteMaterial("BlueMatte", BLUE, 0.6f));
        scene._addMaterial(new MatteMaterial("GreyMatte", GREY, 0.6f));
        scene._addMaterial(new MatteMaterial("CyanMatte", CYAN, 0.6f));
        scene._addMaterial(new MatteMaterial("MagentaMatte", MAGENTA, 0.6f));

        // Cook Torrence
        scene._addMaterial(new CookTorranceMaterial("PBR_Gold", gold, 1.f, 0.5f));

        // mirrors
        scene._addMaterial(new MirrorMaterial("MirrorLightBlue", lightBlue));
        scene._addMaterial(new MirrorMaterial("MirrorLightGreen", lightGreen));
        scene._addMaterial(new MirrorMaterial("MirrorLightRed", lightRed));
        scene._addMaterial(new MirrorMaterial("MirrorLightCyan", lightCyan));
        scene._addMaterial(new MirrorMaterial("MirrorLightMagenta", lightMagenta));
        scene._addMaterial(new MirrorMaterial("MirrorLightYellow", lightYellow));
        scene._addMaterial(new MirrorMaterial("MirrorWhite", WHITE));

        // transparent
        scene._addMaterial(new TransparentMaterial("TransparentWhite", WHITE, 1.3f));
        scene._addMaterial(new TransparentMaterial("TransparentLightBlue", lightBlue, 1.3f));
    }

    // Pseudo Cornell box made with infinite planes
    void addCornellBox(Scene &scene, const bool fullMirror, const float scale, const Vec3f &base)
    {
        scene._addObject(new Plane("PlaneGround", base, VEC3F_Y));
        scene._addObject(new Plane("PlaneCeiling", base + scale, -VEC3F_Y));

        scene._addObject(new Plane("PlaneLeft", Vec3f(base.x + scale * .5f, 0.f, 0.f), -VEC3F_X));
        scene._addObject(new Plane("PlaneRight", Vec3f(-scale * .5f + base.x, 0.f, 0.f), VEC3F_X));

        scene._addObject(new Plane("PlaneFront", Vec3f(0.f, 0.f, scale + base.z), -VEC3F_Z));
        scene._addObject(new Plane("PlaneBack", Vec3f(0.f, 0.f, -scale + base.z), VEC3F_Z));

        scene._attachMaterialToObject("GreyMatte", "PlaneGround");

        if (fullMirror)
        {
            scene._attachMaterialToObject("GreyMatte", "PlaneCeiling");
            scene._attachMaterialToObject("MirrorLightCyan", "PlaneLeft");
            scene._attachMaterialToObject("MirrorLightYellow", "PlaneRight");
            scene._attachMaterialToObject("MirrorWhite", "PlaneFront");
            scene._attachMaterialToObject("MirrorLightMagenta", "PlaneBack");
        }
        else
        {
            scene._attachMaterialToObject("GreenMatte", "PlaneCeiling");
            scene._attachMaterialToObject("RedMatte", "PlaneLeft");
            scene._attachMaterialToObject("BlueMatte", "PlaneRight");
            scene._attachMaterialToObject("MagentaMatte", "PlaneFront");
            scene._attachMaterialToObject("MirrorLightMagenta", "PlaneBack");
        }
    }

    RenderSettings setup_TP1(Scene &scene)
    {
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1.f));
        scene._addMaterial(new ColorMaterial("Blue", BLUE));
        scene._attachMaterialToObject("Blue", "Sphere1");

        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));
        scene._addMaterial(new ColorMaterial("Red", RED));
        scene._attachMaterialToObject("Red", "Plane1");

        // RENDER SETTINGS
        RenderSettings settings;
        settings.camera = new PerspectiveCamera(Vec3f(0, 0, 0), Vec3f(0, 0, 3));
        settings.integratorType = IntegratorType::RAY_CAST;
        return settings;
    }

    RenderSettings setup_TP7(Scene &scene)
    {
        addMaterials(scene);
        addCornellBox(scene, false, 32);

        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));
        scene._attachMaterialToObject("RedMatte", "Plane1");

        scene._addObject(new ImplicitSphere("Sphere1", Vec3f(1, 0, 5), 0.5f));
        scene._attachMaterialToObject("lightBlueMatte", "Sphere1");

        // scene._addObject(new ImplicitSphere("Sphere2", VEC3F_ZERO, 0.1f));
        // scene._attachMaterialToObject("GreenMatte", "Sphere2");

        scene._addObject(new ImplicitBulb("Bulb", Vec3f(0, 0, 3), 1.f));
        scene._attachMaterialToObject("PBR_Gold", "Bulb");

        scene._addObject(new ImplicitTorus("Torus", 1.5f, 0.25f, Vec3f(0, 0, 3), 1.f));
        scene._attachMaterialToObject("CyanMatte", "Torus");
        scene._attachMaterialToObject("TransparentWhite", "Torus");

        // SimpleQuadLight *simple_quad = new SimpleQuadLight(WHITE, 50, 1.5, Vec3f(1, 2, -3));
        // simple_quad->setLookAt(Vec3f(0, 0, 3));
        // scene._addLight(simple_quad);

        scene._addLight(new PointLight(WHITE, Vec3f(16, 16, 16), 1000));
        scene._addLight(new PointLight(WHITE, Vec3f(16, 16, -16), 1000));
        scene._addLight(new PointLight(WHITE, Vec3f(-16, 16, 16), 1000));
        scene._addLight(new PointLight(WHITE, Vec3f(-16, 16, -16), 1000));
        scene._addLight(new PointLight(WHITE, Vec3f(0, 0, -16), 1000));

        RenderSettings settings;
        settings.camera = new PerspectiveCamera(Vec3f(0, 3, 5), Vec3f(0, 0, 3));
        return settings;
    }

    RenderSettings setup_TP6_Conference(Scene &scene)
    {
        // * ================ Conference ==================
        scene.loadFileTriangleMesh("UVsphere", DATA_PATH + "/conference/conference.obj");
        // scene.loadFileTriangleMesh("UVsphere", DATA_PATH + "/sponza/sponza2.obj");
        Vec3f u = Vec3f(0, 0, 300);
        Vec3f v = Vec3f(-800, 0, 0);
        Vec3f pos = Vec3f(900, 600, -300);
        QuadLight *quad_light = new QuadLight(WHITE, 25, pos, v, u);
        scene._addLight(quad_light);
        // scene._addLight(new PointLight(WHITE, Vec3f(0, 2, 0), 60));
        // scene._addLight(new PointLight(WHITE, Vec3f(0, 2, 0), 60));

        // SimpleQuadLight *simpleQuadLight = new SimpleQuadLight(WHITE, 20, 2, Vec3f(0, 2, 0));
        // simpleQuadLight->setLookAt(Vec3f(-3, 2, 1));
        // scene._addLight(simpleQuadLight);
        // scene._addLight(new PointLight(WHITE, pos, 6000000));

        RenderSettings settings;
        settings.integratorType = IntegratorType::WHITTED;
        settings.camera = new PerspectiveCamera(Vec3f(-250, 500, 330), Vec3f(0, 350, 100));
        return settings;
    }

    RenderSettings setup_TP6(Scene &scene)
    {
        addMaterials(scene);
        addCornellBox(scene, false, 20, Vec3f(0, 0, 0));

        // ================ Add Objects ================== //
        scene.loadFileTriangleMesh("bunny_lowpoly", DATA_PATH + "bunny_lowpoly.obj", Vec3f(3, 1.17, -2));

        scene.loadFileTriangleMesh("bunny", DATA_PATH + "bunny.obj", Vec3f(0, 1.17, 0));

        scene.loadFileTriangleMesh("uvsphere", DATA_PATH + "uvsphere.obj", Vec3f(-5, 1, 0));

        scene.loadFileTriangleMesh("cube_sphere", DATA_PATH + "cube_sphere.obj", Vec3f(5, 0.05, 5));

        scene.loadFileTriangleMesh("bunny", DATA_PATH + "teapot.obj", Vec3f(-3, 0, 6));

        // scene._attachMaterialToObject("CyanMatte", "Bunny_defaultobject");

        // ================ Add lights ================== //
        // scene._addLight(new PointLight(WHITE, Vec3f(3, 6, -6), 200));
        SimpleQuadLight *quad_light = new SimpleQuadLight(WHITE, 6, 4, Vec3f(5, 12, -5));
        quad_light->setLookAt(VEC3F_ZERO);
        scene._addLight(quad_light);

        RenderSettings settings;
        settings.integratorType = IntegratorType::WHITTED;
        settings.camera = new PerspectiveCamera(Vec3f(0, 3, -8), Vec3f(0, 0, 3));
        return settings;
    }

    RenderSettings setup_TP5(Scene &scene)
    {
        // ================ Add Base scene ================== //
        bool useQuadLight = true;
        addMaterials(scene);
        addCornellBox(scene, true, 15, Vec3f(0, -3, 0));

        // ================ Add Objects ================== //
        // scene._addObject(new Sphere("Sphere1", Vec3f(-2, 0, 3), 1.5f));
        // scene._addObject(new Sphere("Sphere2", Vec3f(2, 0, 3), 1.5f));
        // scene._attachMaterialToObject("MirrorLightRed", "Sphere1");
        // scene._attachMaterialToObject("TransparentWhite", "Sphere2");

        scene.loadFileTriangleMesh("Bunny", DATA_PATH + "Bunny.obj", Vec3f(-2, 0, 3));
        scene._attachMaterialToObject("MirrorLightRed", "Bunny_defaultobject");

        scene.loadFileTriangleMesh("Bunny2", DATA_PATH + "Bunny.obj", Vec3f(2, 0, 3));
        scene._attachMaterialToObject("TransparentWhite", "Bunny2_defaultobject");

        // ================ Add lights ================== //
        Vec3f lightPosition = Vec3f(0, 5, 0);
        SimpleQuadLight *quadLight = new SimpleQuadLight(WHITE, 40, 2, lightPosition);
        quadLight->setLookAt(Vec3f(0, 0, 3));
        PointLight *point_light = new PointLight(WHITE, lightPosition, 200.f);
        useQuadLight ? scene._addLight(quadLight) : scene._addLight(point_light);

        // ================ RENDER SETTINGS ================ //
        RenderSettings settings;
        settings.integratorType = IntegratorType::WHITTED;
        settings.camera = new PerspectiveCamera(Vec3f(0, 2, -8), Vec3f(0, 2, 0));
        return settings;
    }

    RenderSettings setup_TP4(Scene &scene)
    {
        bool useQuadlight = false;

        // ================ Add lights ================== //
        Vec3f lightPosition = Vec3f(0, 0, -2);
        PointLight *pointLight = new PointLight(WHITE, lightPosition, 60);

        SimpleQuadLight *quadLight = new SimpleQuadLight(WHITE, 2, 4, Vec3f(3, 6, -3));
        quadLight->setLookAt(Vec3f(0, 0, 3));

        useQuadlight ? scene._addLight(quadLight) : scene._addLight(pointLight);

        // ================ Add Materials ================== //
        scene._addMaterial(new LambertMaterial("Grey", GREY));
        scene._addMaterial(new LambertMaterial("Red", RED));
        scene._addMaterial(new MatteMaterial("Matte_Grey", GREY, 1.f));
        scene._addMaterial(new PlasticMaterial("Plastic_Grey", GREY * 0.7f, WHITE * 0.3f, 64.f)); // 70% grey, 30% specular
        scene._addMaterial(new PlasticMaterial("Plastic_RED", RED, WHITE, 16.f));
        scene._addMaterial(new CookTorranceMaterial("PBR_Gold", Vec3f(1.f, 0.85f, 0.57f), 0.5f, 0.3f));

        // ================ Add Objects ================== //
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1));
        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));

        // attach materials to objects
        scene._attachMaterialToObject("PBR_Gold", "Sphere1");
        scene._attachMaterialToObject("Red", "Plane1");

        // ================ RENDER SETTINGS ================ //
        RenderSettings settings;
        settings.integratorType = IntegratorType::DIRECT_LIGHT;
        settings.camera = new PerspectiveCamera(Vec3f(0, 0, 0), Vec3f(0, 0, 3));
        return settings;
    }

    RenderSettings setup_TP3(Scene &scene)
    {
        // ================ Add lights ================== //
        Vec3f position = Vec3f(1, 10, 1);
        Vec3f u = Vec3f(-2, 0, 0);
        Vec3f v = Vec3f(0, 0, 2);
        QuadLight *quadLight = new QuadLight(WHITE, 40, position, u, v);
        quadLight->displayLight();

        // quadLight = new SimpleQuadLight(WHITE, 40, 2, Vec3f(0, 10, 0), -VEC3F_Y);
        scene._addLight(quadLight);

        // ================ ADD MATERIALS ================ //
        scene._addMaterial(new ColorMaterial("Blue", BLUE));
        scene._addMaterial(new ColorMaterial("Red", RED));

        // ================ Add Objects ================== //
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1));
        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));

        // Link objects and materials.
        scene._attachMaterialToObject("Blue", "Sphere1");
        scene._attachMaterialToObject("Red", "Plane1");

        // ================ RENDER SETTINGS ================ //

        RenderSettings settings;
        settings.integratorType = IntegratorType::DIRECT_LIGHT;
        settings.camera = new PerspectiveCamera(Vec3f(0, 0, -2), Vec3f(0, 0, 3));
        return settings;
    }

    RenderSettings setup_scene(Scene &scene, const SceneType scene_type)
    {
        addMaterials(scene); // always add materials before objects
        switch (scene_type)
        {
        case SceneType::TP1:
            return setup_TP1(scene);

        case SceneType::TP2:
            break;

        case SceneType::TP3:
            return setup_TP3(scene);

        case SceneType::TP4:
            return setup_TP4(scene);

        case SceneType::TP5:
            return setup_TP5(scene);

        case SceneType::TP6:
            return setup_TP6(scene);

        case SceneType::TP6_Conference:
            return setup_TP6_Conference(scene);

        case SceneType::TP7:
            return setup_TP7(scene);

        case SceneType::SCENE_TYPE_COUNT:
            break;

        default:
            break;
        }

        RenderSettings settings;
        settings.camera = new PerspectiveCamera(Vec3f(0, 0, 0), Vec3f(0, 0, 3));
        return settings;
    }
} // namespace RT_ISICG
