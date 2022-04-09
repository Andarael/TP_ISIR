#ifndef __RT_ISICG_SCENE_SETUP__
#define __RT_ISICG_SCENE_SETUP__

#include "Scene.hpp"
#include "defines.hpp"

// import materials
#include "materials/ColorMaterial.hpp"
#include "materials/CookTorranceMaterial.hpp"
#include "materials/MatteMaterial.hpp"
#include "materials/PlasticMaterial.hpp"
#include "materials/lambertMaterial.hpp"

// import lights
#include "lights/BaseLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/QuadLight.hpp"
#include "lights/SimpleQuadLight.hpp"

// import objects
#include "objects/BaseObject.hpp"
#include "objects/Plane.hpp"
#include "objects/Sphere.hpp"

namespace RT_ISICG
{
    static void setup_TP5(Scene &scene)
    {
        /* ==============================================
         * ================ Add Materials ===============
         * ============================================== */
        scene._addMaterial(new MatteMaterial("WhiteMatte", WHITE, 0.6f));
        scene._addMaterial(new MatteMaterial("RedMatte", RED, 0.6f));
        scene._addMaterial(new MatteMaterial("GreenMatte", GREEN, 0.6f));
        scene._addMaterial(new MatteMaterial("BlueMatte", BLUE, 0.6f));
        scene._addMaterial(new MatteMaterial("GreyMatte", GREY, 0.6f));
        scene._addMaterial(new MatteMaterial("MagentaMatte", MAGENTA, 0.6f));

        /* ==============================================
         * ================ Add Objects =================
         * ============================================== */

        // Spheres .
        scene._addObject(new Sphere("Sphere1", Vec3f(-2.f, 0.f, 3.f), 1.5f));
        scene._addObject(new Sphere("Sphere2", Vec3f(2.f, 0.f, 3.f), 1.5f));
        scene._attachMaterialToObject("WhiteMatte", "Sphere1");
        scene._attachMaterialToObject("WhiteMatte", "Sphere2");

        // Pseudo Cornell box made with infinite planes .
        scene._addObject(new Plane("PlaneGround", Vec3f(0.f, -3.f, 0.f), Vec3f(0.f, 1.f, 0.f)));
        scene._addObject(new Plane("PlaneLeft", Vec3f(5.f, 0.f, 0.f), Vec3f(-1.f, 0.f, 0.f)));
        scene._addObject(new Plane("PlaneCeiling", Vec3f(0.f, 7.f, 0.f), Vec3f(0.f, -1.f, 0.f)));
        scene._addObject(new Plane("PlaneRight", Vec3f(-5.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f)));
        scene._addObject(new Plane("PlaneFront", Vec3f(0.f, 0.f, 10.f), Vec3f(0.f, 0.f, -1.f)));
        scene._attachMaterialToObject("GreyMatte", "PlaneGround");
        scene._attachMaterialToObject("RedMatte", "PlaneLeft");
        scene._attachMaterialToObject("GreenMatte", "PlaneCeiling");
        scene._attachMaterialToObject("BlueMatte", "PlaneRight");
        scene._attachMaterialToObject("MagentaMatte", "PlaneFront");

        /* ==============================================
         * ================ Add lights ==================
         * ============================================== */
        //scene._addLight(new PointLight(WHITE, Vec3f(0.f, 5.f, 0.f), 400.f)); // todo unify light types constructor (pos, power, etc...)
        Vec3f pos = Vec3f(1.f, 5.f, -2.f);
         scene._addLight(new QuadLight(WHITE, 40.f, pos, Vec3f(-2.f, 0.f, 0.f), Vec3f(0.f, 1.f, 2.f)));
    }

    static void setup_TP3(Scene &scene)
    {
        /* ==============================================
         * ================ Add lights ==================
         * ============================================== */
        // Point lights;
        Vec3f position = Vec3f(1, 10, 1);
        PointLight *pointLight = new PointLight(WHITE, position, 100);

        // quad light
        position = Vec3f(1, 10, 1);
        Vec3f u = Vec3f(-2, 0, 0);
        Vec3f v = Vec3f(0, 0, 2);
        QuadLight *quadLight = new QuadLight(WHITE, 40, position, u, v);
        quadLight->displayLight();
        quadLight = new SimpleQuadLight(WHITE, 40, 4, position, Vec3f(0, -1, 0));

        // dual point light
        position = Vec3f(-2, 7, -2);
        PointLight *light2 = new PointLight(WHITE, position, 50);

        //_addLight(light2);
        //_addLight(pointLight);
        scene._addLight(quadLight);

        /* ==============================================
         * ================ Add Materials ===============
         * ============================================== */
        scene._addMaterial(new ColorMaterial("Blue", BLUE));
        scene._addMaterial(new ColorMaterial("Red", RED));

        /* ==============================================
         * ================ Add Objects =================
         * ============================================== */
        // Add objects.
        Vec3f planeNormal = Vec3f(0, 1, 0);
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1));
        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), planeNormal));

        // Link objects and materials.
        scene._attachMaterialToObject("Blue", "Sphere1");
        scene._attachMaterialToObject("Red", "Plane1");
    }

    static void setup_TP4(Scene &scene)
    {
        /* ==============================================
         * ================ Add lights ==================
         * ============================================== */
        Vec3f lightPosition = Vec3f(1, 1, -2);
        PointLight *pointLight = new PointLight(WHITE, lightPosition, 60);
        SimpleQuadLight *quadLight = new SimpleQuadLight(WHITE, 10, 4, lightPosition, Vec3f(1));
        quadLight->setLookAt(Vec3f(0, 0, 3));
        scene._addLight(quadLight);

        /* ==============================================
         * ================ Add Materials ===============
         * ============================================== */
        scene._addMaterial(new LambertMaterial("Grey", GREY));
        scene._addMaterial(new LambertMaterial("Red", RED));
        scene._addMaterial(new MatteMaterial("Matte_Grey", GREY, 1.f));
        scene._addMaterial(new PlasticMaterial("Plastic_Grey", GREY, 0.3f, 8.f, false));
        scene._addMaterial(new PlasticMaterial("Plastic_Grey_additive", GREY, 0.3f, 8.f, true));
        scene._addMaterial(new CookTorranceMaterial("PBR_Gold", Vec3f(1.f, 0.85f, 0.57f), 0.5f, 0.3f));

        /* ==============================================
         * ================ Add Objects =================
         * ============================================== */
        scene._addObject(new Sphere("Sphere1", Vec3f(0, 0, 3), 1));
        scene._addObject(new Plane("Plane1", Vec3f(0, -2, 0), Vec3f(0, 1, 0)));
        scene._attachMaterialToObject("PBR_Gold", "Sphere1");
        scene._attachMaterialToObject("Red", "Plane1");
    }

    static void setup_scene(Scene &scene, const SceneType scene_type)
    {
        switch (scene_type)
        {
        case SceneType::TP1:
            break;
        case SceneType::TP2:
            break;
        case SceneType::TP6:
            break;
        case SceneType::TP3:
            setup_TP3(scene);
            break;
        case SceneType::TP4:
            setup_TP4(scene);
            break;
        case SceneType::TP5:
            setup_TP5(scene);
            break;
        default:
            break;
        }
    }

}

#endif /* __RT_ISICG_SCENE_SETUP__ */