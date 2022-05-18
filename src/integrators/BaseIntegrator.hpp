#ifndef __RT_ISICG_BASE_INTEGRATOR__
#define __RT_ISICG_BASE_INTEGRATOR__

#include "HitRecord.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
    class Scene;

    enum class IntegratorType : int
    {
        DEBUG = 0,
        RAY_CAST = 1,
        DIRECT_LIGHT = 2,
        WHITTED = 3,
        PATHT_RACING = 4,
        COUNT // keep it last
    };

    class BaseIntegrator
    {
    public:
        BaseIntegrator() = default;

        virtual IntegratorType getType() const = 0;

        virtual ~BaseIntegrator() = default;

        // Return incoming luminance.
        virtual Vec3f Li(const Scene &p_scene, const Ray &p_ray) const = 0;

        const Vec3f &getBackgroundColor() const { return _backgroundColor; }

        void setBackgroundColor(const Vec3f &p_color) { _backgroundColor = p_color; }

        void getTanBitan(const Vec3f &p_normal, Vec3f &zAxis, Vec3f &xAxis, Vec3f &yAxis) const
        {
            Vec3f crossAxis = VEC3F_X;

            zAxis = p_normal;

            xAxis = glm::cross(zAxis, crossAxis);
            if (glm::length(xAxis) < 0.001f)
                xAxis = glm::normalize(glm::cross(zAxis, VEC3F_Y));
            xAxis = glm::normalize(xAxis);

            yAxis = glm::normalize(glm::cross(xAxis, zAxis));
        }

        // this one seems to be correct according to white furnace
        Vec3f getRandomHemisphere1(const Vec3f &p_normal) const
        {
            float rand1 = randomFloat();
            float rand2 = randomFloat();

            float r = glm::sqrt(1.f - rand1 * rand1);
            float phi = 2 * PIf * rand2;
            Vec3f sampleDir = Vec3f(glm::cos(phi) * r, glm::sin(phi) * r, rand1);

            Vec3f zAxis;
            Vec3f xAxis;
            Vec3f yAxis;
            getTanBitan(p_normal, zAxis, xAxis, yAxis);

            Vec3f direction;
            direction.x = glm::dot(Vec3f(xAxis.x, yAxis.x, p_normal.x), sampleDir);
            direction.y = glm::dot(Vec3f(xAxis.y, yAxis.y, p_normal.y), sampleDir);
            direction.z = glm::dot(Vec3f(xAxis.z, yAxis.z, p_normal.z), sampleDir);

            return direction;
        }

        Vec3f getRandomHemisphere2(const Vec3f &p_normal) const
        {
            Vec3f zAxis;
            Vec3f xAxis;
            Vec3f yAxis;
            getTanBitan(p_normal, zAxis, xAxis, yAxis);
            float a = (randomFloat() - .5f) * 2.f;
            float b = (randomFloat() - .5f) * 2.f;
            float c = randomFloat();

            Vec3f direction = yAxis * a + xAxis * b + zAxis * c;
            return glm::normalize(direction);
        }

        Vec3f getRandomDirection(const Vec3f &p_normal) const
        {
            return getRandomHemisphere1(p_normal);
            // return getRandomHemisphere2(p_normal);
        }

    protected:
        Vec3f _backgroundColor = BLACK;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_INTEGRATOR__
