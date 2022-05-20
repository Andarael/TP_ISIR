#ifndef __RT_ISICG_UTILS__
#define __RT_ISICG_UTILS__
#include "defines.hpp"

namespace RT_ISICG
{
    static float fresnel(const Vec3f &p_I, const Vec3f &p_N, const float n1 = 1.0f, const float n2 = 1.45f)
    {
        float cosI = glm::dot(p_N, p_I); // cos of incoming

        float r = n1 / n2;
        float sinT = r * r * (1.0f - cosI * cosI); // sin of Transmitted (sin = 1-cos�)

        if (sinT > 1.0f) // no refraction
            return 1.0f;

        float cosT = glm::sqrt(glm::max(0.0f, 1.0f - sinT * sinT)); // cos of Transmitted
        cosI = glm::abs(cosI);

        float Rs = ((n2 * cosI) - (n1 * cosT)) / ((n2 * cosI) + (n1 * cosT));
        float Rp = ((n1 * cosI) - (n2 * cosT)) / ((n1 * cosI) + (n2 * cosT));

        return glm::clamp((Rs * Rs + Rp * Rp) / 2.0f, 0.f, 1.f);
    }

    static void getTanBitan(const Vec3f &p_normal, Vec3f &zAxis, Vec3f &xAxis, Vec3f &yAxis)
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
    static Vec3f getRandomHemisphere1(const Vec3f &p_normal)
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

    static Vec3f getRandomHemisphere2(const Vec3f &p_normal)
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

    static Vec3f getRandomHemisphere3(const Vec3f &p_normal)
    {
        float x = (randomFloat() - .5f) * 2.f;
        float y = (randomFloat() - .5f) * 2.f;
        float z = (randomFloat() - .5f) * 2.f;
        Vec3f dir = glm::normalize(Vec3f(x, y, z));
        if (glm::dot(dir, p_normal) < 0)
            dir = -dir;
        return dir;
    }

    static Vec3f getRandomHemisphere(const Vec3f &p_normal)
    {
        return getRandomHemisphere1(p_normal);
        // return getRandomHemisphere3(p_normal);
        // return getRandomHemisphere2(p_normal);
    }

} // namespace RT_ISICG

#endif // __RT_ISICG_UTILS__