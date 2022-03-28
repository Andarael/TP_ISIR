#ifndef __RT_ISICG_TRIANGLE_GEOMETRY__
#define __RT_ISICG_TRIANGLE_GEOMETRY__

#include "BaseGeometry.hpp"

namespace RT_ISICG
{
    class MeshTriangle;

    class TriangleMeshGeometry : public BaseGeometry
    {
    public:
        TriangleMeshGeometry() = delete;

        ~TriangleMeshGeometry() override = default;

        TriangleMeshGeometry(unsigned int p_v0, unsigned int p_v1, unsigned int p_v2, MeshTriangle *p_refMesh);

        bool intersect(const Ray &p_ray, float &p_t) const;

        const Vec3f &getFaceNormal() const
        {
            return _faceNormal;
        }

    private:
        MeshTriangle *_refMesh;

        union
        {
            struct
            {
                unsigned int _v0, _v1, _v2;
            };
            unsigned int _v[3] = {0, 0, 0};
        };

        Vec3f _faceNormal{};
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_TRIANGLE_GEOMETRY__
