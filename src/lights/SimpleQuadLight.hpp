#ifndef __RT_ISICG_SIMPLE_QUAD_LIGHT__
#define __RT_ISICG_SIMPLE_QUAD_LIGHT__

#include "lights/QuadLight.hpp"

namespace RT_ISICG
{

    class SimpleQuadLight : public QuadLight
    {
    public:
        SimpleQuadLight() = delete;

        SimpleQuadLight(Vec3f p_color, float power, float p_area, Vec3f p_position, Vec3f p_normal)
            : QuadLight(p_color, power, p_position, VEC3F_ZERO, VEC3F_ZERO), _length(glm::sqrt(p_area))
        {
            Vec3f vec = Vec3f(0, 1, 1);
            _u = _length * cross(p_normal, vec);
            _v = _length * normalize(cross(_u, p_normal));
            _u = _length * normalize(cross(_v, p_normal));
            _normal = normalize(p_normal);
            _area = p_area;

            _position = _position + (normalize(_v) - normalize(_u)) * 0.5f * _length;

            displayClass();
        }

    protected:
        float _length;
    };

}

#endif
