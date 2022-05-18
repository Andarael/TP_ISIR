#include "scene_setup.hpp"

#include "Renderer.hpp"
#include "Scene.hpp"
#include "defines.hpp"

// import materials
#include "materials/ColorMaterial.hpp"
#include "materials/CookTorranceMaterial.hpp"
#include "materials/EmissiveMaterial.hpp"
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
#include "materials/TextureMaterial.hpp"
#include "objects/BaseObject.hpp"
#include "objects/implicits/ImplicitBulb.hpp"
#include "objects/implicits/ImplicitJulia.hpp"
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
        scene._addMaterial(new MatteMaterial("BlueMatte", BLUE, 0.6f));
        scene._addMaterial(new MatteMaterial("GreyMatte", GREY, 0.6f));
        scene._addMaterial(new MatteMaterial("CyanMatte", CYAN, 0.6f));
        scene._addMaterial(new MatteMaterial("MagentaMatte", MAGENTA, 0.6f));

        scene._addMaterial(new MatteMaterial("lightBlueMatte", lightBlue, 0.3f));
        scene._addMaterial(new MatteMaterial("lightRedMatte", lightRed, 0.6f));
        scene._addMaterial(new MatteMaterial("lightYellowMatte", lightYellow, 0.6f));
        scene._addMaterial(new MatteMaterial("BeigeMatte", Vec3f(.99f, .8f, .6f), 0.3f));

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
        scene._addMaterial(new TransparentMaterial("TransparentLightRed", lightRed, 1.3f));

        // emissive
        scene._addMaterial(new EmissiveMaterial("EmissiveWhite", Vec3f(15)));

        // scene._addMaterial(new TextureMaterial("TextureWhite", "textures/white.png"));
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

    BaseCamera &setup_TP1(Scene &scene)
    {
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1.f));
        scene._addMaterial(new ColorMaterial("Blue", BLUE));
        scene._attachMaterialToObject("Blue", "Sphere1");

        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));
        scene._addMaterial(new ColorMaterial("Red", RED));
        scene._attachMaterialToObject("Red", "Plane1");

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera();
        return *camera;
    }

    BaseCamera &setup_TP7(Scene &scene)
    {
        // addCornellBox(scene, false, 32);

        scene._addObject(new Plane("Plane1", Vec3f(0, -1, 0), VEC3F_Y));
        scene._attachMaterialToObject("lightRedMatte", "Plane1");

        // scene._addObject(new ImplicitSphere("Sphere1", Vec3f(1, 0, 5), 0.5f));
        // scene._attachMaterialToObject("lightBlueMatte", "Sphere1");

        // scene._addObject(new ImplicitSphere("Sphere2", VEC3F_ZERO, 0.1f));
        // scene._attachMaterialToObject("GreenMatte", "Sphere2");

        scene._addObject(new ImplicitBulb("Bulb", Vec3f(0, 0, 3), 1.f));
        scene._attachMaterialToObject("lightBlueMatte", "Bulb");

        scene._addObject(new ImplicitJulia("Julia", Vec3f(0, 0, 0), 1.5f));
        scene._attachMaterialToObject("BeigeMatte", "Julia");

        scene._addObject(new ImplicitTorus("Torus", 3.f, 0.1f, Vec3f(0, 0, 1.5), 1.f));
        scene._attachMaterialToObject("PBR_Gold", "Torus");

        SimpleQuadLight *simple_quad = new SimpleQuadLight(WHITE, 50, 1.5, Vec3f(1, 5, -3));
        simple_quad->setLookAt(Vec3f(0, 0, 3));
        scene._addLight(simple_quad);

        // scene._addLight(new PointLight(WHITE, Vec3f(16, 16, 16), 1000));
        // scene._addLight(new PointLight(WHITE, Vec3f(16, 16, -16), 1000));
        // scene._addLight(new PointLight(WHITE, Vec3f(-16, 16, 16), 1000));
        // scene._addLight(new PointLight(WHITE, Vec3f(-16, 16, -16), 1000));
        // scene._addLight(new PointLight(WHITE, Vec3f(0, 0, -16), 1000));

        // SimpleQuadLight *quad_light = new SimpleQuadLight(WHITE, 70, 2, Vec3f(1, 8, -2));
        // quad_light->setLookAt(VEC3F_ZERO);
        // scene._addLight(quad_light);

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(-6, 3, 2), Vec3f(0, 0, 1.5), 40);
        return *camera;
    }

    BaseCamera &setup_TP6_Conference(Scene &scene)
    {
        // * ================ Sponza ==================
        scene.loadFileTriangleMesh("Conference", DATA_PATH + "/conference/", "conference.obj");

        // ================ light ================ //
        Vec3f u = Vec3f(0, 0, 300);
        Vec3f v = Vec3f(-800, 0, 0);
        Vec3f pos = Vec3f(900, 600, -300);
        QuadLight *quad_light = new QuadLight(WHITE, 25, pos, v, u);
        scene._addLight(quad_light);

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(-250, 500, 330), Vec3f(0, 350, 100));
        return *camera;
    }

    BaseCamera &setup_TP6_Sponza(Scene &scene)
    {
        // * ================ Conference ==================
        scene.loadFileTriangleMesh("Sponza", DATA_PATH + "/sponza/", "sponza.obj", VEC3F_ZERO, .01f);

        // ================ light ================ //
        Vec3f cameraPos = Vec3f(12, 2, 0);
        // scene._addLight(new PointLight(WHITE, cameraPos, 10));
        scene._addLight(new PointLight(Vec3f(1, 0.66f, 0), Vec3f(10, 2, 1.8), 2));
        scene._addLight(new PointLight(Vec3f(0, 0.33f, 1), Vec3f(10, 2, -1.3), 2));

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(cameraPos, Vec3f(0, 0, 0), 80);
        return *camera;
    }

    BaseCamera &setup_TP6(Scene &scene)
    {
        addCornellBox(scene, false, 20, Vec3f(0, 0, 0));

        // scene.loadFileTriangleMesh("UVsphere", DATA_PATH + "/CornellBox/CornellBox-Water.obj", VEC3F_ZERO, 10);

        // ================ Add Objects ================== //
        scene.loadFileTriangleMesh("bunny_lowpoly", DATA_PATH + "/bunny_lowpoly/", "bunny_lowpoly.obj",
                                   Vec3f(3, 1.25, -2), 1);

        scene.loadFileTriangleMesh("bunny", DATA_PATH, "bunny.obj",
                                   Vec3f(0, 1.25, 0), 1.0f, 180 - 45);

        scene.loadFileTriangleMesh("uvsphere", DATA_PATH, "uvsphere.obj",
                                   Vec3f(-5, 1, 0), 1);

        scene.loadFileTriangleMesh("cube_sphere", DATA_PATH, "cube_sphere.obj", Vec3f(5, 0.05, 5));

        scene.loadFileTriangleMesh("teapot", DATA_PATH, "teapot.obj",
                                   Vec3f(-4, 0, 6));

        // scene._attachMaterialToObject("CyanMatte", "Bunny_defaultobject");

        // ================ Add lights ================== //
        // scene._addLight(new PointLight(WHITE, Vec3f(3, 6, -6), 200));
        SimpleQuadLight *quad_light = new SimpleQuadLight(WHITE, 6, 4, Vec3f(5, 12, -5));
        quad_light->setLookAt(VEC3F_ZERO);
        scene._addLight(quad_light);

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(0, 3, -8), Vec3f(0, 0, 3));
        return *camera;
    }

    BaseCamera &setup_TP5(Scene &scene)
    {
        // ================ Add Base scene ================== //
        bool useQuadLight = true;
        bool full_mirror = false;
        float scale = 10;

        addCornellBox(scene, full_mirror, scale, Vec3f(0, -3, 0));

        // ================ Add Objects ================== //
        scene._addObject(new Sphere("Sphere1", Vec3f(-2, 0, 3), 1.5f));
        scene._addObject(new Sphere("Sphere2", Vec3f(2, 0, 3), 1.5f));
        scene._attachMaterialToObject("MirrorWhite", "Sphere1");
        scene._attachMaterialToObject("TransparentLightRed", "Sphere2");

        // scene.loadFileTriangleMesh("Bunny", DATA_PATH + "Bunny.obj", Vec3f(-2, 0, 3));
        // scene._attachMaterialToObject("MirrorLightRed", "Bunny_defaultobject");

        // scene.loadFileTriangleMesh("Bunny2", DATA_PATH + "Bunny.obj", Vec3f(2, 0, 3));
        // scene._attachMaterialToObject("TransparentWhite", "Bunny2_defaultobject");

        // ================ Add lights ================== //
        Vec3f lightPosition = Vec3f(0, 5, 0);
        SimpleQuadLight *quadLight = new SimpleQuadLight(WHITE, 40, 2, lightPosition);
        quadLight->setLookAt(Vec3f(0, 0, 3));
        PointLight *point_light = new PointLight(WHITE, lightPosition, 60.f);

        useQuadLight ? scene._addLight(quadLight) : scene._addLight(point_light);

        scene._addObject(new Sphere("SphereLight", Vec3f(3, 5, 3), .5f));
        scene._attachMaterialToObject("EmissiveWhite", "SphereLight");

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(0, 2, -8), Vec3f(0, 2, 0));
        return *camera;
    }

    BaseCamera &setup_TP4(Scene &scene)
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

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(0, 0, 0), Vec3f(0, 0, 3));
        return *camera;
    }

    BaseCamera &setup_TP3(Scene &scene)
    {
        // ================ Add lights ================== //
        Vec3f position = Vec3f(1, 10, 1);
        Vec3f u = Vec3f(-2, 0, 0);
        Vec3f v = Vec3f(0, 0, 2);
        QuadLight *quadLight = new QuadLight(WHITE, 40, position, u, v);
        quadLight->displayLight();

        // quadLight = new SimpleQuadLight(WHITE, 40, 2, Vec3f(0, 10, 0), -VEC3F_Y);
        // scene._addLight(quadLight);

        // ================ ADD MATERIALS ================ //
        scene._addMaterial(new ColorMaterial("Blue", BLUE));
        scene._addMaterial(new ColorMaterial("Red", RED));
        scene._addMaterial(new LambertMaterial("lambertWhite", WHITE));

        // ================ Add Objects ================== //
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1));
        // scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), VEC3F_Y));

        // Link objects and materials.
        scene._attachMaterialToObject("lambertWhite", "Sphere1");
        // scene._attachMaterialToObject("Red", "Plane1");

        // ================ Camera ================ //
        BaseCamera *camera = new PerspectiveCamera(Vec3f(0, 0, -2), Vec3f(0, 0, 3));
        return *camera;
    }

    BaseCamera &setup_scene(Scene &scene, const SceneType scene_type)
    {
        addMaterials(scene); // always add materials before objects

        switch (scene_type)
        {
        case SceneType::TP1:
        {
            return setup_TP1(scene);
        }
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

        case SceneType::TP6_Sponza:
            return setup_TP6_Sponza(scene);

        case SceneType::SCENE_TYPE_COUNT:

        default:
            break;
        }

        BaseCamera *camera = new PerspectiveCamera(Vec3f(0, 0, 0), Vec3f(0, 0, 3));
        return *camera;
    }
} // namespace RT_ISICG
