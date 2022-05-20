#ifndef __RT_ISICG_SCENE_SETUP__
#define __RT_ISICG_SCENE_SETUP__

#include "Renderer.hpp"
#include "Scene.hpp"

namespace RT_ISICG
{
    enum class SceneType
    {
        TP1,
        TP2,
        TP3,
        TP4,
        TP5,
        TP6,
        TP6_Conference,
        TP6_Sponza,
        TP7,
        APPLE,
        DEMO,
        SCENE_TYPE_COUNT
    };

    void addMaterials(Scene &scene);

    void addCornellBox(Scene &scene, bool fullMirror, float scale = 10.f, const Vec3f &base = Vec3f(0.f, -3.f, 0.f));

    BaseCamera &setup_TP1(Scene &scene);

    BaseCamera &setup_TP6_Conference(Scene &scene);

    BaseCamera &setup_TP6_Sponza(Scene &scene);

    BaseCamera &setup_TP7(Scene &scene);

    BaseCamera &setup_TP6(Scene &scene);

    BaseCamera &setup_TP5(Scene &scene);

    BaseCamera &setup_TP4(Scene &scene);

    BaseCamera &setup_TP3(Scene &scene);

    BaseCamera &setup_pomme(Scene &scene);

    BaseCamera &setup_demo(Scene &scene);

    BaseCamera &setup_scene(Scene &scene, const SceneType scene_type);

} // namespace RT_ISICG

#endif /* __RT_ISICG_SCENE_SETUP__ */