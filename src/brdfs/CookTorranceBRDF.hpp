#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
    class CookTorranceBrdf
    {
    public:
        CookTorranceBrdf() = default;

        CookTorranceBrdf(const float p_roughness)
            : _roughness(p_roughness),
              _k(getK(p_roughness)){};

        Vec3f evaluate(const Vec3f &n, const Vec3f &wi, const Vec3f &wo, const Vec3f &F0, const float roughness) const
        {
            float alpha = roughness * roughness;
            Vec3f h = normalize(wi + wo);
            Vec3f f = F(wo, h, F0);
            float d = D(n, h, alpha);
            float g = G(n, wi, wo, roughness);
            return f * d * g / (4.f * dot(wo, n) * dot(wi, n));
        }

        Vec3f evaluate(const Vec3f &n, const Vec3f &wi, const Vec3f &wo, const Vec3f &F0) const
        {
            return evaluate(n, wi, wo, F0, _roughness);
        }

    protected:
        /* The Normal Distribution Function (NDF) */
        float D(const Vec3f &n, const Vec3f &h, const float alpha) const
        {
            float alpha2 = alpha * alpha;
            return alpha2 * INV_PIf / pow(((dot(n, h) * dot(n, h)) * (alpha2 - 1) + 1), 2.f);
        }

        /* The Geometric occlusion function */
        float G(const Vec3f &n, const Vec3f &wi, const Vec3f &wo, const float roughness) const
        {
            float k = getK(roughness);
            return G1(dot(n, wo), k) * G1(dot(n, wi), k);
        }

        float G1(const float x, const float k) const
        {
            return x / (x * (1 - k) + k);
        }

        Vec3f F(const Vec3f &wo, const Vec3f &h, const Vec3f &F0) const
        {
            return F0 + (1.f - F0) * pow(1.f - dot(wo, h), 5.f);
        }

    private:
        float _roughness = 0.5f;
        float _k = getK(0.5f);

        float getK(const float p_roughness) const { return glm::pow(p_roughness + 1.f, 2.f) / 8.f; }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__