#pragma once

#include <iostream>
#include <vector>

namespace RTType
{
    struct Vector3
    {
    public:
        double x, y, z;

    public:
        Vector3();
        explicit Vector3(double inX, double inY, double inZ);

        Vector3 operator-() const;
        Vector3& operator+=(const Vector3& other);
        Vector3& operator*=(double multiplier);
        Vector3& operator*=(const Vector3& other);
        Vector3& operator/=(double divisor);

        [[nodiscard]] double Length() const;
        [[nodiscard]] double LengthSquared() const;

        [[nodiscard]] bool NearZero() const;

        static Vector3 Random();
        static Vector3 Random(double min, double max);
    };

    // Type aliases for Vector3
    using Point3 = Vector3;   // 3D point
    using Color = Vector3;    // RGB color

    // Colors
    const Color colorWhite(1.0, 1.0, 1.0);
    const Color colorSkyBlue(0.5, 0.7, 1.0);
    const Color colorBlack(0.0, 0.0, 0.0);

    // Vector3 utility functions
    std::ostream& operator<<(std::ostream& out, const Vector3& vector);

    Vector3 operator+(const Vector3& vectorA, const Vector3& vectorB);
    Vector3 operator-(const Vector3& vectorA, const Vector3& vectorB);
    Vector3 operator*(const Vector3& vector, const double multiplier);
    Vector3 operator*(const double multiplier, const Vector3& vector);
    Vector3 operator*(const Vector3& vectorA, const Vector3& vectorB);
    Vector3 operator/(const Vector3& vector, const double divisor);

    double Dot(const Vector3& vectorA, const Vector3& vectorB);
    Vector3 Cross(const Vector3& vectorA, const Vector3& vectorB);
    Vector3 UnitVector(const Vector3& vector);

    Vector3 RandomInUnitSphere();
    Vector3 RandomUnitVector();
    Vector3 RandomUnitDisk();
    Vector3 RandomInHemisphere(const Vector3& normal);

    Vector3 Reflect(const Vector3& vector, const Vector3& normal);
    Vector3 Refract(const Vector3& vector, const Vector3& normal, double etaiOverEtat);

    void WriteColor(std::vector<int>& image, const int pixelIndex, const Color pixelColor, const int inSamplesPerPixel);
}
