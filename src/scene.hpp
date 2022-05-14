#ifndef __RT_ISICG_SCENE__
#define __RT_ISICG_SCENE__

#include "Renderer.hpp"
#include "defines.hpp"
#include "lights/BaseLight.hpp"
#include "objects/BaseObject.hpp"

#include <exception>
#include <map>
#include <vector>

namespace RT_ISICG
{
    // forward declaration
    enum class SceneType;

    using ObjectMap = std::map<const std::string, BaseObject *>;
    using ObjectMapPair = ObjectMap::value_type;
    using MaterialMap = std::map<const std::string, BaseMaterial *>;
    using MaterialMapPair = MaterialMap::value_type;
    using LightList = std::vector<BaseLight *>;

    class Scene
    {
    public:
        Scene();

        ~Scene();

        // Hard coded initialization.
        BaseCamera &init();

        BaseCamera &init(const SceneType &p_type);

        // Initialization from file. todo
        static RenderSettings init(const std::string &p_path)
        {
            throw std::exception("Not implemented !");
        }

        const LightList &getLights() const { return _lightList; }

        void loadFileTriangleMesh(const std::string &p_name,
                                  const std::string &p_path,
                                  const Vec3f &p_pos = VEC3F_ZERO,
                                  float p_scale = 1.0f,
                                  float p_rotation = 0.0f,
                                  Vec3f p_rotation_axis = VEC3F_Y);

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        bool intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const;

        bool intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const;

    public:
        void _addObject(BaseObject *p_object);
        void _addMaterial(BaseMaterial *p_material);
        void _addLight(BaseLight *p_light);
        void _attachMaterialToObject(const std::string &p_materialName, const std::string &p_objectName);

    private:
        ObjectMap _objectMap;
        MaterialMap _materialMap;
        LightList _lightList;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_SCENE__
