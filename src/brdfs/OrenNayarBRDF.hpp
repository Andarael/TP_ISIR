#ifndef __RT_ISICG_BRDF_ORENNAYAR__
#define __RT_ISICG_BRDF_ORENNAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
    class OrenNayarBRDF
    {
    public:
        OrenNayarBRDF(const Vec3f &p_kd, const float p_roughness)
            : _kd(p_kd), _roughness(p_roughness){};

        Vec3f evaluate(const Vec3f &normal, const Vec3f &wi, const Vec3f &wo) const
        {
            float sigma2 = _roughness * _roughness;

            /*
            // get theta angles in spherical coordinates from wi and wo
            float cosThetaIn = glm::max(0.f, dot(normal, wi));
            float cosThetaOut = glm::max(0.f, dot(normal, wo));
            float tethaI = glm::acos(cosThetaIn);
            float tethaO = glm::acos(cosThetaOut);

            // projection of the incoming and outgoing vectors onto the surface normal
            Vec3f projectedIncoming = glm::normalize(wi - cosThetaIn * normal);
            Vec3f projectedOutgoing = glm::normalize(wo - cosThetaOut * normal);

            // get phi angles in spherical coordinates from projected wi and wo
            float phi_i = glm::acos(dot(normal, projectedIncoming));
            float phi_o = glm::acos(dot(normal, projectedOutgoing));
            float dotDiff = glm::cos(phi_i - phi_o)

            // get alpha and beta factors
            float beta = glm::min(tethaI, tethaO);
            float alpha = glm::max(tethaI, tethaO);

            // oren-nayar factor compute
            float A = 1.f - 0.5f * sigma2 / (sigma2 + 0.33f);
            float B = 0.45f * sigma2 / (sigma2 + 0.09f);
            float factor = A + B * glm::max(0.f, dotDiff) * glm::sin(alpha) * glm::tan(beta);
            */

            // dot product of the normal and the wi and wo
            float cosThetaIn = dot(normal, wi);
            float cosThetaOut = dot(normal, wo);

            // get theta angles in spherical coordinates from wi and wo
            float thetaI = acos(cosThetaIn);
            float thetaO = acos(cosThetaOut);

            // projection of the incoming and outgoing vectors onto the surface
            Vec3f incoming = normalize(wi - cosThetaIn * normal);
            Vec3f outgoing = normalize(wo - cosThetaOut * normal);
            float dotDiff = dot(incoming, outgoing);
            // cos (Phi_I - Phi_O) = dot(i, o) (page 5 of https://boksajak.github.io/files/CrashCourseBRDF.pdf)

            // alpha and beta factors
            float alpha = glm::max(thetaI, thetaO);
            float beta = glm::min(thetaI, thetaO);

            // oren-nayar factor compute
            float A = 1.f - 0.5f * sigma2 / (sigma2 + 0.33f);
            float B = 0.45f * sigma2 / (sigma2 + 0.09f);
            float factor = A + B * glm::max(0.f, dotDiff) * glm::sin(alpha) * glm::tan(beta);

            return _kd * INV_PIf * factor;
        }

        const Vec3f &getKd() const
        {
            return _kd;
        }

        const float &getRoughness() const
        {
            return _roughness;
        }

    private:
        Vec3f _kd = WHITE;
        // todo move kd to material
        float _roughness = 0.0f;
    };
}

#endif // __RT_ISICG_BRDF_ORENNAYAR__