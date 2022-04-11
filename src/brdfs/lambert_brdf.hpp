#ifndef __RT_ISICG_BRDF_LAMBERT__
#define __RT_ISICG_BRDF_LAMBERT__

#include "defines.hpp"

namespace RT_ISICG
{
    class LambertBRDF
    {
    public:
        LambertBRDF(const Vec3f &p_kd)
            : _kd(p_kd), _precompute(_kd * INV_PIf){};

        Vec3f evaluate() const
        {
            return _precompute;
        }

        const Vec3f &getKd() const
        {
            return _kd;
        }

    private:
        Vec3f _kd = WHITE;
        Vec3f _precompute;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_LAMBERT__
