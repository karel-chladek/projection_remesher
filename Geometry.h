#pragma once
#include <type_traits>
#include <vector>
#include <numeric>
#include <functional> 
#include <cmath>

namespace geometry {
    
template <typename T>
struct Vec3 {
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, 
        "Vec3 can hold only numeric values");
public:
    using value_t = T;
    Vec3() = default;
    Vec3(T x, T y, T z) : x(x), y(y), z(z) { }

    T lengthSquared() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(lengthSquared()); }
    bool isNullVector(T e) const {
        return x <= e && x >= -e && y <= e && y >= -e && z <= e && z >= -e;
    }

    bool operator==(const Vec3& v) const {
        if constexpr (std::is_floating_point_v<T>){
            return (*this - v).isNullVector(std::numeric_limits<T>::epsilon());
        } 
        return x == v.x && y == v.y && z = v.z;
    }
    bool operator!=(const Vec3& v) const { return !(*this == v); }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& v) {
        return *this += -v;
    }

    Vec3& operator*=(const Vec3 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Vec3& operator*=(T scalar) {
        return *this *= {scalar,scalar,scalar};
    }

    Vec3& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    Vec3<T> operator-() const{
        return -1*(*this);
    }    

    friend Vec3<T> operator+(Vec3<T> lhs, const Vec3<T>& rhs) {
        return lhs += rhs;
    }

    friend Vec3<T> operator-(Vec3<T> lhs, const Vec3<T>& rhs) {
        return lhs -= rhs;
    }

    friend Vec3<T> operator*(Vec3<T> lhs, const Vec3<T>& rhs) {
        return lhs *= rhs;
    }
    friend Vec3<T> operator*(Vec3<T> lhs, T scalar) {
        return lhs *= scalar;
    }

    friend Vec3<T> operator*(T scalar, Vec3<T> rhs) {
        return rhs *= scalar;
    }

    friend Vec3<T> operator/(Vec3<T> lhs, T scalar) {
        return lhs /= scalar;
    }
    friend bool operator<(const Vec3<T>& lhs, const Vec3<T>& rhs){ 
        if(lhs.x != rhs.x) return lhs.x < rhs.x;
        if(lhs.y != rhs.y) return lhs.y < rhs.y;
        return lhs.z < rhs.z;
    }

    friend std::ostream& operator<<(std::ostream& os, const geometry::Vec3<T>& vec) {
        os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << std::endl;
        return os;
    }

    static Vec3<float> max_vector(){
        return {std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max()};
    }
    T x, y, z = 0;
};

template <typename T>
Vec3<T> normalize(Vec3<T> lhs) {
    return lhs /= lhs.length();
}

template <typename T>
Vec3<float> inversion(const Vec3<T>& lhs) {
    return {
        1 / lhs.x,
        1 / lhs.y,
        1 / lhs.z,
    };    
}

template <typename T>
T dot(const Vec3<T>& lhs, const Vec3<T>& rhs) {
    return lhs.x * rhs.x + 
           lhs.y * rhs.y +
           lhs.z * rhs.z;
}

template <typename T>
Vec3<T> cross(const Vec3<T>& lhs, const Vec3<T>& rhs) {
    return Vec3<T>(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
template <typename T>
float distance(const Vec3<T>& lhs, const Vec3<T>& rhs) {
    return (lhs - rhs).length();
}

template<typename T>
Vec3<T> getNormal(const Vec3<T> &a, const Vec3<T> &b, const Vec3<T> &c){
    return cross(b - a, c - a);
}

template<typename Container>
typename Container::value_type getCentroid(const Container& vertices){
    using vector_t = typename Container::value_type;
    if(vertices.empty()){
        return {};
    }
    auto result = std::accumulate(vertices.begin(), vertices.end()
                            , vector_t(), std::plus<vector_t >());
    return result /= vertices.size();
    
}
template<typename Container>
typename Container::value_type::value_t getRadius(const typename Container::value_type &center, const Container& vertices){
    typename Container::value_type::value_t maxDist = 0;
    for(const auto &x: vertices){
        float dist = distance(center,x);
        if(dist > maxDist){
            maxDist = dist;
        }
    }
    return maxDist;
}
template<typename Container>
typename Container::value_type::value_t getRadius(const Container& vertices){
return getRadius(getCentroid(vertices), vertices);
}


template<typename T>
Vec3< T > sumVectors(const std::vector< Vec3< T > >& vertices){
    return std::accumulate(vertices.begin(), vertices.end()
                            , Vec3<T>(), std::plus<Vec3 < T > >());
}

} // namespace geometry