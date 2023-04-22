#include "Sphere.h"

namespace RTObject
{
    Sphere::Sphere() = default;

    Sphere::Sphere(RTTPoint3 inCenter, double inRadius, std::shared_ptr<RTTMaterial> inMaterial)
        : center(inCenter), radius(inRadius), material(std::move(inMaterial))
    {
    }

    bool Sphere::Hit(const RTTRay& ray, const double tMin, const double tMax, RTTHitResult& hitResult) const
    {
        /*
         * Equation for a sphere centered at C(Cx, Cy, Cz) is (X - Cx)^2 + (Y - Cy)^2 + (Z - Cz)^2 = r^2.
         * Thus, for a given point P(Px, Py, Pz),
         *	if (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 = r^2, the point is on the sphere,
         *	if (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 < r^2, the point is inside the sphere, and
         *	if (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 > r^2, the point is outside the sphere.
         *
         * We can transform this equation into the vector form: (P - C)^2 = r^2.
         * We can also define ray as a function P(t) = A + B * t, where:
         *	P - ray function,
         *	A - ray origin point P(x, y, z),
         *	B - ray direction D(x, y, z),
         *	t - ray length
         *
         * So, to check if the ray hit the sphere,
         *	we can transform the equation to:
         *		(P(t) - C)^2 = r^2,
         *
         *	then expand R(l):
         *		(A + B * t - C)^2 = r^2,
         *
         *	after that put r^2 on the left side and open parenthesis where:
         *		a = B * t,
         *		b = A - C, and
         *		(a + b)^2 = a^2 + 2 * a * b + b^2:
         *
         *	B^2 * t^2 + 2 * B * (A - C) * t + (A - C)^2 - r^2 = 0.
         *
         * The only unknown value in this equation is t, so we can transform equation into quadratic
         * ax^2 + bx + c = 0, and solve it for t, where:
         *	a = B^2,
         *	b = 2 * B * (A - C),
         *	c = (A - C)^2, and
         *	D = b^2 - 4 * a * c
         *
         * If D > 0, the ray hits sphere at two points,
         * if D = 0, the ray hits sphere at one point, and
         * if D < 0, the ray does not hit sphere at all.
         */

        const RTTVector3 oc(ray.Origin() - center);
        const double a = ray.Direction().LengthSquared();
        const double b = 2.0 * Dot(oc, ray.Direction());
        const double c = oc.LengthSquared() - radius * radius;
        const double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;

        const double left = -b / (2.0 * a);
        const double right = std::sqrt(discriminant) / (2.0 * a);

        // Find the nearest root that lies between t min and t max
        double root = left - right;
        if (root < tMin || tMax < root)
        {
            root = left + right;
            if (root < tMin || tMax < root)
            {
                return false;
            }
        }

        hitResult.t = root;
        hitResult.point = ray.At(hitResult.t);
        hitResult.SetFaceNormal(ray, (hitResult.point - center) / radius);
        hitResult.material = material;

        return true;
    }
}
