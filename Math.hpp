#include "Geometry.h"

namespace math{
    using namespace geometry;

    template <typename T>
    struct Mat3{
        using value_t = T;
        Vec3<T> _x_col;
        Vec3<T> _y_col;
        Vec3<T> _z_col;

        T getDeterminant(){
            return getDeterminant3(_x_col, _y_col, _z_col);          
        }    
    };

    template<typename T>
    T getDeterminant3(const Vec3<T> &xVec, const Vec3<T> &yVec, const Vec3<T> &zVec){
        return xVec.x*yVec.y*zVec.z +
               yVec.x*zVec.y*xVec.z +
               zVec.x*xVec.y*yVec.z -
               zVec.x*yVec.y*xVec.z -
               yVec.x*xVec.y*zVec.z -
               xVec.x*zVec.y*yVec.z;               
    }

    int signum(float a){
        return a > 0? 1 : a == 0? 0 : -1;
    }
    template<typename T>
    Vec3<T> solveSystem3(const Vec3<T> &xVec, const Vec3<T> & yVec,
                         const Vec3<T> &zVec, const Vec3<T> &resVec){
        T mainDeterminant = getDeterminant3(xVec,yVec,zVec);
        if(mainDeterminant == 0) return Vec3<T>::max_vector();
        return { getDeterminant3(resVec,yVec,zVec)/mainDeterminant,
                 getDeterminant3(xVec,resVec,zVec)/mainDeterminant,
                 getDeterminant3(xVec,yVec,resVec)/mainDeterminant };
    }

    template<typename T>
    T getFirstParameter(const Vec3<T> &xVec, const Vec3<T> & yVec,
                              const Vec3<T> &zVec, const Vec3<T> &resVec){
        T mainDeterminant = getDeterminant3(xVec,yVec,zVec);
        //dont want nonactive result to be inside triangle -> -1
        if(mainDeterminant == 0) return std::numeric_limits<T>::max();
        T y = getDeterminant3(xVec,resVec,zVec)/mainDeterminant;
        if(y < 0 || y > 1) return std::numeric_limits<T>::max();
        T z = getDeterminant3(xVec,yVec,resVec)/mainDeterminant;
        if(z < 0 || z > 1) return std::numeric_limits<T>::max();
        return getDeterminant3(resVec,yVec,zVec)/mainDeterminant;
        }
}// namespace math
