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
        TP7,
        SCENE_TYPE_COUNT
    };

    void addMaterials(Scene &scene);

    void addCornellBox(Scene &scene, bool fullMirror);

    RenderSettings setup_scene(Scene &scene, const SceneType scene_type);

} // namespace RT_ISICG

#endif /* __RT_ISICG_SCENE_SETUP__ */