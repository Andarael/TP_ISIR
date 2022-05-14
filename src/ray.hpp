#ifndef __RT_ISICG_RAY__
#define __RT_ISICG_RAY__

#include "defines.hpp"

namespace RT_ISICG
{

    enum class RayType : int
    {
        none,
        camera,
        shadow,
        diffuse,
        reflection,
        transmission,
        count
    };

    struct LightPath
    {
        float tmin = 0;
        float tmax = 0;

        // depths
        int depthReflected = 0;
        int depthTransmitted = 0;
        int depthDiffuse = 0;

        RayType rayType = RayType::none;
    };

    class Ray
    {
    public:
        // ray constructors ==============================================================

        Ray() = delete;

        Ray(const Vec3f &p_origin, const Vec3f &p_direction, const RayType p_type, const Ray *p_parentRay)
            : _origin(p_origin),
              _direction(normalize(p_direction)),
              _invDirection(1.f / _direction)
        {
            if (p_parentRay != nullptr)
                copyLightPath(*p_parentRay);

            _lightPath.rayType = p_type;
        };

        Ray(const Vec3f &p_origin, const Vec3f &p_direction, const RayType p_type)
            : Ray(p_origin, p_direction, p_type, nullptr){};

        Ray(const Vec3f &p_origin, const Vec3f &p_direction)
            : Ray(p_origin, p_direction, RayType::none, nullptr){};

        Ray(const Vec3f &p_origin, const Vec3f &p_direction, const Ray *p_parentRay)
            : Ray(p_origin, p_direction, RayType::none, p_parentRay){};

        // ray accessors =================================================================

        const Vec3f &getOrigin() const { return _origin; }

        const Vec3f &getDirection() const { return _direction; }

        const Vec3f &getInvDirection() const { return _invDirection; }

        // Return the point along _direction at distance 'p_t'
        Vec3f pointAtT(const float p_t) const { return _origin + _direction * p_t; }

        // Offset =======================================================================
        // This function add an offset to the ray origin to avoid self-intersection when computing shadows
        // See:
        // W�chter C., Binder N. (2019)
        // A Fast and Robust Method for Avoiding Self-Intersection.
        // In: Haines E., Akenine-M�ller T. (eds) Ray Tracing Gems. Apress, Berkeley, CA
        void offset(const Vec3f p_normal)
        {
            Vec3i of_i(p_normal * _intScale());

            Vec3i ip_i(*reinterpret_cast<const int *>(&_origin.x) + ((_origin.x < 0.f) ? -of_i.x : of_i.x),
                       *reinterpret_cast<const int *>(&_origin.y) + ((_origin.y < 0.f) ? -of_i.y : of_i.y),
                       *reinterpret_cast<const int *>(&_origin.z) + ((_origin.z < 0.f) ? -of_i.z : of_i.z));

            Vec3f p_i(*reinterpret_cast<const float *>(&ip_i[0]),
                      *reinterpret_cast<const float *>(&ip_i[1]),
                      *reinterpret_cast<const float *>(&ip_i[2]));

            _origin = Vec3f(fabsf(_origin.x) < _originScale() ? _origin.x + _floatScale() * p_normal.x : p_i.x,
                            fabsf(_origin.y) < _originScale() ? _origin.y + _floatScale() * p_normal.y : p_i.y,
                            fabsf(_origin.z) < _originScale() ? _origin.z + _floatScale() * p_normal.z : p_i.z);
        }

        // Light Path methods ============================================================

        // does not copy parent ray origin
        void copyLightPath(const Ray &p_ray)
        {
            _lightPath.tmin = p_ray._lightPath.tmin;
            _lightPath.tmax = p_ray._lightPath.tmax;
            _lightPath.depthReflected = p_ray._lightPath.depthReflected;
            _lightPath.depthTransmitted = p_ray._lightPath.depthTransmitted;
            _lightPath.depthDiffuse = p_ray._lightPath.depthDiffuse;
        }

        void incrRefreact() { _lightPath.depthTransmitted++; }
        void incrRefrlect() { _lightPath.depthReflected++; }
        void incrDiffuse() { _lightPath.depthDiffuse++; }

        bool isCameraRay() const { return _lightPath.rayType == RayType::camera; }
        bool isShadowRay() const { return _lightPath.rayType == RayType::shadow; }
        bool isDiffuseRay() const { return _lightPath.rayType == RayType::diffuse; }
        bool isReflectionRay() const { return _lightPath.rayType == RayType::reflection; }
        bool isTransmissionRay() const { return _lightPath.rayType == RayType::transmission; }

        void setCameraRay() { _lightPath.rayType = RayType::camera; }
        void setShadowRay() { _lightPath.rayType = RayType::shadow; }
        void setDiffuseRay() { _lightPath.rayType = RayType::diffuse; }
        void setReflectionRay() { _lightPath.rayType = RayType::reflection; }
        void setTransmissionRay() { _lightPath.rayType = RayType::transmission; }

        int getDepth() const
        {
            int temp = glm::max(_lightPath.depthReflected, _lightPath.depthTransmitted);
            return glm::max(temp, _lightPath.depthDiffuse);
        }

        float getTmax() const { return _lightPath.tmax; }
        float getTmin() const { return _lightPath.tmin; }

        LightPath _lightPath;

        // Private members =============================================================

    private:
        // Used for offset()
        static constexpr float _originScale() { return 1.f / 32.f; }
        static constexpr float _floatScale() { return 1.f / 65536.f; }
        static constexpr float _intScale() { return 256.f; }

    private:
        Vec3f _origin = VEC3F_ZERO;
        Vec3f _direction = VEC3F_ZERO;
        Vec3f _invDirection = VEC3F_ZERO;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_RAY__
