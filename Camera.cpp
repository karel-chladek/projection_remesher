#include <QMatrix4x4>
#include <QVector3D>

#include "Camera.h"

Camera::Camera()
    : _eye(0, 5, 20)
    , _target(0, 0, 0)
    , _up(0, 1, 0) { }

QMatrix4x4 Camera::getMatrix() const {
    QMatrix4x4 matrix;
    matrix.lookAt(_eye, _target, _up);
    return matrix;
}

void Camera::rotate(const QQuaternion& q) {
    _eye = q * _eye;
    _up = q * _up;
}

void Camera::zoom(float factor) {
    _eye -= _eye.normalized() * factor;
}
