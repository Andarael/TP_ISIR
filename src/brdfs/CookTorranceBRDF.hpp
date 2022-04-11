#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
    class CookTorranceBrdf
    {
    public:
        CookTorranceBrdf(const Vec3f &p_F0, const float p_roughness)
            : _alpha(p_roughness * p_roughness),
              _k(float(glm::pow(p_roughness + 1.f, 2)) / 8.f),
              _F0(p_F0){};

        Vec3f evaluate(const Vec3f &n, const Vec3f &wi, const Vec3f &wo) const
        {
            Vec3f h = normalize(wi + wo);
            Vec3f f = F(wo, h);
            float d = D(n, h);
            float g = G(n, wi, wo);
            return f * d * g / (4.f * dot(wo, n) * dot(wi, n));
        }

        Vec3f getF0() const
        {
            return _F0;
        }

    protected:
        /* The Normal Distribution Function (NDF) */
        float D(const Vec3f &n, const Vec3f &h) const
        {
            float alpha2 = _alpha * _alpha;
            return alpha2 * INV_PIf / pow(((dot(n, h) * dot(n, h)) * (alpha2 - 1) + 1), 2.f);
        }

        /* The Geometric occlusion function */
        float G(const Vec3f &n, const Vec3f &wi, const Vec3f &wo) const
        {
            return G1(dot(n, wo)) * G1(dot(n, wi));
        }

        float G1(const float x) const
        {
            return x / (x * (1 - _k) + _k);
        }

        Vec3f F(const Vec3f wo, const Vec3f h) const
        {
            return _F0 + (1.f - _F0) * pow(1.f - dot(wo, h), 5.f);
        }

    private:
        float _alpha;
        float _k;
        Vec3f _F0;
    };
}

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__