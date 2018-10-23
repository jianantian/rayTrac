#ifndef _TRACING
#define _TRACING

#include <stdio.h>
#include <tchar.h>

#include "stdafx.h"

#include "vec3.h"
#include "geometry.h"


const int MAX_DEPTH = 4;


float mix(const double& a, const double& b, const double& mix_rate) {

	return a * (1 - mix_rate) + b * mix_rate;
}

Vec3l ray_trace(const Vec3l& ray_source, const Vec3l &ray_direction, const std::vector<Spherel> &sphere_list, int depth) {

	double bias = 1e-6f;
	double distance = 1e8f;
	Vec3l hit_point;
	bool intersection_bool = false;
	Spherel sphere;
	for (Spherel s : sphere_list) {
		Vec3l tmp_intersection_point;
		bool tmp_intersection_bool;
		tmp_intersection_bool = s.intersection(ray_source, ray_direction, tmp_intersection_point);
		float tmp_distance;
		if (tmp_intersection_bool) {

			intersection_bool = true;
			tmp_distance = (tmp_intersection_point - ray_source).square_norm();
			if (tmp_distance < distance) {
				distance = tmp_distance;
				hit_point = tmp_intersection_point;
				sphere = s;
			}
		}
	}

	if (!intersection_bool) { return Vec3l(2., 2., 2.); }

	Vec3l normal_vec = (hit_point - sphere.center).normalize();
	bool is_inside = false;
	if (ray_direction.dot(normal_vec) > 0) {
		// ray_source 在 sphere 内
		normal_vec = -normal_vec;
		is_inside = true;
	}

	Vec3l surface_color(0., 0., 0.);
	float fresnel_coeff;
	if ((sphere.transparency > 0 || sphere.reflection > 0) && depth < MAX_DEPTH) {
		float cos_theta = -(normal_vec.dot(ray_direction));


		// 计算反射光线的方向
		Vec3l reflection_direction = ray_direction - normal_vec * (2.0 * ray_direction.dot(normal_vec));
		reflection_direction = reflection_direction.normalize();

		// 稍微偏移, 不让起点在球面上
		Vec3l reflection = ray_trace(hit_point + normal_vec * bias, reflection_direction, sphere_list, depth + 1);


		Vec3l refraction(0., 0., 0.);
		double eta = 0.;
		if (sphere.transparency > 0) {
			eta = sphere.refractive_rate;

			if (!is_inside) {
				eta = 1.0 / eta;
			}
			double square_cos_alpha = 1 - (1 - cos_theta * cos_theta) * eta * eta;
			if (square_cos_alpha > 0) {
				double cos_alpha;
				Vec3l refraction_direction;
				cos_alpha = sqrt(square_cos_alpha);
				// 计算折射光线方向
				refraction_direction = ray_direction * eta + normal_vec * (eta * cos_theta - cos_alpha);
				refraction_direction = refraction_direction.normalize();
				refraction = ray_trace(hit_point - normal_vec * bias, refraction_direction, sphere_list, depth + 1);
			}
		}
		fresnel_coeff = mix(pow(1.f - cos_theta, 3), 1.f, 0.1f);
		surface_color = (reflection * fresnel_coeff + refraction * (1 - fresnel_coeff) * sphere.transparency) * sphere.surface_color;
	}
	else {
		int num = sphere_list.size();
		double transmission = 1.;
		Vec3l light_direction, hit_pt;
		for (int i = 0; i < num; i++) {
			Spherel sphere_i = sphere_list[i];
			if (sphere_i.emission_color.x > 0) {
				transmission = 1;
				light_direction = sphere_i.center - hit_point;
				light_direction = light_direction.normalize();
				for (int j = 0; j < num; j++) {

					if (i == j) continue;
					Spherel sphere_j = sphere_list[j];
					if (sphere_j.intersection(hit_point + normal_vec * bias, light_direction, hit_pt)) {
						// 光线被阻挡
						transmission = 0;
						break;
					}
				}

				surface_color += (sphere.surface_color
					* transmission
					* fmax(0.f, normal_vec.dot(light_direction))
					* sphere_i.emission_color);
			}

		}
	}
	return surface_color + sphere.emission_color;
}


int per_pixel_modify(double x) {
	return int(fmax(fmin(x, 1), 0) * 255.99);
}

Vec3<int> pixel_modify(Vec3l pixel) {

	return Vec3<int>(per_pixel_modify(pixel.x), per_pixel_modify(pixel.y), per_pixel_modify(pixel.z));
}

void render(std::vector<Spherel> sphere_list, int width, int height, float view_angle, Vec3l source_point) {
	double inv_width = 1.0 / width;
	double inv_height = 1.0 / height;
	double tan_theta = tan((view_angle / 180.) / 2. * M_PI);
	double ratio = (double)width / height;
	double x, y;
	Vec3l r_direction, pixel;
	Vec3<int> rgb_vec;

	std::ofstream pic("res.ppm");
	pic << "P3\n" << width << " " << height << "\n255\n";

	int ns = 10;
	unsigned int num;
	errno_t err;
	double r;
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			Vec3l col;
			for (unsigned int k = 0; k < ns; k++) {
				err = rand_s(&num);
				r = (err != 0) ? (double)num / ((double)UINT_MAX + 1) : 0.5;
				r = r / 10;
				x = (2 * (j + 0.5 + r) * inv_width - 1) * tan_theta * ratio;
				y = (1 - (2 * (i + r + 0.5) * inv_height)) * tan_theta;
				r_direction = Vec3l(x, y, -1.0);
				r_direction = r_direction.normalize();
				pixel = ray_trace(source_point, r_direction, sphere_list, 0);
				col += pixel;
			}
			col = col / ns;
			rgb_vec = pixel_modify(col);
			pic << rgb_vec.x << " " << rgb_vec.y << " " << rgb_vec.z << "\n";
		}
	}
	pic.close();
}

#endif // !_TRACING
