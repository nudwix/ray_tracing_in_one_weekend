#pragma once

#include "Common/Common.h"
#include "Types/RTTypes.h"

namespace RTObject
{
    class Camera
    {
    private:
        RTTPoint3 origin;
        RTTVector3 w, u, v;
        double lensRadius;
        RTTVector3 horizontal, vertical;
        RTTPoint3 lowerLeftCorner;

    public:
        Camera(RTTPoint3 lookFrom, RTTPoint3 lookAt, RTTVector3 vectorUp, double inVerticalFieldOfViewDegrees, double inAspectRatio, double aperture, double focusDistance)
            : origin(lookFrom)
              , w(UnitVector(lookFrom - lookAt))
              , u(UnitVector(Cross(vectorUp, w)))
              , v(Cross(w, u))
              , lensRadius(aperture / 2)
        {
            const double theta = RT::DegreesToRadians(inVerticalFieldOfViewDegrees);
            const double h = tan(theta / 2);
            const double viewportHeight = 2.0 * h;
            const double viewportWidth = inAspectRatio * viewportHeight;

            horizontal = focusDistance * viewportWidth * u;
            vertical = focusDistance * viewportHeight * v;
            lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDistance * w;
        }

        [[nodiscard]] RTTRay GetRay(const double col, const double row) const
        {
            const RTTVector3 rd = lensRadius * RTType::RandomUnitDisk();
            const RTTVector3 offset = u * rd.x + v * rd.y;
            return RTTRay(origin + offset, lowerLeftCorner + col * horizontal + row * vertical - origin - offset);
        }
    };
}
