#pragma once

#include "Vector3.h"
#include "Ray.h"

namespace RTType
{
    class Material;

    struct HitResult
    {
        Point3 point;
        Vector3 normal;
        std::shared_ptr<Material> material;
        double t{};
        bool frontFace{};

        void SetFaceNormal(const Ray& ray, const Vector3& outNormal)
        {
            frontFace = Dot(ray.Direction(), outNormal) < 0;
            normal = frontFace ? outNormal : -outNormal;
        }
    };
}
