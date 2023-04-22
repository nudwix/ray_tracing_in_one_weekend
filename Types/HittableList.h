#pragma once

#include "HitResult.h"
#include "Ray.h"

#include <memory>
#include <vector>

namespace RTType
{
    class Hittable
    {
    public:
        virtual ~Hittable() = default;
        virtual bool Hit(const Ray& ray, double tMin, double tMax, HitResult& hitRecord) const = 0;
    };

    struct HittableList : Hittable
    {
        std::vector<std::shared_ptr<Hittable>> objects;

        HittableList() = default;

        HittableList(const std::shared_ptr<Hittable>& hittable)
        {
            Add(hittable);
        }

        void Clear()
        {
            objects.clear();
        }

        void Add(const std::shared_ptr<Hittable>& hittable)
        {
            objects.push_back(hittable);
        }

        bool Hit(const Ray& ray, const double tMin, const double tMax, HitResult& hitRecord) const
        {
            HitResult currentRecord;
            bool isAnythingHit = false;
            double closestSoFar = tMax;

            for (const auto& object : objects)
            {
                if (object->Hit(ray, tMin, closestSoFar, currentRecord))
                {
                    isAnythingHit = true;
                    closestSoFar = currentRecord.t;
                    hitRecord = currentRecord;
                }
            }
            return isAnythingHit;
        }
    };
}
