#include "MeshTriangle.hpp"

namespace RT_ISICG
{
    bool MeshTriangle::intersect(const Ray &p_ray, const float p_tMin, const float p_tMax, HitRecord &p_hitRecord) const
    {
        float tClosest = p_tMax;           // Hit distance.
        size_t hitTri = _triangles.size(); // Hit triangle id.

        Vec2f uvTemp;
        Vec2f uv = VEC2F_ZERO;
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
            Vec3f normal;
            if (_smoothShading)
                normal = _triangles[hitTri].getSmoothNormal(uv);
            else
                normal = _triangles[hitTri].getFaceNormal();

            fillHitRecord(p_hitRecord, p_ray, normal, tClosest);

            return true;
        }
        return false;
    }

    bool MeshTriangle::intersectAny(const Ray &p_ray, const float p_tMin, const float p_tMax) const
    {
        for (size_t i = 0; i < _triangles.size(); i++)
        {
            float t;
            Vec2f uv;
            if (_triangles[i].intersect(p_ray, t, uv))
            {
                if (t >= p_tMin && t <= p_tMax)
                    return true; // No need to search for the nearest.
            }
        }
        return false;
    }
} // namespace RT_ISICG
