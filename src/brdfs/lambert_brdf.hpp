#ifndef __RT_ISICG_BRDF_LAMBERT__
#define __RT_ISICG_BRDF_LAMBERT__

#include "defines.hpp"

namespace RT_ISICG
{
    class LambertBRDF
    {
    public:
        /*kd ha been moved to the material*/
        LambertBRDF() = default;

        static float evaluate()
        {
            return INV_PIf;
        }
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_LAMBERT__
