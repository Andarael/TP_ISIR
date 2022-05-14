#ifndef __RT_ISICG_PERSPECTIVE_CAMERA__
#define __RT_ISICG_PERSPECTIVE_CAMERA__

#include "cameras/BaseCamera.hpp"

namespace RT_ISICG
{
    class PerspectiveCamera : public BaseCamera
    {

        // todo the view is not zoomed enough compared to the TP
    public:
        PerspectiveCamera(float p_aspectRatio);

        PerspectiveCamera(const Vec3f &p_position, const Vec3f &p_lookAt, const Vec3f &p_up, float p_fovy = 60.f, float p_aspectRatio = 1.f);

        PerspectiveCamera(const Vec3f &p_position, const Vec3f &p_lookAt, const float p_fovy = 60.f, const float p_aspectRatio = 1.f)
            : PerspectiveCamera(p_position, p_lookAt, VEC3F_Y, p_fovy, p_aspectRatio){};

        Ray generateRay(const float p_sx, const float p_sy) const override
        {
            Vec3f origin = _position;

            Vec3f posOnScreen = _viewportTopLeftCorner + (p_sx * _viewportU) - (p_sy * _viewportV);

            Vec3f direction = normalize(posOnScreen - origin);

            return Ray(origin, direction, RayType::camera);
        }

        void setAspectRatio(const float aspect_ratio) override
        {
            _aspectRatio = aspect_ratio;
            _updateViewport();
        }

        void displayCamera() const override;

    private:
        void _updateViewport();

    private:
        float _fovy = 60.f;
        float _focalDistance = 1.f;
        float _aspectRatio = 1.f;

        // Local coordinates system
        Vec3f _u = VEC3F_X;
        Vec3f _v = VEC3F_Y;
        Vec3f _w = -VEC3F_Z;

        // Viewport data
        Vec3f _viewportTopLeftCorner = VEC3F_ZERO; // Top left corner position
        Vec3f _viewportU = VEC3F_ZERO;             // Horizontal vector
        Vec3f _viewportV = VEC3F_ZERO;             // Vertical vector
        Vec3f _screenCenter = VEC3F_ZERO;          // Screen center
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_PERSPECTIVE_CAMERA__
