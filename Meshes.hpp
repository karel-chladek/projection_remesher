#include "Model.h"

class IcoSphere {
using Lookup=std::map<std::pair<unsigned int, unsigned int>, unsigned int>;

static constexpr const float X=.525731112119133606f;
static constexpr const float Z=.850650808352039932f;
static constexpr const float N=0.f;
std::vector<geometry::Vec3<float>> _vertices=
{
  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
};
std::vector<geometry::Vec3<unsigned int>> _triangles=
{
  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

 
unsigned int vertex_for_edge(Lookup& lookup,
    std::vector<geometry::Vec3<float>>& vertices,
    unsigned int first, 
    unsigned int second)
{
    using std::swap;

    Lookup::key_type key(first, second);
    if (key.first>key.second)
        swap(key.first, key.second);
 
    auto inserted=lookup.insert({key, vertices.size()});
    if (inserted.second){
        auto& edge0=vertices[first];
        auto& edge1=vertices[second];
        auto point= geometry::normalize(edge0+edge1) ;
        vertices.push_back(point);
    }
 
  return inserted.first->second;
}

std::vector<geometry::Vec3<unsigned int>> subdivide(std::vector<geometry::Vec3<float>>& vertices,
  std::vector<geometry::Vec3<unsigned int>> triangles)
{
  Lookup lookup;
  std::vector<geometry::Vec3<unsigned int>> result;
 
  for (auto&& each:triangles)
  {
    std::array<unsigned int, 3> mid;
    mid[0]=vertex_for_edge(lookup, vertices,
        each.x, each.y);
    mid[1]=vertex_for_edge(lookup, vertices,
        each.y, each.z);
    mid[2]=vertex_for_edge(lookup, vertices,
        each.z, each.x);
 
    result.push_back({each.x, mid[0], mid[2]});
    result.push_back({each.y, mid[1], mid[0]});
    result.push_back({each.z, mid[2], mid[1]});
    result.push_back({mid[0], mid[1], mid[2]});
  }
 
  return result;
}

public:
Model get(QOpenGLShaderProgram& program, float radius, unsigned int subdivisions){
    for (unsigned int i=0; i < subdivisions; ++i){
        _triangles = subdivide(_vertices, _triangles);
    }
    Model result(program);
    for(const auto & v : _vertices){
        result.addVertex(v);
    }
    result.uniformScale(radius);
    unsigned index = 0;
    for(const auto & t : _triangles){
        auto normal = geometry::getNormal(
                                    _vertices[t.x],
                                    _vertices[t.y],
                                    _vertices[t.z]);
        result.addNormal(normal);
        result.addIndexPack({t.x, {}, index});
        result.addIndexPack({t.y, {}, index});
        result.addIndexPack({t.z, {}, index});
        index++;
    }
    return result;
}
};
