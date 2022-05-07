#ifndef __RT_ISICG_TRIANGLE_MESH__
#define __RT_ISICG_TRIANGLE_MESH__

#include "objects/BaseObject.hpp"
#include "acceleration_structures/aabb.hpp"
#include "acceleration_structures/bvh.hpp"
#include "geometry/TriangleMeshGeometry.hpp"
#include <vector>

namespace RT_ISICG
{
    class MeshTriangle : public BaseObject
    {
        friend class TriangleMeshGeometry;

    public:
        MeshTriangle() = delete;

        MeshTriangle(const std::string &p_name)
            : BaseObject(p_name){};

        MeshTriangle(const std::string &p_name, const Vec3f &p_position)
            : BaseObject(p_name, p_position){};

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
        };

        void addVertex(const float p_x, const float p_y, const float p_z)
        {
            _vertices.emplace_back(p_x + _position.x,
                                   p_y + _position.y,
                                   p_z + _position.z);
            _aabb.extend(Vec3f(p_x, p_y, p_z));
        }

        void addNormal(const float p_x, const float p_y, const float p_z)
        {
            _normals.emplace_back(p_x, p_y, p_z);
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

        std::vector<Vec3f> _vertices;
        std::vector<Vec3f> _normals;
        std::vector<Vec2f> _uvs;
        std::vector<TriangleMeshGeometry> _triangles;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_TRIANGLE_MESH__
