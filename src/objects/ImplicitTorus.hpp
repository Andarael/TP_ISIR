#ifndef __RT_ISICG_IMPLICIT_TORUS__
#define __RT_ISICG_IMPLICIT_TORUS__
#include "ImplicitSurface.hpp"

namespace RT_ISICG
{
    class ImplicitTorus : public ImplicitSurface

    {
    public:
        ImplicitTorus(const std::string &p_name, const Vec3f &p_position, const float p_scale = 1.0f)
            : ImplicitSurface(p_name, p_position, p_scale), _R(0), _r(0) {}

        ImplicitTorus() = delete;

        ImplicitTorus(const std::string &p_name, const float p_R = 1.f, const float p_r = 2.f)
            : ImplicitSurface(p_name), _R(p_r), _r(p_R){};

    private:
        float _R;
        float _r;

        float _blob(const Vec3f &p) const
        {
            return p.x * p.x + p.y * p.y + p.z * p.z + glm::sin(4.f * p.x) - glm::cos(4.f * p.y) + glm::sin(4.f * p.z);
        }

        float _sdf(const Vec3f &p) const override
        {
            return _blob(p * 0.05f);
        }
    };
} // namespace RT_ISICG

#endif __RT_ISICG_IMPLICIT_TORUS__
