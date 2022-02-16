#include "perspective_camera.hpp"

#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
    PerspectiveCamera::PerspectiveCamera(const float p_aspectRatio) : _aspectRatio(p_aspectRatio)
    {
        _updateViewport();
    }

    PerspectiveCamera::PerspectiveCamera(const Vec3f &p_position,
                                         const Vec3f &p_lookAt,
                                         const Vec3f &p_up,
                                         const float p_fovy,
                                         const float p_aspectRatio)
        : BaseCamera(p_position), _fovy(p_fovy), _aspectRatio(p_aspectRatio)
    {
        /// TODO ! _u ? _v ? _w ?
        _updateViewport();
    }

    void PerspectiveCamera::displayCamera()
    {
        std::cout << " ===== camera ====== " << std::endl;
        std::cout << "      position : " << glm::to_string(_position) << std::endl;
        std::cout << "      center : " << glm::to_string(_center) << std::endl;
        std::cout << "      _viewportTopLeftCorner : " << glm::to_string(_viewportTopLeftCorner) << std::endl;
        std::cout << "      _viewportV : " << glm::to_string(_viewportV) << std::endl;
        std::cout << "      _viewportU : " << glm::to_string(_viewportU) << std::endl;
        std::cout << " ================== " << std::endl;
    }

    void PerspectiveCamera::_updateViewport()
    {
        Vec3f _center = _position + (-_w * _focalDistance);

        float viewPortHeight = glm::cos(glm::radians(_fovy)) * _focalDistance * 2.0f;
        float viewPortWidth = viewPortHeight * _aspectRatio;

        _viewportV = _v * viewPortHeight;
        _viewportU = _u * viewPortWidth;

        _viewportTopLeftCorner = _center + _viewportV * 0.5f;
        _viewportTopLeftCorner = _viewportTopLeftCorner - _viewportU * 0.5f;

        displayCamera();
    }

} // namespace RT_ISICG
