#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Core/Core.h"
#include "utils.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "stb_image_write.h"


void write_color(std::vector<unsigned char>& image_data, const glm::vec3& pixel_color) {
	auto r = pixel_color[0];
	auto g = pixel_color[1];
	auto b = pixel_color[2];

	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	image_data.push_back(static_cast<unsigned char>(rbyte));
	image_data.push_back(static_cast<unsigned char>(gbyte));
	image_data.push_back(static_cast<unsigned char>(bbyte));
};

glm::vec3 unit_vector(const glm::vec3& v) {
	return v / static_cast<float>(v.length());
}

/*
float hit_sphere(const glm::vec3& center, float radius, const ray& r) {
	glm::vec3 oc = center - r.origin();
	auto a = length_squared(r.direction());
	auto h = glm::dot(r.direction(), oc);
	auto c = length_squared(oc) - radius*radius;
	auto discriminant = h * h - a * c;
	if (discriminant < 0) {
		return -1.0f;
	}
	else {
		return (h - std::sqrt(discriminant)) / a;
	}
}
*/

glm::vec3 ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5f * (rec.normal + glm::vec3(1, 1, 1));
	}

	glm::vec3 unit_direction = unit_vector(r.direction());
	float a = 0.5 * (unit_direction[1] + 1.0);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + a * glm::vec3(0.5, 0.7, 1.0);
};

int main()
{

	auto aspect_ratio = 16.0 / 9.0;
	
	int img_width = 400;
// Calculate img_height & ensure that it's at least 1
	int img_height = int(img_width / aspect_ratio);
	img_height = (img_height < 1) ? 1 : img_height;

	int channels = 3;

	std::vector<unsigned char> image_data;
	image_data.reserve(img_width * img_height * channels);

// World

	hittable_list world;
	world.add(make_shared<sphere>(glm::vec3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(glm::vec3(0, -100.5, -1), 100));

// Camera
	
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	double viewport_width = viewport_height * (double(img_width) / img_height);
	glm::vec3 camera_center(0, 0, 0);

// Calculate vectors across horizontal and down the vertical viewport edges
 
	glm::vec3 viewport_u( viewport_width, 0, 0 );
	glm::vec3 viewport_v(0, -viewport_height, 0 );

// Calculate horizontal and vertical delta vectors from pixel to pixel

	glm::vec3 pixel_delta_u(viewport_u / float(img_width));
	glm::vec3 pixel_delta_v(viewport_v / float(img_height));

// Calculate the location of the upper left pixel
	auto viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
	auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);


// Render loop

	for (int j = 0; j < img_height; j++) {
		std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
		for (int i = 0; i < img_width; i++) {
			auto pixel_center = pixel00_loc + (float(i) * pixel_delta_u) + (float(j) * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;

			ray r(camera_center, ray_direction);
			glm::vec3 pixel_color = ray_color(r, world);
			write_color(image_data, pixel_color);
		}
	}

	stbi_write_png("RayTracedImg.png", img_width, img_height, channels, image_data.data(), img_width * channels);

	std::clog << "\r Done. \n";
}