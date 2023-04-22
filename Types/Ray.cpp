#include "Ray.h"

namespace RTType
{
    Ray::Ray() = default;

    Ray::Ray(const Point3& inOrigin, const Vector3& inDirection)
        : origin(inOrigin), direction(inDirection)
    {
    }

    Point3 Ray::Origin() const
    {
        return origin;
    }

    Vector3 Ray::Direction() const
    {
        return direction;
    }

    Point3 Ray::At(const double length) const
    {
        return origin + direction * length;
    }
}
