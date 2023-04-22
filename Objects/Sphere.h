#pragma once

#include "Types/RTTypes.h"

namespace RTObject
{
    class Sphere : public RTTHittable
    {
    public:
        RTTPoint3 center;
        double radius{};
        std::shared_ptr<RTTMaterial> material;

    public:
        Sphere();
        Sphere(RTTPoint3 inCenter, double inRadius, std::shared_ptr<RTTMaterial> inMaterial);

        bool Hit(const RTTRay& ray, double tMin, double tMax, RTTHitResult& hitResult) const override;
    };
}
