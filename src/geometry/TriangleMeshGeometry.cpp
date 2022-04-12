#include "TriangleMeshGeometry.hpp"
#include "objects/MeshTriangle.hpp"

namespace RT_ISICG
{
    TriangleMeshGeometry::TriangleMeshGeometry(const unsigned int p_v0, const unsigned int p_v1, const unsigned int p_v2, MeshTriangle *p_refMesh)
        : BaseGeometry(VEC3F_ZERO), _refMesh(p_refMesh), _v0(p_v0), _v1(p_v1), _v2(p_v2)
    {
        _faceNormal = glm::normalize(glm::cross(_refMesh->_vertices[p_v1] - _refMesh->_vertices[p_v0], _refMesh->_vertices[p_v2] - _refMesh->_vertices[p_v0]));
    }

    bool TriangleMeshGeometry::intersect(const Ray &p_ray, float &p_t, Vec2f &uv) const
    {
        p_t = -1.f;

        const Vec3f &o = p_ray.getOrigin();
        const Vec3f &d = p_ray.getDirection();
        const Vec3f &v0 = _refMesh->_vertices[_v0];
        const Vec3f &v1 = _refMesh->_vertices[_v1];
        const Vec3f &v2 = _refMesh->_vertices[_v2];

        // Compute the edge vectors
        Vec3f edge1 = v1 - v0;
        Vec3f edge2 = v2 - v0;

        // Compute the determinant
        Vec3f pvec = glm::cross(d, edge2);
        float det = glm::dot(edge1, pvec);

        // ray and triangle are parallel if det is close to 0
        if (glm::abs(det) < EPSILON)
            return false;

        float invDet = 1.0f / det;

        // calculate distance from v0 to ray origin
        Vec3f tvec = o - v0;

        // calculate u parameter and test bounds
        float u = dot(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f)
            return false;

        // prepare to test v parameter
        Vec3f qvec = glm::cross(tvec, edge1);

        // calculate v parameter and test bounds
        float v = glm::dot(d, qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f)
            return false;

        uv.x = u;
        uv.y = v;

        // calculate t, ray intersects triangle
        p_t = glm::dot(edge2, qvec) * invDet;

        return true;
    }

    Vec3f TriangleMeshGeometry::getSmoothNormal(Vec2f const &uv) const
    {
        Vec3f n0 = _refMesh->_normals[_v0];
        Vec3f n1 = _refMesh->_normals[_v1];
        Vec3f n2 = _refMesh->_normals[_v2];

        return glm::normalize(Vec3f((1.f - uv.x - uv.y) * n0 + uv.x * n1 + uv.y * n2));
    }
} // namespace RT_ISICG
