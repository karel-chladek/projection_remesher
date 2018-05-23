#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <QOpenGLShaderProgram>

#include "Model.h"

class ObjHandler {
public:
    static std::vector<Model> loadObj(const std::string& filepath, QOpenGLShaderProgram& program);
    static void loadObj(const std::string& filepath,
                             QOpenGLShaderProgram& program,
                             std::vector<Model>& models);

    // static void saveObj(const Model& model, const std::string& filepath);
    // static void saveObj(const std::vector<Model>& models, const std::string& filepath);

private:
    static void handleVertexAttribute(Model& model, std::stringstream& line);
    static void handleFace(Model& model, std::stringstream& line);
    static void handleVertex(Model& model, std::stringstream& line);
    static void handleNormal(Model& model, std::stringstream& line);
    static void handleTexture(Model& model, std::stringstream& line);

};