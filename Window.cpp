#include <memory>
#include <string>
#include <cstdlib>

#include <QString>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <iostream>


#include "Window.h"
#include "ObjHandler.h"
#include "Model.h"
#include "Meshes.hpp"
#include "remesher/projection_remesher.hpp"

Window::Window(QWidget* parent) 
    : QOpenGLWidget(parent) { }

void Window::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    compileShaderProgram("shaders/main.vert", "shaders/main.frag");

    addModels("res/apple.obj");
    std::vector<Model> parts = _models;
    _models.back().setColor({0.5,0.1,0.2});
    _models.pop_back();
    // _models.back().uniformScale(0.98);
    _models.push_back(projection_remesher::remesh(parts, IcoSphere().get(*_program, 1, 3)));
    std::cout<<"bezim"<<std::endl;
    _modelMatrix.setToIdentity();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Window::resizeGL(int width, int height) {
    _projection.setToIdentity();

    float aspect = width / (float)height;
    _projection.perspective(60.0f, aspect, 0.1f, 1000.0f);
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT); 
    
    _program->bind();
    _program->setUniformValue("MVPmatrix", _projection * _camera.getMatrix()  * _modelMatrix);
    _program->setUniformValue("light_position", _camera.getEye().x(),
                                                 _camera.getEye().y(),
                                                 _camera.getEye().z());
    _program->release();

    for (Model& model : _models) {
        model.draw();
    }
    
}

void Window::addModels(const std::string& filepath) {
    ObjHandler::loadObj(filepath, *_program, _models);
}

void Window::update() {
    QOpenGLWidget::update();
}



void Window::wheelEvent(QWheelEvent* event) {
    _camera.zoom(event->delta() / 100.0);
    update();
}

void Window::mousePressEvent(QMouseEvent* event) {
    _lastPos = event->pos();
}

void Window::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() != Qt::LeftButton) return;
    QPoint diff = _lastPos - event->pos();
    QVector3D upNormalized = _camera.getUp().normalized();
    QVector3D sideNormalized = QVector3D::crossProduct(
        _camera.getUp(), 
        _camera.getLookVector()
    ).normalized();
    
    QVector3D to = (-diff.y() * upNormalized - diff.x() * sideNormalized) / 500 + _camera.getEye();
    _camera.rotate(QQuaternion::rotationTo(_camera.getEye(), to));

    update();
}


void Window::compileShaderProgram(const QString& vertexFilepath, const QString& fragmentFilepath) {
    _program = std::make_unique<QOpenGLShaderProgram>(this);
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilepath);
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilepath);
    _program->link();
    _program->bind();
}