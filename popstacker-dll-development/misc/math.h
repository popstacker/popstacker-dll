#pragma once
#include <cmath>

enum class MouseButton : uint8_t {
    None = 0,
    Left = 1,
    Right = 2
};

enum class MouseAction : bool {
    Press = true,
    Release = false
};

template <typename T>
struct Vec2 {
    T x, y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}

    Vec2<T> operator+(const Vec2<T>& other) const { return { x + other.x, y + other.y }; }
    Vec2<T> operator-(const Vec2<T>& other) const { return { x - other.x, y - other.y }; }
    Vec2<T> operator*(T scalar) const { return { x * scalar, y * scalar }; }
    Vec2<T> operator/(T scalar) const { return { x / scalar, y / scalar }; }

    Vec2<T>& operator+=(const Vec2<T>& other) { x += other.x; y += other.y; return *this; }
    Vec2<T>& operator-=(const Vec2<T>& other) { x -= other.x; y -= other.y; return *this; }
    Vec2<T>& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2<T>& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

    T length() const { return std::sqrt(x * x + y * y); }
    Vec2<T> normalized() const { T len = length(); return len != 0 ? (*this) / len : Vec2<T>{ 0,0 }; }
};

template <typename T>
struct Vec3 {
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vec3<T> operator+(const Vec3<T>& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vec3<T> operator-(const Vec3<T>& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vec3<T> operator*(T scalar) const { return { x * scalar, y * scalar, z * scalar }; }
    Vec3<T> operator/(T scalar) const { return { x / scalar, y / scalar, z / scalar }; }

    Vec3<T>& operator+=(const Vec3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3<T>& operator-=(const Vec3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vec3<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    T length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<T> normalized() const { T len = length(); return len != 0 ? (*this) / len : Vec3<T>{ 0,0,0 }; }

    T distanceTo(const Vec3<T>& other) const {
        return (*this - other).length();
    }
};

template <typename T>
struct Vec4 {
    T x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    Vec4<T> operator+(const Vec4<T>& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
    Vec4<T> operator-(const Vec4<T>& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
    Vec4<T> operator*(T scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
    Vec4<T> operator/(T scalar) const { return { x / scalar, y / scalar, z / scalar, w / scalar }; }

    Vec4<T>& operator+=(const Vec4<T>& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
    Vec4<T>& operator-=(const Vec4<T>& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
    Vec4<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
    Vec4<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

    T length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Vec4<T> normalized() const { T len = length(); return len != 0 ? (*this) / len : Vec4<T>{ 0,0,0,0 }; }
};