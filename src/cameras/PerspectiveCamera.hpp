#ifndef __RT_ISICG_PERSPECTIVE_CAMERA__
#define __RT_ISICG_PERSPECTIVE_CAMERA__

#include "BaseCamera.hpp"

#include "glm/gtx/string_cast.hpp"

namespace RT_ISICG
{
    class PerspectiveCamera : public BaseCamera
    {

        // todo the view is not zoomed enough compared to the TP
    public:
        PerspectiveCamera(float p_aspectRatio);

        PerspectiveCamera(const Vec3f &p_position, const Vec3f &p_lookAt, const Vec3f &p_up, float p_fovy, float p_aspectRatio);

        PerspectiveCamera(const Vec3f &p_position, const Vec3f &p_lookAt, float p_fovy, float p_aspectRatio)
            : PerspectiveCamera(p_position, p_lookAt, Vec3f(0, 1, 0), p_fovy, p_aspectRatio){};

        Ray generateRay(const float p_sx, const float p_sy) const override
        {
            Vec3f origin = _position;

            Vec3f posOnScreen = _viewportTopLeftCorner + (p_sx * _viewportU) - (p_sy * _viewportV);

            Vec3f direction = normalize(posOnScreen - origin);

            return Ray(origin, direction);
        }

        void displayCamera() const;

    private:
        void _updateViewport();

    private:
        float _fovy = 60.f;
        float _focalDistance = 1.f;
        float _aspectRatio = 1.f;

        // Local coordinates system
        Vec3f _u = Vec3f(1, 0, 0);
        Vec3f _v = Vec3f(0, 1, 0);
        Vec3f _w = Vec3f(0, 0, -1);

        // Viewport data
        Vec3f _viewportTopLeftCorner = VEC3F_ZERO; // Top left corner position
        Vec3f _viewportU = VEC3F_ZERO;             // Horizontal vector
        Vec3f _viewportV = VEC3F_ZERO;             // Vertical vector
        Vec3f _screenCenter = VEC3F_ZERO;          // Screen center
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_PERSPECTIVE_CAMERA__
