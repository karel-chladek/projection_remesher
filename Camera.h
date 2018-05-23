#pragma once

#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Camera {
public:
    Camera();

    void setEye(const QVector3D& eye) { _eye = eye; }
    void setTarget(const QVector3D& target) { _target = target; }
    void setUp(const QVector3D& up) { _up = up; }

    QMatrix4x4 getMatrix() const;
    QVector3D getEye() const { return _eye;};
    QVector3D getUp() const { return _up; };
    QVector3D getTarget() const { return _target; };
    QVector3D getLookVector() const {return _target - _eye; };

    void rotate(const QQuaternion& q);
    void zoom(float factor);


private:
    QVector3D _eye;
    QVector3D _target;
    QVector3D _up;
};