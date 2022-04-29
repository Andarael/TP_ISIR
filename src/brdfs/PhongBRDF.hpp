#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
    class PhongBrdf
    {
    public:
        PhongBrdf(const float p_shininess, bool p_useBlinn = false)
            : _shininess(p_shininess), _blinnPhong(p_useBlinn){};

        float evaluate(const Vec3f &nrm, const Vec3f &wi, const Vec3f &wo) const
        {
            float spec;

            if (_blinnPhong)
            {
                Vec3f h = normalize(wo + wi);
                spec = glm::pow(glm::max(dot(nrm, h), 0.f), _shininess);
            }
            else
            {
                Vec3f reflectDir = glm::reflect(-wi, nrm);
                spec = glm::pow(glm::max(dot(wo, reflectDir), 0.f), _shininess);
            }
            return spec;
        }

    private:
        float _shininess;
        bool _blinnPhong = true;
    };

} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__