#include "objects/meshs/MeshTriangle.hpp"

namespace RT_ISICG
{
    bool MeshTriangle::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        if (_bvh.intersect(p_ray, p_tMin, p_tMax, p_hitRecord))
        {
            p_hitRecord._object = this;
            return true;
        }

        return false;

        // todo comparison
        if (!_aabb.intersect(p_ray, p_tMin, p_tMax))
            return false;

        float tClosest = p_tMax;           // Hit distance.
        size_t hitTri = _triangles.size(); // Hit triangle id.

        Vec2f uvTemp;
        Vec2f uv = VEC2F_ZERO; // this is the uv of the hitpoint in the triangle
        for (size_t i = 0; i < _triangles.size(); i++)
        {
            float t;
            if (_triangles[i].intersect(p_ray, t, uvTemp))
            {
                if (t >= p_tMin && t <= tClosest)
                {
                    uv = uvTemp; // copy
                    tClosest = t;
                    hitTri = i;
                }
            }
        }
        if (hitTri != _triangles.size()) // Intersection found.
        {
            Vec3f normal = _triangles[hitTri].getSmoothNormal(uv);
            Vec3f trueNormal = _triangles[hitTri].getFaceNormal();

            p_hitRecord.fill(p_ray, normal, trueNormal, tClosest);
            p_hitRecord._uv = _triangles[hitTri].getUV(uv); // this is the UVs of the hitpoint in the texture

            return true;
        }
        return false;
    }

    bool MeshTriangle::intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        return _bvh.intersectAny(p_ray, p_tMin, p_tMax);

        // todo comparison
        if (!_aabb.intersect(p_ray, p_tMin, p_tMax))
            return false;

        for (size_t i = 0; i < _triangles.size(); i++)
        {
            float t;
            Vec2f uv;
            if (_triangles[i].intersect(p_ray, t, uv))
                if (t >= p_tMin && t <= p_tMax)
                    return true; // No need to search for the nearest.
        }
        return false;
    }
} // namespace RT_ISICG
