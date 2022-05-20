#ifndef __RT_ISICG_BRDF_ORENNAYAR__
#define __RT_ISICG_BRDF_ORENNAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
    class OrenNayarBRDF
    {
    public:
        OrenNayarBRDF() = default;

        OrenNayarBRDF(const float p_roughness)
            : _roughness(p_roughness){};

        float evaluate(const Vec3f &normal, const Vec3f &wi, const Vec3f &wo, const float roughness) const
        {
            float sigma2 = roughness * roughness;

            // dot product of the normal and the wi and wo todo check if max is needed
            float cosThetaIn = glm::max(0.f, dot(normal, wi));
            float cosThetaOut = glm::max(0.f, dot(normal, wo));

            // get theta angles in spherical coordinates from wi and wo
            float thetaI = glm::acos(cosThetaIn);
            float thetaO = glm::acos(cosThetaOut);

            // projection of the incoming and outgoing vectors onto the surface
            Vec3f incoming = glm::normalize(wi - cosThetaIn * normal);
            Vec3f outgoing = glm::normalize(wo - cosThetaOut * normal);
            float dotDiff = glm::dot(incoming, outgoing);
            // cos (Phi_I - Phi_O) = dot(i, o) (page 5 of https://boksajak.github.io/files/CrashCourseBRDF.pdf)

            // alpha and beta factors
            float alpha = glm::max(thetaI, thetaO);
            float beta = glm::min(thetaI, thetaO);

            // oren-nayar factor compute

            float A = 1.f - 0.5f * sigma2 / (sigma2 + 0.33f);
            float B = 0.45f * sigma2 / (sigma2 + 0.09f);
            float factor = A + B * glm::max(0.f, dotDiff) * glm::sin(alpha) * glm::tan(beta);

            return INV_PIf * factor;
        }

        float evaluate(const Vec3f &normal, const Vec3f &wi, const Vec3f &wo) const
        {
            return evaluate(normal, wi, wo, _roughness);
        }

        const float &getRoughness() const { return _roughness; }

    private:
        float _roughness = 0.0f;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_ORENNAYAR__