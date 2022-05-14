#ifndef __RT_ISICG_PATH_INTEGRATOR__
#define __RT_ISICG_PATH_INTEGRATOR__

#include "defines.hpp"
#include "integrators/WhittedIntegrator.hpp"

namespace RT_ISICG
{
    class PathIntegrator : public WhittedIntegrator
    {

    public:
        PathIntegrator(const int p_shadowSamples, const int p_lightDepth, const int p_transparentDepth)
            : WhittedIntegrator(p_shadowSamples, p_transparentDepth){};

        IntegratorType getType() const override { return IntegratorType::WHITTED; }

        Vec3f Li(const Scene &p_scene, const Ray &p_ray) const override
        {
            return trace(p_scene, p_ray);
        }

    };
} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__