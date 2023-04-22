#pragma once

#include "Common/Common.h"

#include "HitResult.h"
#include "Ray.h"
#include "Vector3.h"

namespace RTType
{
    class Material
    {
    public:
        virtual bool Scatter(const Ray& inRay, const HitResult& hitResult, Color& attenuation, Ray& scattered) const = 0;
    };

    class Lambertian : public Material
    {
    public:
        Color albedo;

    public:
        Lambertian(const Color& inAlbedo)
            : albedo(inAlbedo)
        {
        }

        bool Scatter(const Ray& inRay, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override
        {
            Vector3 scatterDirection = hitResult.normal + RandomUnitVector();
            if (scatterDirection.NearZero())
            {
                scatterDirection = hitResult.normal;
            }
            scattered = Ray(hitResult.point, scatterDirection);
            attenuation = albedo;
            return true;
        }
    };

    class Metal : public Material
    {
    public:
        Color albedo;
        double fuzziness;

    public:
        Metal(const Color& inAlbedo, double inFuzziness)
            : albedo(inAlbedo), fuzziness(inFuzziness < 1 ? inFuzziness : 1)
        {
        }

        bool Scatter(const Ray& inRay, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override
        {
            const Vector3 reflected = Reflect(UnitVector(inRay.Direction()), hitResult.normal);
            scattered = Ray(hitResult.point, reflected + fuzziness * RandomInUnitSphere());
            attenuation = albedo;
            return Dot(scattered.Direction(), hitResult.normal) > 0;
        }
    };

    class Dielectric : public Material
    {
    public:
        double refraction;

    public:
        Dielectric(double inRefraction)
            : refraction(inRefraction)
        {
        }

        bool Scatter(const Ray& inRay, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override
        {
            const double refractionRatio = hitResult.frontFace ? (1.0 / refraction) : refraction;
            const Vector3 unitDirection = UnitVector(inRay.Direction());
            const double cosineTheta = fmin(Dot(-unitDirection, hitResult.normal), 1.0);
            const double sineTheta = sqrt(1.0 - cosineTheta * cosineTheta);

            const bool canRetract = refractionRatio * sineTheta <= 1.0;
            Vector3 direction;

            if (!canRetract || Reflectance(cosineTheta, refractionRatio) > RT::RandomDouble())
            {
                direction = Reflect(unitDirection, hitResult.normal);
            }
            else
            {
                direction = Refract(unitDirection, hitResult.normal, refractionRatio);
            }

            scattered = Ray(hitResult.point, direction);
            attenuation = colorWhite;
            return true;
        }

    private:
        static double Reflectance(double cosine, double refractionRatio)
        {
            double r0 = (1 - refractionRatio) / (1 + refractionRatio);
            r0 *= r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
    };
}
