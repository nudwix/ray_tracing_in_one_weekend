#pragma once

#include "Config.h"

// Shared standard headers
#include <limits>
#include <random>

namespace RT
{
    // Constants
    constexpr double infinity = std::numeric_limits<double>::infinity();
    constexpr double pi = 3.14159265358979323846;
    constexpr double nearZero = 1e-8;
    
    // Utility functions
    inline double Clamp(const double val, const double min, const double max)
    {
        if (val < min) return min;
        if (val > max) return max;
        return val;
    }

    inline double DegreesToRadians(double degrees)
    {
        return degrees * pi / 180.0;
    }

    inline double RandomDouble()
    {
        static std::random_device rd;
        static std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
        static std::mt19937 generator{ ss };
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return distribution(generator);
    }

    inline double RandomDouble(const double min, const double max)
    {
        return min + (max - min) * RandomDouble();
    }
}
