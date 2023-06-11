#pragma once
#include <glm/glm.hpp>
#include "PhysX/PhysX.h"
#include <glm/gtc/constants.hpp>

namespace jd
{
    union alignas(16) Quaternion
    {
        // Data members. The following data members can be accessed publicly:
        // - glm::vec3 v, which is the same as {x, y, z}
        // - float x
        // - float y
        // - float z
        // - float w
        // - float e[4], which is the same as {x, y, z, w}
        struct
        {
            union
            {
                glm::vec3 v;
                struct
                {
                    float x;
                    float y;
                    float z;
                };
            };
            float w;
        };
        float e[4];

        static constexpr Quaternion identity(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) noexcept;
        static Quaternion           angleAxis(const float angle, const glm::vec3& axis) noexcept;
        static Quaternion           fromEuler(const glm::vec3& angles) noexcept;
        static Quaternion           lookAt(const glm::vec3& sourcePoint, const glm::vec3& destPoint) noexcept;

        // Methods
        constexpr glm::vec3         axis() const noexcept;
        glm::vec3                   unitAxis() const noexcept;
        float                       angle() const noexcept;
        float                       angleWith(const Quaternion& q) const noexcept;
        constexpr glm::vec3         rotate(const glm::vec3& v) const noexcept;
        glm::vec3                   eulerAngles() const noexcept;
        float                       sqrLength() const noexcept;
        float                       length() const noexcept;
        float                       dot(const Quaternion& q) const noexcept;
        Quaternion                  normalized() const noexcept;
        Quaternion                  safelyNormalized() const noexcept;
        void                        normalize() noexcept;
        void                        safelyNormalize() noexcept;
        constexpr Quaternion        conjugate() const noexcept;
        constexpr Quaternion        inversed() const noexcept;
        Quaternion                  to(const Quaternion& target) const noexcept;
        Quaternion                  slerp(const Quaternion& target, const float t) const noexcept;
        Quaternion                  nlerp(const Quaternion& target, const float t) const noexcept;
        bool                        isEqualTo(const Quaternion& q,  const float eps = FLT_EPSILON) const noexcept;
        constexpr glm::mat3         mat3_cast() const noexcept;
        constexpr glm::mat4         mat4_cast() const noexcept;

        // Operator overloads
        constexpr bool              operator==(const Quaternion& q) const noexcept;
        constexpr Quaternion        operator*=(const float k) const noexcept;
        constexpr Quaternion        operator/=(const float k) const noexcept;
        constexpr Quaternion        operator+(const Quaternion& q) const noexcept;
        constexpr Quaternion        operator-(const Quaternion& q) const noexcept;
        constexpr Quaternion        operator-() const noexcept;
        Quaternion                  operator*(const Quaternion& q) const noexcept;
        constexpr glm::vec3         operator*(const glm::vec3& v) const noexcept;
        constexpr Quaternion        operator*(const float k) const noexcept;
        constexpr Quaternion        operator/(const float k) const noexcept;

        operator glm::vec3() const noexcept;
    };

    using Quat = Quaternion;

    /* =================== Static methods (pseudo-constructors) =================== */
    inline constexpr Quaternion Quaternion::identity(float x, float y, float z, float w) noexcept {
        return { glm::vec3{x, y, z}, w };
    }

    inline Quaternion Quaternion::angleAxis(const float angle, const glm::vec3& axis) noexcept
    {
        const float halfAngle{ angle * 0.5f };
        return { glm::normalize(axis) * sinf(halfAngle), cosf(halfAngle) };
    }

    inline Quaternion Quaternion::fromEuler(const glm::vec3& angles) noexcept
    {
        // Half angles
        const glm::vec3 halfAngles{ angles * 0.5f };

        // Cosinus and sinus of half angles
        const float cosX{ cosf(halfAngles.x) }, sinX{ sinf(halfAngles.x) },
            cosY{ cosf(halfAngles.y) }, sinY{ sinf(halfAngles.y) },
            cosZ{ cosf(halfAngles.z) }, sinZ{ sinf(halfAngles.z) };

        // Results used more than once
        const float cosYcosZ{ cosY * cosZ }, sinYsinZ{ sinY * sinZ },
            sinYcosZ{ sinY * cosZ }, cosYsinZ{ cosY * sinZ };

        const glm::vec3 imAxis
        {
            sinX * cosYcosZ - cosX * sinYsinZ,
            cosX * sinYcosZ - sinX * cosYsinZ,
            cosX * cosYsinZ + sinX * sinYcosZ
        };

        return { imAxis, cosX * cosYcosZ + sinX * sinYsinZ };
    }

    // TODO: simplify, add const, remove casts, remove ifs, pass a "up" glm::vec3 parameter
    inline Quaternion Quaternion::lookAt(const glm::vec3& sourcePoint, const glm::vec3& destPoint) noexcept
    {
        glm::vec3 forwardVector = glm::normalize(destPoint - sourcePoint);
        float dot = glm::dot(glm::vec3{ 0.0f, 0.0f, 1.0f }, forwardVector);

        if (std::abs(dot - (-1.0f)) < 0.000001f) {
            return Quaternion::angleAxis(glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
        }
        if (std::abs(dot - (1.0f)) < 0.000001f) {
            return Quaternion::identity();
        }

        float rotAngle = (float)std::acos(dot);
        glm::vec3  rotAxis = glm::cross(glm::vec3{ 0.0f, 0.0f, 1.0f }, forwardVector);
        rotAxis = glm::normalize(rotAxis);

        return Quaternion::angleAxis(rotAngle, rotAxis);
    }
    /* =================== Methods =================== */
    inline constexpr glm::vec3 Quaternion::axis() const noexcept {
        return v;
    }

    inline glm::vec3 Quaternion::unitAxis() const noexcept {
        return glm::normalize(v);
    }

    inline float Quaternion::angle() const noexcept {
        return 2.0f * acosf(w);
    }

    inline float Quaternion::angleWith(const Quaternion& q) const noexcept {
        return 2.0f * acosf(dot(q));
    }

    inline constexpr glm::vec3 Quaternion::rotate(const glm::vec3& p) const noexcept {
        return p * (2.0f * w * w - 1.0f) + (v * glm::cross(v, p) * 2.0f) + (glm::cross(v, p) * (2.0f * w));
    }

    inline glm::vec3 Quaternion::eulerAngles() const noexcept
    {
        glm::vec3 angles;

        { // X and Y
            const float z2 = z * z;

            { // X
                const float sinXcosZ = 2.f * ((w * x) + (z * y));
                const float cosXcosZ = 1.f - 2.f * ((x * x) + z2);
                angles.x = atan2f(sinXcosZ, cosXcosZ);
            }

            { // Y
                const float sinYcosZ = 2.f * ((w * y) + (x * z));
                const float cosYcosZ = 1.f - 2.f * (z2 + (y * y));
                angles.y = atan2f(sinYcosZ, cosYcosZ);
            }
        }

        { // Z
            const float sinZ = 2.f * ((w * z) - (y * x));

            if (fabsf(sinZ) >= 1.f)
            {
                angles.z = copysignf(glm::pi<float>() / 2.0f, sinZ);
            }
            else
            {
                angles.z = asinf(sinZ);
            }
        }

        return angles;
    }


    inline float Quaternion::sqrLength() const noexcept {
        return jd::lenSqr(v) + (w * w);
    }

    inline float Quaternion::length() const noexcept {
        return sqrtf(sqrLength());
    }

    inline float Quaternion::dot(const Quaternion& q) const noexcept {
        return glm::dot(v, q.v) + w * q.w;
    }

    inline Quaternion Quaternion::normalized() const noexcept {
        return *this / length();
    }

    inline Quaternion Quaternion::safelyNormalized() const noexcept
    {
        const float sqrLen{ sqrLength() };
        return sqrLen ? *this / sqrtf(sqrLen) : *this;
    }


    inline void Quaternion::normalize() noexcept
    {
        const float reciprocal_len{ 1.0f / length() };
        v *= reciprocal_len;
        w *= reciprocal_len;
    }

    inline void Quaternion::safelyNormalize() noexcept
    {
        const float sqrLen{ sqrLength() };

        if (sqrLen) {
            *this /= sqrtf(sqrLen);
        }
    }

    inline constexpr Quaternion Quaternion::conjugate() const noexcept {
        return { -v, w };
    }


    inline constexpr Quaternion Quaternion::inversed() const noexcept {
        return conjugate() / sqrLength();
    }


    inline Quaternion Quaternion::to(const Quaternion& target) const noexcept {
        return target * inversed();
    }


    inline Quaternion Quaternion::slerp(const Quaternion& target, const float t) const noexcept
    {
        const float dotQaQb = dot(target);
        const float angle = std::acos(std::abs(dotQaQb));
        const float sign = copysignf(1.0f, dotQaQb);
        return (*this * sign * std::sin((1.0f - t) * angle) + target * std::sin(t * angle)) / std::sin(angle);
    }


    inline Quaternion Quaternion::nlerp(const Quaternion& target, const float t) const noexcept
    {
        const Quaternion q{ *this * (1.0f - t) + target * t };
        return q.normalized();
    }

    inline bool Quaternion::isEqualTo(const Quaternion& q, const float eps) const noexcept {
        return v == q.v && fabs(w - q.w) < eps;
    }

    inline constexpr glm::mat3 Quaternion::mat3_cast() const noexcept {
        glm::mat3 result{ 1.0f };
        float qxx{ x * x };
        float qyy{ y * y };
        float qzz{ z * z };
        float qxz{ x * z };
        float qxy{ x * y };
        float qyz{ y * z };
        float qwx{ w * x };
        float qwy{ w * y };
        float qwz{ w * z };

        result[0][0] = 1.0f - 2.0f * (qyy + qzz);
        result[0][1] = 2.0f * (qxy + qwz);
        result[0][2] = 2.0f * (qxz - qwy);

        result[1][0] = 2.0f * (qxy - qwz);
        result[1][1] = 1.0f - 2.0f * (qxx + qzz);
        result[1][2] = 2.0f * (qyz + qwx);

        result[2][0] = 2.0f * (qxz + qwy);
        result[2][1] = 2.0f * (qyz - qwx);
        result[2][2] = 1.0f - 2.0f * (qxx + qyy);
        return glm::mat4(result);
    }

    inline constexpr glm::mat4 Quaternion::mat4_cast() const noexcept {
        return glm::mat4(mat3_cast());
    }

    /* =================== Operator overloads =================== */
    inline constexpr bool Quaternion::operator==(const Quaternion& q) const noexcept
    {
        return v == q.v && w == q.w;
    }

    inline constexpr Quaternion Quaternion::operator*=(const float k) const noexcept
    {
        return { v * k, w * k };
    }

    inline constexpr Quaternion Quaternion::operator/=(const float k) const noexcept
    {
        const float reciprocal{ 1.f / k };

        return { v * reciprocal, w * reciprocal };
    }

    inline constexpr Quaternion Quaternion::operator+(const Quaternion& q) const noexcept
    {
        return { v + q.v, w + q.w };
    }

    inline constexpr Quaternion Quaternion::operator-(const Quaternion& q) const noexcept
    {
        return { v - q.v, w - q.w };
    }

    inline constexpr Quaternion Quaternion::operator-() const noexcept
    {
        return { -v, -w };
    }

    inline Quaternion Quaternion::operator*(const Quaternion& q) const noexcept
    {
        return { (q.v * w) + (v * q.w) + glm::cross(v, q.v), (w * q.w) - glm::dot(v, q.v) };
    }


    // The quaternion is assumed normalized
    inline constexpr glm::vec3 Quaternion::operator*(const glm::vec3& v) const noexcept
    {
        return rotate(v);
    }


    inline constexpr Quaternion Quaternion::operator*(const float k) const noexcept
    {
        return { v * k, w * k };
    }


    inline constexpr Quaternion Quaternion::operator/(const float k) const noexcept
    {
        const float reciprocal{ 1.f / k };

        return { v * reciprocal, w * reciprocal };
    }

    inline Quaternion::operator glm::vec3() const noexcept {
        return glm::vec3{x, y, z};
    }
}