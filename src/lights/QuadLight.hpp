#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "BaseLight.hpp"

namespace RT_ISICG
{
    class QuadLight : public BaseLight
    {

    public:
        QuadLight() = delete;

        QuadLight(const Vec3f &p_color, Vec3f p_position, const float p_power = 1.f)
            : BaseLight(p_color, p_power), _position(p_position){};

    private:
        Vec3f _position;
        Vec3f _u;
        Vec3f _v;
        float _area;
    };

} // namespace RT_ISICG

#endif //__RT_ISICG_QUAD_LIGHT__