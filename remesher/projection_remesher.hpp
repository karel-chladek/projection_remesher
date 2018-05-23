#pragma once
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>
#include <functional>
#include "../Model.h"
#include "../Geometry.h"
#include "../Math.hpp"
#include <limits>

//TODO::MEGA REFACTOR
namespace projection_remesher{
    using namespace geometry;

    struct Triangle{
        Vec3<float> vertex;
        Vec3<float> uVec;
        Vec3<float> vVec;
        float centerDist;

        Triangle(const Vec3<float> &vert, const Vec3<float> &u, const Vec3<float> &v, const Vec3<float> &c)
            : vertex(vert),
              uVec(u),
              vVec(v),
              centerDist((v-c).length()){}

        bool operator<(const Triangle& rhs) const {
            return centerDist > rhs.centerDist;
        }
    };

    Vec3<float> sceneAvgCenter(const std::vector<Model>& scene){
        unsigned count = 0;
        Vec3<float> res = {0,0,0};
        for(const auto& m : scene){
            res += sumVectors(m.getVertices());
            count += m.getVertices().size();
        }
        return res /= count;
    }

    Vec3<float> sceneBBCenter(const std::vector<Model>& scene){
        Vec3<float> maxCoord = -Vec3<float>::max_vector();
        Vec3<float> minCoord = Vec3<float>::max_vector();
        for(const auto& m : scene){
            for(const auto & v: m.getVertices()){
                if(v.x > maxCoord.x){
                    maxCoord.x = v.x;
                }
                if(v.y > maxCoord.y){
                    maxCoord.y = v.y;
                }
                if(v.z > maxCoord.z){
                    maxCoord.z = v.z;
                }

                if(v.x < minCoord.x){
                    minCoord.x = v.x;
                }
                if(v.y < minCoord.y){
                    minCoord.y = v.y;
                }
                if(v.z < minCoord.z){
                    minCoord.z = v.z;
                }
            }
        }
        return (minCoord + maxCoord)/2;
    }

    float sceneRadius(const Vec3<float> center, const std::vector<Model>& scene){
        float max = 0;
        for(const auto& m : scene){
            float currDist = getRadius(center, m.getVertices());
            if(currDist > max){
                max = currDist;
            }
        }
        return max;
    }

    std::multiset<Triangle> getTriangles(const std::vector<Model>& scene, const Vec3<float>& center){
        std::multiset<Triangle> triangles;
        for(const auto & m : scene){
            std::vector<IndexPack> iPacks = m.getIndexPacks();
            for(size_t i = 2; i < iPacks.size(); i += 3){
                auto mainVert = m.getVertex(iPacks[i-2].vertex);
                auto uVec = m.getVertex(iPacks[i].vertex) - mainVert;
                auto vVec = m.getVertex(iPacks[i - 1].vertex) - mainVert;
                triangles.insert({mainVert, uVec, vVec, center});
            }
        }
        return triangles;
    }

    Model remesh(std::vector<Model>& scene, const Model& primitive){

        Vec3<float> center = sceneBBCenter(scene);
        Vec3<float> prim_center = getCentroid(primitive.getVertices());
        Model result = primitive;
        float sceneR = sceneRadius(center, scene);
        float resultR = getRadius(result.getVertices());
        result.uniformScale(2*sceneR/resultR);
        result.translate(center - prim_center);
        
        std::multiset<Triangle> triangles = getTriangles(scene, center);

        for(auto& v : result.getVertices()){
            Vec3<float> moveDir = center - v;
            bool moved = false;
            for(const auto& t : triangles){
                float parameter = math::getFirstParameter(moveDir, -1*t.uVec, -1*t.vVec, t.vertex - v);
                if(parameter >= 0 && parameter <= 1){
                    v += parameter*moveDir;
                    moved = true;
                    break;
                }
            }
            if(!moved){
                v = center;
            }
        }
        return result;

    }
}//namespace sewer