#ifndef __RT_ISICG_BVH__
#define __RT_ISICG_BVH__

#include "HitRecord.hpp"
#include "aabb.hpp"
#include <vector>

namespace RT_ISICG
{
    class TriangleMeshGeometry;

    struct BVHNode
    {
        BVHNode() = default;

        ~BVHNode()
        {
            delete _left;
            delete _right;
        }

        bool isLeaf() const
        {
            return (_left == nullptr && _right == nullptr);
        }

        bool nbTriangles() const
        {
            return _firstTriangleId - _lastTriangleId; // +1 ?
        }

        int getDepth() const
        {
            if (isLeaf())
                return 0;

            int leftDepth = _left->getDepth();
            int rightDepth = _right->getDepth();

            return 1 + glm::max(leftDepth, rightDepth);
        }

        bool stopCondition(const unsigned int maxDepth, const unsigned int maxTriangles, const unsigned int actualDepth = 0) const
        {
            if (isLeaf())
                return actualDepth < maxDepth && nbTriangles() < maxTriangles;

            bool leftCondition = false;
            bool rightCondition = false;

            if (_left != nullptr)
                leftCondition = _left->stopCondition(maxDepth, maxTriangles, actualDepth + 1);

            if (_right != nullptr)
                rightCondition = _right->stopCondition(maxDepth, maxTriangles, actualDepth + 1);

            return leftCondition || rightCondition;
        }

        AABB _aabb;
        BVHNode *_left = nullptr;
        BVHNode *_right = nullptr;
        unsigned int _firstTriangleId = 0;
        unsigned int _lastTriangleId = 0;
    };

    class BVH
    {
    public:
        BVH() = default;

        ~BVH()
        {
            delete _root;
        }

        // Build the BVH from a list of triangles (call _buildRec).
        void build(std::vector<TriangleMeshGeometry> *p_triangles);

        // Search for the nearest intersection with the ray (call _intersectRec).
        bool intersect(const Ray &p_ray, float p_tMin, float p_tMax, HitRecord &p_hitRecord) const;

        // Search for the any intersection with the ray (call _intersectRec).
        bool intersectAny(const Ray &p_ray, float p_tMin, float p_tMax) const;

        void print() const
        {
            std::cout << "depth : " << _root->getDepth() << std::endl;
        }

    private:
        void _buildRec(BVHNode *p_node, unsigned int p_firstTriangleId, unsigned int p_nbTriangles, unsigned int p_depth);

        bool _intersectRec(const BVHNode *p_node, const Ray &p_ray, float p_tMin, float p_tMax, HitRecord &p_hitRecord) const;

        bool _intersectAnyRec(const BVHNode *p_node, const Ray &p_ray, float p_tMin, float p_tMax) const;

    private:
        std::vector<TriangleMeshGeometry> *_triangles = nullptr;
        BVHNode *_root = nullptr;

        const unsigned int _maxTrianglesPerLeaf = 8;
        const unsigned int _maxDepth = 32;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BVH__
