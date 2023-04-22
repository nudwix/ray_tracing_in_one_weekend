#include "Common/Common.h"
#include "Types/RTTypes.h"
#include "Objects/RTObjects.h"

#include <vector>
#include <thread>

RTTColor RayColor(const RTTRay& ray, const RTTHittable& world, int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered
	if (depth <= 0) {
		return RTType::colorBlack;
	}

	RTTHitResult hitRecord;
	if (world.Hit(ray, 0.001, RT::infinity, hitRecord)) {
		RTTRay scattered;
		RTTColor attenuation;
		if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered)) {
			return attenuation * RayColor(scattered, world, --depth);
		}
		return RTType::colorBlack;
	}
	
	// Sky

	// Scale ray direction to unit length (-1.0, 1.0)
	const RTTVector3 unitDirection = UnitVector(ray.Direction());
	
	// Scale y axis to [0, 1];
	const double t = 0.5 * (unitDirection.y + 1.0);

	// When scaled height is 1 return sky blue, 
	// when scaled height is 0 return white,
	// else return blend of blue and white
	return t * RTType::colorSkyBlue + (1.0 - t) * RTType::colorWhite;
}

RTTHittableList RandomScene() {
	RTTHittableList scene;

	// Ground 
	auto materialGround = std::make_shared<RTType::Lambertian>(RTTColor(0.5, 0.5, 0.5));
	scene.Add(std::make_shared<RTOSphere>(RTTPoint3(0.0, -1000.0, 0.0), 1000.0, materialGround));

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			const double randomMaterial = RT::RandomDouble();
			RTTPoint3 center(a + 0.9 * RT::RandomDouble(), 0.2, b + 0.9 * RT::RandomDouble());

			if ((center - RTTPoint3(4.0, 0.2, 0.0)).Length() > 0.9) {
				std::shared_ptr<RTTMaterial> material;

				if (randomMaterial < 0.75) {
					// Diffuse material
					RTTColor albedo = RTTColor::Random() * RTTColor::Random();
					material = std::make_shared<RTType::Lambertian>(albedo);
					scene.Add(std::make_shared<RTOSphere>(center, 0.2, material));
				} else if (randomMaterial < 0.95) {
					// Metallic material
					RTTColor albedo = RTTColor::Random(0.5, 1.0);
					double fuzziness = RT::RandomDouble(0.1, 0.9);
					material = std::make_shared<RTType::Metal>(albedo, fuzziness);
					scene.Add(std::make_shared<RTOSphere>(center, 0.2, material));
				} else {
					material = std::make_shared<RTType::Dielectric>(1.5);
					scene.Add(std::make_shared<RTOSphere>(center, 0.2, material));
				}
			}
		}
	}
	
	auto materialMatte = std::make_shared<RTType::Lambertian>(RTTColor(1.0, 0.75, 0.8));
	scene.Add(std::make_shared<RTOSphere>(RTTPoint3(0.0, 1.0, 0.0), 1.0, materialMatte));
	
	auto materialMetal = std::make_shared<RTType::Metal>(RTTColor(1.0, 0.85, 0.0), 0.3);
	scene.Add(std::make_shared<RTOSphere>(RTTPoint3(-4.0, 1.0, 0.0), 1.0, materialMetal));
	
	auto materialDielectric = std::make_shared<RTType::Dielectric>(1.5);
	scene.Add(std::make_shared<RTOSphere>(RTTPoint3(4.0, 1.0, 0.0), 1.0, materialDielectric));
	
	return scene;
}

void RenderLines(std::vector<int>& inImage, const RTOCamera inCamera, const RTTHittableList& inWorld, const int inWorkerId, const int inStep) {
	for (int j = RT::imageHeight - 1 - inWorkerId; j >= 0; j -= inStep) {
		for (int i = 0; i < RT::imageWidth; ++i) {
			RTTColor pixelColor(0.0, 0.0, 0.0);

			for (int sample = 0; sample < RT::samplesPerPixel; ++sample) {
				const double col = (static_cast<double>(i) + RT::RandomDouble()) / (RT::imageWidth - 1);
				const double row = (static_cast<double>(j) + RT::RandomDouble()) / (RT::imageHeight - 1);
				const RTTRay ray(inCamera.GetRay(col, row));
				pixelColor += RayColor(ray, inWorld, RT::maxDepth);
			}

			RTType::WriteColor(inImage, 3 * ((RT::imageHeight - 1 - j) * RT::imageWidth + i), pixelColor, RT::samplesPerPixel);
		}
	}
}

int main() {
	// World
	const RTTHittableList world = RandomScene();
	
	// Camera
	const RTTPoint3 lookFrom(13.0, 2.0, 3.0);
	const RTTPoint3 lookAt(0.0, 0.0, 0.0);
	const RTTVector3 vectorUp(0.0, 1.0, 0.0);
	
	const RTOCamera camera(lookFrom, lookAt, vectorUp, 20.0, RT::aspectRatio, 0.1, 10);

	// Multithreading
	constexpr int threadCount = 90;
	std::vector<std::thread> workers;

	// Render
	std::cout << "P3\n" << RT::imageWidth << ' ' << RT::imageHeight << "\n255\n";
	std::vector<int> image(RT::imageWidth * RT::imageHeight * 3);

	std::cerr << "Tracing image with " << threadCount << " threads on CPU.\n";
	for (int workerId = 0; workerId < threadCount; ++workerId) {
		workers.emplace_back(RenderLines, std::ref(image), std::ref(camera), std::ref(world), workerId, threadCount);
	}
	
	for (std::thread& worker : workers) {
		worker.join();
	}
	
	// Output
	std::cerr << "Writing image\n";
	for (int i = 0; i < image.size(); i += 3) {
		std::cout << image[i] << ' ' << image[i + 1] << ' ' << image[i + 2] << '\n';
	}
	std::cerr << "Done.\n";
}
