#include "bvh.hpp"
#include "geometry/TriangleMeshGeometry.hpp"
#include "utils/chrono.hpp"

#include <algorithm>

namespace RT_ISICG
{
    void BVH::build(std::vector<TriangleMeshGeometry> *p_triangles)
    {
        std::cout << "Building BVH..." << std::endl;

        if (p_triangles == nullptr || p_triangles->empty())
            throw std::exception("BVH::build() error: no triangle provided");

        _triangles = p_triangles;

        Chrono chr;
        chr.start();

        _root = new BVHNode();
        _buildRec(_root, 0, _triangles->size(), 0);

        chr.stop();

        std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
    }

    bool BVH::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        return _intersectRec(_root, p_ray, p_tMin, p_tMax, p_hitRecord);
    }

    bool BVH::intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        return _intersectAnyRec(_root, p_ray, p_tMin, p_tMax);
    }

    void BVH::_buildRec(BVHNode *p_node, const unsigned int p_firstTriangleId, const unsigned int p_lastTriangleId, const unsigned int p_depth)
    {
        p_node->_firstTriangleId = p_firstTriangleId;
        p_node->_lastTriangleId = p_lastTriangleId;

        for (unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++)
            p_node->_aabb.extend((*_triangles)[i]);

        bool stopCondition = p_depth >= _maxDepth || p_lastTriangleId - p_firstTriangleId <= _maxTrianglesPerLeaf;
        if (stopCondition)
            return;

        int partitionAxis = int(p_node->_aabb.largestAxis());

        unsigned int idPartition = unsigned int((p_firstTriangleId + p_lastTriangleId) * 0.5f);

        std::partial_sort(
            _triangles->begin() + p_firstTriangleId,
            _triangles->begin() + p_lastTriangleId,
            _triangles->begin() + p_lastTriangleId,
            [partitionAxis](const TriangleMeshGeometry &a, const TriangleMeshGeometry &b)
            {
                return a._centroid[partitionAxis] < b._centroid[partitionAxis];
            });

        p_node->_left = new BVHNode();
        p_node->_right = new BVHNode();

        _buildRec(p_node->_left, p_firstTriangleId, idPartition, p_depth + 1);
        _buildRec(p_node->_right, idPartition, p_lastTriangleId, p_depth + 1);
    }

    bool BVH::_intersectRec(const BVHNode *p_node, const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        if (!p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
            return false;

        if (p_node->isLeaf())
        {
            float tClosest = p_tMax;            // Hit distance.
            size_t hitTri = _triangles->size(); // Hit triangle id.

            Vec2f uvTemp;
            Vec2f uv = VEC2F_ZERO;
            for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++)
            {
                float t;
                if ((*_triangles)[i].intersect(p_ray, t, uvTemp))
                {
                    if (t >= p_tMin && t <= tClosest)
                    {
                        uv = uvTemp;
                        tClosest = t;
                        hitTri = i;
                    }
                }
            }
            if (hitTri != _triangles->size()) // Intersection found.
            {
                if (p_hitRecord._distance > tClosest)
                {
                    Vec3f normal = (*_triangles)[hitTri].getSmoothNormal(uv);
                    Vec3f trueNormal = (*_triangles)[hitTri].getFaceNormal();
                    p_hitRecord.fill(p_ray, normal, trueNormal, tClosest);
                }
                return true;
            }
            return false;
        }

        bool intersectLeft = _intersectRec(p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord);
        bool intersectRight = _intersectRec(p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord);
        return intersectLeft || intersectRight;
    }

    bool BVH::_intersectAnyRec(const BVHNode *p_node, const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        if (!p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
            return false;

        if (p_node->isLeaf())
        {
            for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++)
            {
                float t;
                Vec2f uv;
                if ((*_triangles)[i].intersect(p_ray, t, uv))
                    if (t >= p_tMin && t <= p_tMax)
                        return true; // No need to search for the nearest.
            }
            return false;
        }

        return _intersectAnyRec(p_node->_left, p_ray, p_tMin, p_tMax) ||
               _intersectAnyRec(p_node->_right, p_ray, p_tMin, p_tMax);
    }
} // namespace RT_ISICG
