
#ifndef __RT_ISICG_EMISSIVE_MATERIAL__
#define __RT_ISICG_EMISSIVE_MATERIAL__

#include "materials/BaseMaterial.hpp"

namespace RT_ISICG
{
    class EmissiveMaterial : public BaseMaterial
    {
    public:
        EmissiveMaterial(const std::string &p_name, const Vec3f &p_emit)
            : BaseMaterial(p_name), _emit(p_emit){};

        Vec3f shade(const Vec3f &p_rayDirection, const HitRecord &p_hitRecord, const Vec3f &p_lightDirection) const override
        {
            return BLACK;
        }

        Vec3f getFlatColor(const HitRecord &p_hitRecord) const override
        {
            return _emit;
        }

        Vec3f getEmit(const HitRecord &p_hitRecord) const override
        {
            return _emit;
        }

    protected:
        Vec3f _emit = WHITE;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_EMISSIVE_MATERIAL__
