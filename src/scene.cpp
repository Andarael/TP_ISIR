#include "Scene.hpp"

// assimp
#include <assimp/assimp/Importer.hpp>
#include <assimp/assimp/postprocess.h>
#include <assimp/assimp/scene.h>

#include "Renderer.hpp"
#include "scene_setup.hpp"

#include "materials/ColorMaterial.hpp"
#include "materials/PbrMaterial.hpp"
#include "materials/PlasticMaterial.hpp"
#include "materials/TextureMaterial.hpp"
#include "materials/lambertMaterial.hpp"
#include "objects/meshs/MeshTriangle.hpp"

namespace RT_ISICG
{
    Scene::Scene() { _addMaterial(new ColorMaterial("default", WHITE)); }

    Scene::~Scene()
    {
        for (const ObjectMapPair &object : _objectMap)
            delete object.second;

        for (const MaterialMapPair &material : _materialMap)
            delete material.second;

        for (const BaseLight *light : _lightList)
            delete light;
    }

    BaseCamera &Scene::init() { return Scene::init(SceneType::TP1); }

    BaseCamera &Scene::init(const SceneType &p_type) { return setup_scene(*this, p_type); }

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

    void Scene::loadFileTriangleMesh(const std::string &p_name, const std::string &p_filePath, const std::string &p_path, const Vec3f &p_pos, const float p_scale, const float p_rotation, const Vec3f p_rotation_axis)
    {
        std::cout << "Loading: " << p_path << std::endl;
        Assimp::Importer importer;

        // Read scene and triangulate meshes
        const aiScene *const scene = importer.ReadFile(p_filePath + p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);

        if (scene == nullptr)
            throw std::runtime_error("Fail to load file: " + p_path);

        unsigned int cptTriangles = 0;
        unsigned int cptVertices = 0;

        // #pragma omp parallel for schedule(dynamic)
        for (int m = 0; m < scene->mNumMeshes; ++m)
        {
            const aiMesh *const mesh = scene->mMeshes[m];
            if (mesh == nullptr)
                throw std::runtime_error("Fail to load file: " + p_path + ": mesh is null");

            const std::string meshName = p_name + "_" + std::string(mesh->mName.C_Str());
            std::cout << "----- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

            cptTriangles += mesh->mNumFaces;
            cptVertices += mesh->mNumVertices;

            const bool hasUV = mesh->HasTextureCoords(0);

            MeshTriangle *triMesh = new MeshTriangle(meshName, p_pos, p_scale, p_rotation, p_rotation_axis); // mesh position is intialized here

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

            triMesh->buildBVH();
            _addObject(triMesh);

            const aiMaterial *const mtl = scene->mMaterials[mesh->mMaterialIndex];

            if (mtl == nullptr)
                std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;

            else
            {
                Vec3f kd = WHITE;      // diffuse
                Vec3f ks = BLACK;      // specular
                Vec3f ke = BLACK;      // emissive
                Vec3f tf = BLACK;      // transmission
                float shininess = 0.f; // shininess

                aiColor3D aiKd;
                if (mtl->Get(AI_MATKEY_COLOR_DIFFUSE, aiKd) == AI_SUCCESS)
                    kd = Vec3f(aiKd.r, aiKd.g, aiKd.b);

                aiColor3D aiKs;
                if (mtl->Get(AI_MATKEY_COLOR_SPECULAR, aiKs) == AI_SUCCESS)
                    ks = Vec3f(aiKs.r, aiKs.g, aiKs.b);

                float aiS = 0.f;
                if (mtl->Get(AI_MATKEY_SHININESS, aiS) == AI_SUCCESS)
                    shininess = aiS;

                aiColor3D aiE;
                if (mtl->Get(AI_MATKEY_COLOR_EMISSIVE, aiE) == AI_SUCCESS)
                    ke = Vec3f(aiE.r, aiE.g, aiE.b);

                aiColor3D aiTf;
                if (mtl->Get(AI_MATKEY_OPACITY, tf) == AI_SUCCESS)
                    tf = Vec3f(aiTf.r, aiTf.g, aiTf.b);

                aiString mtlName;
                mtl->Get(AI_MATKEY_NAME, mtlName);
                std::string mtlNameStr = p_name + "_" + mtlName.C_Str();

                aiString aiTexDiffuse;
                mtl->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexDiffuse);
                std::string texDiffusePath = p_filePath + aiTexDiffuse.C_Str();

                aiString aiTexEmit;
                mtl->GetTexture(aiTextureType_EMISSION_COLOR, 0, &aiTexEmit);
                std::string texEmitPath = p_filePath + aiTexDiffuse.C_Str();

                aiString aiTexRough;
                mtl->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &aiTexRough);
                std::string texRoughPath = p_filePath + aiTexDiffuse.C_Str();

                aiString aiTexNormal;
                mtl->GetTexture(aiTextureType_NORMALS, 0, &aiTexNormal);
                if (strlen(aiTexNormal.C_Str()) == 0)
                    mtl->GetTexture(aiTextureType_HEIGHT, 0, &aiTexNormal);

                std::string texNormalPath = p_filePath + aiTexNormal.C_Str();

                printf("Material: %s\n", mtlNameStr.c_str());
                printf("  Diffuse: %f %f %f\n", kd.x, kd.y, kd.z);
                printf("  Specular: %f %f %f\n", ks.x, ks.y, ks.z);
                printf("  Opacity: %f %f %f\n", tf.x, tf.y, tf.z);
                printf("  Shininess: %f\n", shininess);
                printf("  Emission: %f %f %f\n", ke.x, ke.y, ke.z);
                printf("  Texture Diffuse: %s\n", aiTexDiffuse.C_Str());
                printf("  Texture Normal : %s\n", aiTexNormal.C_Str());
                printf("  Texture emit : %s\n", aiTexEmit.C_Str());
                printf("  Texture rough : %s\n", aiTexRough.C_Str());

                PbrMaterial *material = new PbrMaterial(mtlNameStr, texDiffusePath, texNormalPath, texEmitPath, texRoughPath);
                material->setColor(kd);
                material->setEmit(ke);
                material->setRoughness(1.f / shininess);

                _addMaterial(material);

                _attachMaterialToObject(mtlNameStr, meshName);
            }

            std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
                      << " vertices." << std::endl;
        }
        std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
                  << " vertices." << std::endl;
    }

} // namespace RT_ISICG
