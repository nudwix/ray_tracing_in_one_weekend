#pragma once


#include "Vector3.h"

namespace RTType
{
    struct Ray
    {
    public:
        Point3 origin;
        Vector3 direction;

    public:
        Ray();
        explicit Ray(const Point3& inOrigin, const Vector3& inDirection);

        [[nodiscard]] Point3 Origin() const;
        [[nodiscard]] Vector3 Direction() const;
        [[nodiscard]] Point3 At(double length) const;
    };
}
