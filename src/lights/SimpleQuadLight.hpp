#ifndef __RT_ISICG_SIMPLE_QUAD_LIGHT__
#define __RT_ISICG_SIMPLE_QUAD_LIGHT__

#include "lights/QuadLight.hpp"

namespace RT_ISICG
{
    class SimpleQuadLight : public QuadLight
    {
    public:
        SimpleQuadLight() = delete;

        SimpleQuadLight(const Vec3f p_color, const float power, const float p_size, const Vec3f &p_position, const Vec3f &p_normal = Vec3f(0, -1, 0))
            : QuadLight(p_color, power, p_position, VEC3F_ZERO, VEC3F_ZERO),
              _origin(p_position),
              _length(p_size)
        {
            compute_uv(p_size, p_normal);
        }

        void setLookAt(const Vec3f &lookAt) { compute_uv(_length * _length, glm::normalize(lookAt - _origin)); }

    protected:
        Vec3f _origin; // the center of the square, _position will be used to compute the uv in sample()
        float _length;

        float _getPdf(const float distance, const float cosTheta) const override
        {
            return 1.f / 2.f * (distance * distance) / cosTheta; // default value calibrated to point light
        }

    private:
        void compute_uv(const float p_size, const Vec3f &p_normal)
        {
            Vec3f vec = VEC3F_X;

            if (glm::abs(dot(vec, p_normal)) <= float(SHADOW_EPSILON))
                vec = VEC3F_Z;

            _u = _length * cross(p_normal, vec);
            _v = _length * normalize(cross(_u, p_normal));
            _u = _length * normalize(cross(_v, p_normal));
            _area = p_size * p_size;
            _normal = normalize(p_normal);
            _position = _origin + (normalize(_v) - normalize(_u)) * 0.5f * _length;
        }
    };

} // namespace RT_ISICG

#endif
