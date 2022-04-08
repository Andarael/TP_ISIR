#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
    class PhongBrdf
    {
    public:
        PhongBrdf(const Vec3f &p_ks, const float p_shininess)
            : _ks(p_ks), _shininess(p_shininess){};

        Vec3f evaluate(const Vec3f nrm, const Vec3f wi, Vec3f wo) const
        {
            Vec3f spec;

            if (blinnPhong)
            {
                // todo test this implementation
                Vec3f halfVectr = normalize(wo + wi);
                spec = _ks * glm::pow(glm::max(dot(nrm, halfVectr), 0.f), _shininess);
            }
            else
            {
                Vec3f reflectDir = glm::reflect(-wi, nrm);
                spec = _ks * glm::pow(glm::max(dot(wo, reflectDir), 0.f), _shininess);
            }
            return spec;
        }

    private:
        Vec3f _ks;
        float _shininess;
        bool blinnPhong = false;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__