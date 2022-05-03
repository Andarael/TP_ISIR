#include "PerspectiveCamera.hpp"

namespace RT_ISICG
{
    PerspectiveCamera::PerspectiveCamera(const float p_aspectRatio)
        : _aspectRatio(p_aspectRatio)
    {
        _updateViewport();
    }

    PerspectiveCamera::PerspectiveCamera(const Vec3f &p_position, const Vec3f &p_lookAt, const Vec3f &p_up, const float p_fovy, const float p_aspectRatio)
        : BaseCamera(p_position), _fovy(p_fovy), _aspectRatio(p_aspectRatio)
    {
        _w = glm::normalize(_position - p_lookAt);
        _u = glm::normalize(glm::cross(p_up, _w));
        _v = glm::normalize(glm::cross(_w, _u));
        _updateViewport();
    }

    void PerspectiveCamera::displayCamera() const
    {
        std::cout << " ===== camera ====== " << std::endl;
        std::cout << "      position : " << glm::to_string(_position) << std::endl;
        std::cout << "      _screenCenter : " << glm::to_string(_screenCenter) << std::endl;
        std::cout << "      _viewportTopLeftCorner : " << glm::to_string(_viewportTopLeftCorner) << std::endl;
        std::cout << "      _viewportU : " << glm::to_string(_viewportU) << std::endl;
        std::cout << "      _viewportV : " << glm::to_string(_viewportV) << std::endl;
        std::cout << "      _u : " << glm::to_string(_u) << std::endl;
        std::cout << "      _v : " << glm::to_string(_v) << std::endl;
        std::cout << "      _w : " << glm::to_string(_w) << std::endl;
        std::cout << " ================== " << std::endl;
    }

    void PerspectiveCamera::_updateViewport()
    {
        _screenCenter = _position - (_w * _focalDistance);

        float viewPortHeight = glm::tan(glm::radians(_fovy / 2)) * _focalDistance * 2.0f;
        float viewPortWidth = viewPortHeight * _aspectRatio;

        _viewportV = _v * viewPortHeight;
        _viewportU = _u * viewPortWidth;

        _viewportTopLeftCorner = _screenCenter + _viewportV * 0.5f;
        _viewportTopLeftCorner = _viewportTopLeftCorner - _viewportU * 0.5f;
    }
} // namespace RT_ISICG
