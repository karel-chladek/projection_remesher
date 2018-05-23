#include <vector>
#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "Model.h"

Model::Model(QOpenGLShaderProgram& program)
    : _GPUprogram(program)
    , _GPUvertices(QOpenGLBuffer::VertexBuffer)
    , _GPUnormals(QOpenGLBuffer::VertexBuffer)
    , _GPUindices(QOpenGLBuffer::IndexBuffer){
    initializeOpenGLFunctions();
}

Model::~Model() {
    deleteFromGPU();
}

Model::Model(const Model& model)
    : _onGPU(false)
    , _GPUprogram(model._GPUprogram)
    , _GPUvertices(QOpenGLBuffer::VertexBuffer)
    , _GPUnormals(QOpenGLBuffer::VertexBuffer)
    , _GPUindices(QOpenGLBuffer::IndexBuffer)
    , _color(model._color)
    , _name(model._name)
    , _vertices(model._vertices)
    , _normals(model._normals)
    , _textures(model._textures)
    , _indexPacks(model._indexPacks)
    , _indices(model._indices){
        initializeOpenGLFunctions();
}

Model& Model::operator=(Model model) {
    swap(*this, model);
    return *this;
}

void swap(Model& first, Model& second) {
    using std::swap;
    swap(first._onGPU, second._onGPU);
    swap(first._GPUvertices, second._GPUvertices);
    swap(first._GPUnormals, second._GPUnormals);
    swap(first._GPUindices, second._GPUindices);
    swap(first._color, second._color);
    swap(first._name, second._name);
    swap(first._vertices, second._vertices);
    swap(first._normals, second._normals);
    swap(first._textures, second._textures);
    swap(first._indexPacks, second._indexPacks);
    swap(first._indices, second._indices);
}

void Model::uniformScale(float ratio){
    for(auto & v : _vertices){
        v *= ratio;
    }
    if(_onGPU)
        loadPosition();
}

void Model::translate(geometry::Vec3<float> translation){
    for(auto & v : _vertices){
        v += translation;
    }
    if(_onGPU)
        loadPosition();
}

QMatrix4x4 Model::getMatrix(){
    return {};
}

void Model::draw() {
    if (!_onGPU) loadToGPU();
    _GPUprogram.bind();
    _GPUmodel.bind();
    _GPUindices.bind();
    _GPUprogram.setUniformValue("color", _color.x, _color.y, _color.z);
    glDrawElements(GL_TRIANGLES, _GPUindices.size(), GL_UNSIGNED_INT, 0);
    _GPUindices.release();
    _GPUmodel.release();
    _GPUprogram.release();
}


void Model::loadToGPU() {
    _onGPU = true;
    if (!_GPUmodel.isCreated()) {
        _GPUmodel.create();
    }
    if(_indices.empty()){
        makeIndices();
    }
    _GPUmodel.bind();

    createGPUbuffer(_GPUindices, _indices, QOpenGLBuffer::StaticDraw);
    _GPUindices.release();

    loadPosition();
    loadNormal();
    _GPUmodel.release();
}

void Model::deleteFromGPU() {
    _onGPU = false;
    _GPUmodel.destroy();
    _GPUvertices.destroy();
    _GPUnormals.destroy();
    _GPUindices.destroy();
    _GPUtexture.destroy();
}

void Model::clear() {
    deleteFromGPU();
    _indices.clear();
    _vertices.clear();
    _normals.clear();
}

template <typename T>
void Model::createGPUbuffer(QOpenGLBuffer& buff, const std::vector<T>& data, QOpenGLBuffer::UsagePattern usage) {
    if (buff.isCreated()) {
        buff.bind();
        return;
    }
    buff.create();
    buff.bind();
    buff.setUsagePattern(usage);
    buff.allocate(data.data(), data.size() * sizeof(T));
}

static bool getIndex(
	const IndexPack & packed, 
	const std::map<IndexPack, unsigned int> & packToIndex,
	unsigned int &result)
{
	auto it = packToIndex.find(packed);
	if ( it == packToIndex.end() ){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void Model::makeIndices(){
    std::vector<geometry::Vec3<float>> new_vertices;
    std::vector<geometry::Vec3<float>> new_normals;
    std::vector<geometry::Vec3<float>> new_textures;

    _indices.clear();
    std::map<IndexPack, unsigned int> packToIndex;
    unsigned int index;
	for ( const auto& packed : _indexPacks){
        if ( !getIndex( packed, packToIndex, index) ){
			new_vertices.push_back( _vertices[ packed.vertex ] );
			if(!_textures.empty() && packed.texture)
                new_textures.push_back( _textures[ *packed.texture ] );
			if(packed.normal)
                new_normals.push_back( _normals[ *packed.normal ] );
			index = static_cast<unsigned int>(new_vertices.size() - 1);
			packToIndex[ packed ] = index;
		}
        _indices.push_back( index );
    }

    using std::swap;
    swap(_vertices, new_vertices);
    swap(_textures, new_textures);
    swap(_normals, new_normals);
}

void Model::loadPosition(){
    _GPUmodel.bind();
    createGPUbuffer(_GPUvertices, _vertices, QOpenGLBuffer::StaticDraw);
    _GPUprogram.enableAttributeArray("position");
    _GPUprogram.setAttributeBuffer("position", GL_FLOAT, 0, 3, 0);
    _GPUvertices.release();
    _GPUmodel.release();
}

void Model::loadNormal(){
    _GPUmodel.bind();
    createGPUbuffer(_GPUnormals, _normals, QOpenGLBuffer::StaticDraw);
    _GPUprogram.enableAttributeArray("normal");
    _GPUprogram.setAttributeBuffer("normal", GL_FLOAT, 0, 3, 0);
    _GPUnormals.release();
    _GPUmodel.release();
}
