#ifndef __RT_ISICG_BRDF_ORENNAYAR__
#define __RT_ISICG_BRDF_ORENNAYAR__

// todo wtf ?
#include "defines.hpp"

namespace RT_ISICG
{
    class OrenNayarBRDF
    {
    public:
        OrenNayarBRDF(const Vec3f &p_kd, const float p_roughness)
            : _kd(p_kd), _roughness(p_roughness){};

        Vec3f evaluate(const Vec3f normal, const Vec3f wi, const Vec3f wo) const
        {
            float sigma2 = _roughness * _roughness;

            /*
            // get theta angles in spherical coordinates from wi and wo
            float cosThetaI = glm::max(0.f, dot(normal, wi));
            float cosThetaO = glm::max(0.f, dot(normal, wo));
            float tethaI = glm::acos(cosThetaI);
            float tethaO = glm::acos(cosThetaO);

            // projection of the incoming and outgoing vectors onto the surface normal
            Vec3f projectedIncoming = glm::normalize(wi - cosThetaI * normal);
            Vec3f projectedOutgoing = glm::normalize(wo - cosThetaO * normal);

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

            //
            float NrmDotIn = dot(normal, wi);
            float NrmDotOut = dot(normal, wo);

            float thetaI = acos(NrmDotIn);
            float thetaO = acos(NrmDotOut);

            Vec3f incoming = normalize(wi - NrmDotIn * normal);
            Vec3f outgoing = normalize(wo - NrmDotOut * normal);
            float dotDiff = dot(incoming, outgoing);

            float alpha = glm::max(thetaI, thetaO);
            float beta = glm::min(thetaI, thetaO);

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
        float _roughness = 0.0f;
    };
}

#endif // __RT_ISICG_BRDF_ORENNAYAR__