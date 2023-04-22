#include "Vector3.h"

#include "Common/Common.h"

namespace RTType
{
    Vector3::Vector3()
        : x(0), y(0), z(0)
    {
    }

    Vector3::Vector3(const double inX, const double inY, const double inZ)
        : x(inX), y(inY), z(inZ)
    {
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& Vector3::operator*=(const double multiplier)
    {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        return *this;
    }

    Vector3& Vector3::operator*=(const Vector3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3& Vector3::operator/=(const double divisor)
    {
        return *this *= 1 / divisor;
    }

    double Vector3::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    double Vector3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    bool Vector3::NearZero() const
    {
        return (fabs(x) < RT::nearZero && std::fabs(y) < RT::nearZero && std::fabs(z) < RT::nearZero);
    }

    Vector3 Vector3::Random()
    {
        return Vector3(RT::RandomDouble(), RT::RandomDouble(), RT::RandomDouble());
    }

    Vector3 Vector3::Random(const double min, const double max)
    {
        return Vector3(RT::RandomDouble(min, max), RT::RandomDouble(min, max), RT::RandomDouble(min, max));
    }


    // Utility functions
    std::ostream& operator<<(std::ostream& out, const Vector3& vector)
    {
        return out << vector.x << ' ' << vector.y << ' ' << vector.z;
    }

    Vector3 operator+(const Vector3& vectorA, const Vector3& vectorB)
    {
        return Vector3(vectorA.x + vectorB.x, vectorA.y + vectorB.y, vectorA.z + vectorB.z);
    }

    Vector3 operator-(const Vector3& vectorA, const Vector3& vectorB)
    {
        return Vector3(vectorA.x - vectorB.x, vectorA.y - vectorB.y, vectorA.z - vectorB.z);
    }

    Vector3 operator*(const Vector3& vector, const double multiplier)
    {
        return Vector3(vector.x * multiplier, vector.y * multiplier, vector.z * multiplier);
    }

    Vector3 operator*(const double multiplier, const Vector3& vector)
    {
        return vector * multiplier;
    }

    Vector3 operator*(const Vector3& vectorA, const Vector3& vectorB)
    {
        return Vector3(vectorA.x * vectorB.x, vectorA.y * vectorB.y, vectorA.z * vectorB.z);
    }

    Vector3 operator/(const Vector3& vector, const double divisor)
    {
        return vector * (1 / divisor);
    }

    Vector3 UnitVector(const Vector3& vector)
    {
        return vector / vector.Length();
    }

    Vector3 Cross(const Vector3& vectorA, const Vector3& vectorB)
    {
        return Vector3(
            vectorA.y * vectorB.z - vectorA.z * vectorB.y,
            vectorA.z * vectorB.x - vectorA.x * vectorB.z,
            vectorA.x * vectorB.y - vectorA.y * vectorB.x
        );
    }

    double Dot(const Vector3& vectorA, const Vector3& vectorB)
    {
        return vectorA.x * vectorB.x
        + vectorA.y * vectorB.y
        + vectorA.z * vectorB.z;
    }

    Vector3 RandomInUnitSphere()
    {
        while (true)
        {
            Vector3 vector(Vector3::Random());
            if (vector.LengthSquared() >= 1)
            {
                continue;
            }
            return vector;
        }
    }

    Vector3 RandomUnitVector()
    {
        return UnitVector(RandomInUnitSphere());
    }

    Vector3 RandomUnitDisk()
    {
        while (true)
        {
            Vector3 p(RT::RandomDouble(-1.0, 1.0), RT::RandomDouble(-1.0, 1.0), 0.0);
            if (p.LengthSquared() >= 1)
            {
                continue;
            }
            return p;
        }
    }

    Vector3 RandomInHemisphere(const Vector3& normal)
    {
        const Vector3 inUnitSphere = RandomInUnitSphere();
        const bool isInSameHemisphere = Dot(inUnitSphere, normal) > 0.0;
        return isInSameHemisphere ? inUnitSphere : -inUnitSphere;
    }

    Vector3 Reflect(const Vector3& vector, const Vector3& normal)
    {
        return vector - 2 * Dot(vector, normal) * normal;
    }

    Vector3 Refract(const Vector3& vector, const Vector3& normal, const double etaiOverEtat)
    {
        const double cosineTheta = std::fmin(Dot(-vector, normal), 1.0);
        const Vector3 rOutPerpeindicular = etaiOverEtat * (vector + cosineTheta * normal);
        const Vector3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerpeindicular.LengthSquared())) * normal;
        return rOutPerpeindicular + rOutParallel;
    }

    void WriteColor(std::vector<int>& image, const int pixelIndex, const Color pixelColor, const int inSamplesPerPixel)
    {
        // Divide each color by number of samples and gamma-correct for gamma = 2.0
        const double r = std::sqrt(pixelColor.x / inSamplesPerPixel);
        const double g = std::sqrt(pixelColor.y / inSamplesPerPixel);
        const double b = std::sqrt(pixelColor.z / inSamplesPerPixel);

        // Write the translated [0, 255] value of each color component
        image[pixelIndex] = static_cast<int>(256 * RT::Clamp(r, 0.0, 0.999));
        image[pixelIndex + 1] = static_cast<int>(256 * RT::Clamp(g, 0.0, 0.999));
        image[pixelIndex + 2] = static_cast<int>(256 * RT::Clamp(b, 0.0, 0.999));
    }
}
