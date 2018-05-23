#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <QOpenGLShaderProgram>

#include "ObjHandler.h"
#include "Model.h"
#include "Geometry.h"

std::vector<Model> ObjHandler::loadObj(const std::string& filepath, QOpenGLShaderProgram& program) {

    std::vector<Model> models;
    ObjHandler::loadObj(filepath, program, models);
    return models;
}

void ObjHandler::loadObj(const std::string& filepath,
                         QOpenGLShaderProgram& program,
                         std::vector<Model>& models) {

    std::ifstream file(filepath);
    if (!file) {
        throw std::invalid_argument("invalid path to file");
    }
    std::string line;

    models.emplace_back(program);
    while(std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        switch(ss.get()) {
            case 'v':
                handleVertexAttribute(models.back(), ss);
                break;
            case 'f':
                handleFace(models.back(), ss);
                break;
            case 'o':
                models.emplace_back(program);
                break;
            default:
                break;
        }

    }

}

void ObjHandler::handleVertexAttribute(Model& model, std::stringstream& line) {
    if (line.eof()) return;
    char prefix = line.get();
    switch(prefix) {
        case 't':
            handleTexture(model, line);
            break;
        case 'n':
            handleNormal(model, line);
            break;
        case 'p':
            //not counting with
            break;
        default:
            line.putback(prefix);
            handleVertex(model, line);
            break;
    }
}


void ObjHandler::handleFace(Model& model, std::stringstream& line) {
    std::string segment;
    std::vector<IndexPack> specifiers;
    while (!line.eof()){
        line >> segment;
        specifiers.emplace_back(segment);
    }
    for(size_t i = 2; i < specifiers.size(); i++){
        if(!specifiers[i].normal || !specifiers[i - 1].normal || !specifiers[0].normal){
            geometry::Vec3<float> normal = 
                    geometry::getNormal(model.getVertex(specifiers[0].vertex), 
                                        model.getVertex(specifiers[i - 1].vertex), 
                                        model.getVertex(specifiers[i].vertex));
            model.addNormal(normal);
            unsigned long index = model.getNormals().size() - 1;
            specifiers[0].normal = index;
            specifiers[i - 1].normal = index;
            specifiers[i].normal = index;
        }
        model.addIndexPack(specifiers[0]);
        model.addIndexPack(specifiers[i-1]);
        model.addIndexPack(specifiers[i]);
    }

}
template <typename T>
geometry::Vec3<T> getVec3(std::stringstream& line){
    using geometry::Vec3;
    Vec3<T> v;
    line >> v.x  >> v.y >> v.z;
    return v;
}
void ObjHandler::handleVertex(Model& model, std::stringstream& line) {
    model.addVertex(getVec3<float>(line));
}
void ObjHandler::handleNormal(Model& model, std::stringstream& line) {
    model.addNormal(getVec3<float>(line));
}
void ObjHandler::handleTexture(Model& model, std::stringstream& line) {
    model.addTexture(getVec3<float>(line));
}