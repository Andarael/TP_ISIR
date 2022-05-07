#ifndef __RT_ISICG_TRIANGLE_MESH__
#define __RT_ISICG_TRIANGLE_MESH__

#include "acceleration_structures/aabb.hpp"
#include "acceleration_structures/bvh.hpp"
#include "geometry/TriangleMeshGeometry.hpp"
#include "objects/BaseObject.hpp"
#include <vector>

namespace RT_ISICG
{
    class MeshTriangle : public BaseObject
    {
        friend class TriangleMeshGeometry;

    public:
        MeshTriangle() = delete;

        MeshTriangle(const std::string &p_name)
            : MeshTriangle(p_name, VEC3F_ZERO){};

        MeshTriangle(const std::string &p_name,
                     const Vec3f &p_position = VEC3F_ZERO,
                     const float p_scale = 1.0f,
                     const float p_rotation = 0.0f, const Vec3f p_rotation_axis = VEC3F_Y)
            : BaseObject(p_name, p_position)
        {
            _transformation = glm::translate(_transformation, _position);
            _transformation = glm::scale(_transformation, Vec3f(p_scale));
            _transformation = glm::rotate(_transformation, glm::radians(p_rotation), p_rotation_axis);
        };

        ~MeshTriangle() override = default;

        size_t getNbTriangles() const
        {
            return _triangles.size();
        }

        size_t getNbVertices() const
        {
            return _vertices.size();
        }

        void addTriangle(const unsigned int p_v0, const unsigned int p_v1, const unsigned int p_v2)
        {
            _triangles.emplace_back(TriangleMeshGeometry(p_v0, p_v1, p_v2, this));
        }

        void addVertex(const float p_x, const float p_y, const float p_z)
        {
            Vec3f position = _transformation * Vec4f(p_x, p_y, p_z, 1.f); // todo scale avec w?
            _vertices.emplace_back(position.x, position.y, position.z);
            _aabb.extend(Vec3f(position.x, position.y, position.z));
        }

        void addNormal(const float p_x, const float p_y, const float p_z)
        {
            Vec3f normal = Vec3f(p_x, p_y, p_z);
            Mat4f normalMatrix = glm::transpose(glm::inverse(_transformation));
            normal = Mat3f(normalMatrix) * normal;
            _normals.emplace_back(normal.x, normal.y, normal.z);
        }

        void addUV(const float p_u, const float p_v)
        {
            _uvs.emplace_back(p_u, p_v);
        }

        void buildBVH()
        {
            _bvh.build(&_triangles);
        }

        // Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
        bool intersect(const Ray &p_ray, float p_tMin, float p_tMax, HitRecord &p_hitRecord) const override;

        // Check for any intersection between p_tMin and p_tMax.
        bool intersectAny(const Ray &p_ray, float p_tMin, float p_tMax) const override;

    private:
        BVH _bvh;
        AABB _aabb;

        Mat4f _transformation = MAT4F_ID;

        std::vector<Vec3f> _vertices;
        std::vector<Vec3f> _normals;
        std::vector<Vec2f> _uvs;
        std::vector<TriangleMeshGeometry> _triangles;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_TRIANGLE_MESH__
