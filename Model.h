#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "Geometry.h"

struct IndexPack{
    unsigned int vertex;
    std::optional<unsigned int> texture;
    std::optional<unsigned int> normal;

    IndexPack(unsigned int v, unsigned int t, unsigned int n)
        : vertex(v)
        , texture(t)
        , normal(n) { }

    IndexPack(const std::string &segment, unsigned int vc=0, unsigned int nc=0, unsigned int tc=0){
        std::stringstream ssSegment(segment);
        if(!(ssSegment >> vertex))
            throw(std::invalid_argument("failed to read source"));
        vertex--;
        vertex -= vc;
        char trash;
        trash = ssSegment.get();
        unsigned int loader;
        if(ssSegment.peek() != '/') {
            ssSegment >> loader;
            texture = loader - 1 - tc;
        }
        
        trash = ssSegment.get();
        if(ssSegment >> loader)
            normal = loader - 1 - nc;
    };

    friend bool operator<(const IndexPack & a,const IndexPack & b){
	    if(a.vertex != b.vertex) return a.vertex < b.vertex;
	    if(a.texture != b.texture) return a.texture < b.texture;
	    return a.normal < b.normal;
    };
};

class Model : protected QOpenGLFunctions{
public:
    Model(QOpenGLShaderProgram& program);
    Model(const Model& model);
    Model& operator=(Model model);
    friend void swap(Model& first, Model& second);

    ~Model();

    void uniformScale(float ratio);
    void translate(geometry::Vec3<float> translation);
    QMatrix4x4 getMatrix();
    void draw();
    void loadToGPU();
    void deleteFromGPU();
    void clear();

    void addVertex(const geometry::Vec3<float>& v) { _vertices.push_back(v); }
    void addNormal(const geometry::Vec3<float>& n) { _normals.push_back(n); }
    void addTexture(const geometry::Vec3<float>& t) { _textures.push_back(t); }
    void addIndexPack(const IndexPack& p) { _indexPacks.push_back(p); }
    void setColor(geometry::Vec3<float> color) { _color = color; }
    void setName(const std::string& name) { _name = name; }

    const geometry::Vec3<float> &getVertex(unsigned int index) const { return _vertices.at(index); }
    geometry::Vec3<float> &getVertex(unsigned int index) { return _vertices.at(index); }
    const std::vector<geometry::Vec3<float>>& getVertices() const { return _vertices; }
    std::vector<geometry::Vec3<float>>& getVertices() { return _vertices; }

    const std::vector<geometry::Vec3<float>>& getNormals() const { return _normals; }
    std::vector<geometry::Vec3<float>>& getNormals() { return _normals; }

    const std::vector<geometry::Vec3<float>>& getTexCoords() const { return _textures; }
    std::vector<geometry::Vec3<float>>& getTexCoords() { return _textures; }

    const std::vector<unsigned int>& getIndices() const { return _indices; }
    std::vector<unsigned int>& getIndices() { return _indices; }

    const std::vector<IndexPack>& getIndexPacks() const { return _indexPacks; }
    std::vector<IndexPack>& getIndexPacks() { return _indexPacks; }


private:
    template <typename T>
    void createGPUbuffer(QOpenGLBuffer& buff, const std::vector<T>& data, QOpenGLBuffer::UsagePattern usage);
    void makeIndices();
    void loadPosition();
    void loadNormal();

    bool _onGPU = false;
    QOpenGLShaderProgram& _GPUprogram;
    QOpenGLBuffer _GPUvertices;
    QOpenGLBuffer _GPUnormals;
    QOpenGLBuffer _GPUindices;
    QOpenGLBuffer _GPUtexture;
    QOpenGLVertexArrayObject _GPUmodel;

    
    geometry::Vec3<float> _color = {0.3,0.3,0.3};

    std::string _name;

    std::vector<geometry::Vec3<float>> _vertices;
    std::vector<geometry::Vec3<float>> _normals;
    std::vector<geometry::Vec3<float>> _textures;
    std::vector<IndexPack> _indexPacks;
    std::vector<unsigned int> _indices;

}; 

