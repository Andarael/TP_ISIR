#ifndef __RT_ISICG_BASE_INTEGRATOR__
#define __RT_ISICG_BASE_INTEGRATOR__

#include "Scene.hpp"

namespace RT_ISICG
{
    enum class IntegratorType : int
    {
        RAY_CAST = 0,
        DIRECT_LIGHT = 1,
        COUNT // keep it last
    };

    class BaseIntegrator
    {
    public:
        BaseIntegrator() = default;

        virtual IntegratorType getType() const = 0;

        virtual ~BaseIntegrator() = default;

        // Return incoming luminance.
        virtual Vec3f Li(const Scene &p_scene, const Ray &p_ray, float p_tMin, float p_tMax) const = 0;

        const Vec3f &getBackgroundColor() const
        {
            return _backgroundColor;
        }

        void setBackgroundColor(const Vec3f &p_color)
        {
            _backgroundColor = p_color;
        }

    protected:
        Vec3f _backgroundColor = BLACK;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_INTEGRATOR__
