#pragma once

namespace RT
{
    // Threads
    constexpr int threadCount = 90;
    
    // Image
    constexpr double aspectRatio = 16.0 / 9.0;
    constexpr int imageWidth = 1920;
    constexpr int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    constexpr int samplesPerPixel = 32;
    constexpr int maxDepth = 12;
}