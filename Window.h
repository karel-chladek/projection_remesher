#pragma once

#include <memory>
#include <string>
#include <QString>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QPoint>

#include "Model.h"
#include "Camera.h"

class Window : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window() { makeCurrent(); }
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void addModels(const std::string& filepath);

protected slots:
    void update();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void compileShaderProgram(const QString& vertexFilepath, const QString& fragmentFilePath);
    
    std::unique_ptr<QOpenGLShaderProgram> _program;
    std::vector<Model> _models;

    Camera _camera;
    QMatrix4x4 _projection;
    QMatrix4x4 _modelMatrix;

    QPoint _lastPos;
};