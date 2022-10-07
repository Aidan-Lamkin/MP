//
// Created by Aidan Lamkin on 10/5/22.
//

#ifndef A3_ARCBALLCAM_HPP
#define A3_ARCBALLCAM_HPP

#include <CSCI441/Camera.hpp>
#include <cmath>

namespace CSCI441 {
    class ArcballCam : public CSCI441::Camera {
    public:
        void recomputeOrientation() final;
        void zoom(GLfloat movementFactor);
        void moveForward(GLfloat movementFactor);
        void moveBackward(GLfloat movementFactor);
    private:
        void _updateArcballCameraViewMatrix();
    };
}

inline void CSCI441::ArcballCam::recomputeOrientation() {
    _direction = glm::vec3(
            _radius * sin(_theta) * sin(_phi),
            -1 * _radius * cos(_phi),
            -1 * _radius * cos(_theta) * sin(_phi));
    _position = _direction + _lookAtPoint;
    computeViewMatrix();
}

inline void CSCI441::ArcballCam::zoom(GLfloat movementFactor) {
    _radius += movementFactor;
    if(_radius < 0){
        _radius = 0;
    }
    recomputeOrientation();
}

inline void CSCI441::ArcballCam::moveForward(GLfloat movementFactor) {}

inline void CSCI441::ArcballCam::moveBackward(GLfloat movementFactor) {}

inline void CSCI441::ArcballCam::_updateArcballCameraViewMatrix() {}

#endif //A3_ARCBALLCAM_HPP

