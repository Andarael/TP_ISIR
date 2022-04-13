#include "Scene.hpp"

#include "lights/PointLight.hpp"
#include "lights/QuadLight.hpp"
#include "materials/ColorMaterial.hpp"
#include "objects/MeshTriangle.hpp"
#include "scene_setup.hpp"

#include "../lib/assimp/assimp/Importer.hpp"
#include "../lib/assimp/assimp/postprocess.h"
#include "../lib/assimp/assimp/scene.h"

namespace RT_ISICG
{
    Scene::Scene()
    {
        _addMaterial(new ColorMaterial("default", WHITE));
    }

    Scene::~Scene()
    {
        for (const ObjectMapPair &object : _objectMap)
            delete object.second;

        for (const MaterialMapPair &material : _materialMap)
            delete material.second;

        for (const BaseLight *light : _lightList)
            delete light;
    }

    void Scene::init()
    {
        init(SceneType::TP4);
    }

    void Scene::init(const SceneType &p_type)
    {
        setup_scene(*this, p_type);
    }

    void Scene::loadFileTriangleMesh(const std::string &p_name, const std::string &p_path)
    {
        std::cout << "Loading: " << p_path << std::endl;
        Assimp::Importer importer;

        // Read scene and triangulate meshes
        const aiScene *const scene = importer.ReadFile(p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);

        if (scene == nullptr)
            throw std::runtime_error("Fail to load file: " + p_path);

        unsigned int cptTriangles = 0;
        unsigned int cptVertices = 0;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
        {
            const aiMesh *const mesh = scene->mMeshes[m];
            if (mesh == nullptr)
                throw std::runtime_error("Fail to load file: " + p_path + ": mesh is null");

            const std::string meshName = p_name + "_" + std::string(mesh->mName.C_Str());
            std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

            cptTriangles += mesh->mNumFaces;
            cptVertices += mesh->mNumVertices;

            const bool hasUV = mesh->HasTextureCoords(0);

            MeshTriangle *triMesh = new MeshTriangle(meshName);
            // Vertices before faces otherwise face normals cannot be computed.
            for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
            {
                triMesh->addVertex(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
                triMesh->addNormal(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
                if (hasUV)
                    triMesh->addUV(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
            }
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                const aiFace &face = mesh->mFaces[f];
                triMesh->addTriangle(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
            }

            _addObject(triMesh);

            // triMesh->buildBVH();
            // triMesh->_bvh.print();

            const aiMaterial *const mtl = scene->mMaterials[mesh->mMaterialIndex];
            if (mtl == nullptr)
            {
                std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
            }
            else
            {
                Vec3f kd = WHITE;
                Vec3f ks = BLACK;
                float s = 0.f;

                aiColor3D aiKd;
                if (mtl->Get(AI_MATKEY_COLOR_DIFFUSE, aiKd) == AI_SUCCESS)
                    kd = Vec3f(aiKd.r, aiKd.g, aiKd.b);
                aiColor3D aiKs;
                if (mtl->Get(AI_MATKEY_COLOR_SPECULAR, aiKs) == AI_SUCCESS)
                    ks = Vec3f(aiKs.r, aiKs.g, aiKs.b);
                float aiS = 0.f;
                if (mtl->Get(AI_MATKEY_SHININESS, aiS) == AI_SUCCESS)
                    s = aiS;
                aiString mtlName;
                mtl->Get(AI_MATKEY_NAME, mtlName);

                _addMaterial(new PlasticMaterial(std::string(mtlName.C_Str()), kd, ks, s));
                _attachMaterialToObject(mtlName.C_Str(), meshName);
            }

            std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
                      << " vertices." << std::endl;
        }
        std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
                  << " vertices." << std::endl;
    }

    bool Scene::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        float tMax = p_tMax;
        bool hit = false;
        for (const ObjectMapPair &object : _objectMap)
        {
            if (object.second->intersect(p_ray, p_tMin, tMax, p_hitRecord))
            {
                tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
                hit = true;
            }
        }
        return hit;
    }

    bool Scene::intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        bool hit = false;
        for (const ObjectMapPair &object : _objectMap)
            if (object.second->intersectAny(p_ray, p_tMin, p_tMax))
                return true;
        return false;
    }

    void Scene::_addObject(BaseObject *p_object)
    {
        const std::string &name = p_object->getName();
        if (_objectMap.find(name) != _objectMap.end())
        {
            std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
            delete p_object;
        }
        else
        {
            _objectMap[name] = p_object;
            _objectMap[name]->setMaterial(_materialMap["default"]);
        }
    }

    void Scene::_addMaterial(BaseMaterial *p_material)
    {
        const std::string &name = p_material->getName();
        if (_materialMap.find(name) != _materialMap.end())
        {
            std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
            delete p_material;
        }
        else
        {
            _materialMap[name] = p_material;
        }
    }

    void Scene::_addLight(BaseLight *p_light)
    {
        _lightList.emplace_back(p_light);
    }

    void Scene::_attachMaterialToObject(const std::string &p_materialName, const std::string &p_objectName)
    {
        if (_objectMap.find(p_objectName) == _objectMap.end())
        {
            std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
                      << std::endl;
        }
        else if (_materialMap.find(p_materialName) == _materialMap.end())
        {
            std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
                      << "object \'" << p_objectName << "\' keeps its material \'"
                      << _objectMap[p_objectName]->getMaterial()->getName() << "\'" << std::endl;
        }
        else
        {
            _objectMap[p_objectName]->setMaterial(_materialMap[p_materialName]);
        }
    }
} // namespace RT_ISICG
