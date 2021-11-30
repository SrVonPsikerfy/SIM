#pragma once

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "core.hpp"
#include "../render/RenderUtils.hpp"

namespace maths {

    const double PI = 3.14159265;

    static float radians(float const& angle) {
        return angle * PI / 180.0;
    }

    template<typename T>
    static T random(const T& min, const T& max) {
        return (max - min) * ((T)rand() / RAND_MAX) + min;
    };

    template<typename T>
    static T random(const T& max) {
        return (max) * ((T)rand() / RAND_MAX);
    };

    static Vector3 random(const Vector3& v1, const Vector3& v2) {
        return Vector3(random(v1.x, v2.x), random(v1.y, v2.y), random(v1.z, v2.z));
    }

    static Vector4 random(const Vector4& v1, const Vector4& v2) {
        return Vector4(random(v1.x, v2.x), random(v1.y, v2.y), random(v1.z, v2.z), random(v1.w, v2.w));
    }
}