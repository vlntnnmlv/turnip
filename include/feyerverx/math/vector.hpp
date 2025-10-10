// Copyright 2025 Valentin Namleev

#pragma once

#include <array>
#include <initializer_list>
#include <iostream>
#include <type_traits>

#include "feyerverx/error.hpp"

namespace feyerverx {
template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <arithmetic T, size_t N> struct Vector {
private:
    std::array<T, N> coordinates;

public:
    Vector() { coordinates.fill(T{}); }

    explicit Vector(std::initializer_list<T> values) {
        if (values.size() != N) {
            throw new FeyException(ErrorType::WrongNumberOfArguments,
                                   "Wrong number of arguments in Vector<T, N> constructor");
        }

        std::copy(values.begin(), values.end(), coordinates.begin());
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector &obj) {
        os << "Vector<";
        for (auto it = obj.coordinates.begin(); it != (obj.coordinates.end() - 1); it++)
            os << *it << ", ";

        os << *(obj.coordinates.end() - 1) << ">";

        return os;
    }

    T &operator[](size_t i) { return coordinates[i]; }
    const T &operator[](size_t i) const { return coordinates[i]; }

    Vector operator+(const Vector &rhs) const {
        Vector result;
        for (size_t i = 0; i < N; ++i)
            result[i] = coordinates[i] + rhs[i];
        return result;
    }

    Vector &operator+=(const Vector &rhs) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] += rhs[i];
        return *this;
    }

    Vector operator-(const Vector &rhs) const {
        Vector result;
        for (size_t i = 0; i < N; ++i)
            result[i] = coordinates[i] - rhs[i];
        return result;
    }

    Vector &operator-=(const Vector &rhs) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] -= rhs[i];
        return *this;
    }

    Vector operator*(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; ++i)
            result[i] = coordinates[i] * scalar;
        return result;
    }

    Vector &operator*=(T scalar) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] *= scalar;
        return *this;
    }

    Vector operator/(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; ++i)
            result[i] = coordinates[i] / scalar;
        return result;
    }

    Vector &operator/=(T scalar) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] /= scalar;
        return *this;
    }

    T dot(const Vector &rhs) const {
        T result{};
        for (size_t i = 0; i < N; ++i)
            result += coordinates[i] * rhs[i];
        return result;
    }

    T magnitude() const { return std::sqrt(dot(*this)); }
};

template <typename T> struct Vector<T, 2> {
    union {
        struct {
            T x, y;
        };
        std::array<T, 2> coordinates;
    };

    Vector() : x(T{}), y(T{}) {}
    Vector(T x, T y) : x(x), y(y) {}

    friend std::ostream &operator<<(std::ostream &os, const Vector &obj) {
        os << "Vector2<" << obj.x << ", " << obj.y << ">";
        return os;
    }

    T &operator[](size_t i) { return coordinates[i]; }
    const T &operator[](size_t i) const { return coordinates[i]; }

    Vector operator+(const Vector &rhs) const { return {x + rhs.x, y + rhs.y}; }
    Vector &operator+=(const Vector &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vector operator-(const Vector &rhs) const { return {x - rhs.x, y - rhs.y}; }
    Vector &operator-=(const Vector &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vector operator*(T scalar) const { return {x * scalar, y * scalar}; }
    Vector &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector operator/(T scalar) const { return {x / scalar, y / scalar}; }
    Vector &operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    T dot(const Vector &rhs) const { return x * rhs.x + y * rhs.y; }
    T magnitude() const { return std::sqrt(Dot(*this)); }
};

template <typename T> struct Vector<T, 3> {
    union {
        struct {
            T x, y, z;
        };
        std::array<T, 3> coordinates;
    };

    Vector() : x(T{}), y(T{}), z(T{}) {}
    Vector(T x, T y) : x(x), y(y), z(z) {}

    friend std::ostream &operator<<(std::ostream &os, const Vector &obj) {
        os << "Vector3<" << obj.x << ", " << obj.y << ", " << obj.z << ">";
        return os;
    }

    T &operator[](size_t i) { return coordinates[i]; }
    const T &operator[](size_t i) const { return coordinates[i]; }

    Vector operator+(const Vector &rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    Vector &operator+(const Vector &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    Vector operator-(const Vector &rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    Vector &operator-(const Vector &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    Vector operator*(T scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    Vector &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector operator/(T scalar) const { return {x / scalar, y / scalar, z / scalar}; }
    Vector &operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    T dot(const Vector &rhs) const { return x * rhs.x + y * rhs.y; }
    T magnitude() const { return std::sqrt(Dot(*this)); }
};

using Vector2f = Vector<float, 2>;
using Vector2i = Vector<int, 2>;
using Vector3f = Vector<float, 3>;
using Vector3i = Vector<int, 3>;
} // namespace feyerverx
