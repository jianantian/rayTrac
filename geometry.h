#ifndef _GEOMETRY
#define _GEOMETRY

#include "vec3.h"
#include "stdafx.h"


template<typename T>
Vec3<T> get_normal_vector(Vec3<T> pt_0, Vec3<T> pt_1, Vec3<T> pt_2) {
	Vec3<T> n_vec = (this->pt_0 - this->pt_1).cross(this->pt_2 - this->pt_1);
	return n_vec.normalize();
}


template <typename T>
class Ray {
public:
	Vec3<T> source, direction;
	Ray() {};
	Ray(Vec3<T> s, Vec3<T> d): source(s), direction(d) {};
};


typedef Ray<float> Rayf;
typedef Ray<double> Rayl;


template<typename T>
Vec3<T> projection(Vec3<T> point, Ray<T> r) {
	Vec3<T> source = r.source;
	Vec3<T> direction = r.direction;
	return source + direction.dot(point - source) * direction;
}


template <typename T>
class Sphere {
public:
	T radius, refractive_rate, reflection, transparency;
	Vec3<T> center, surface_color, emission_color;
	Sphere() {};
	Sphere(Vec3<T> c, T r, T refr_rate, Vec3<T> s_c, Vec3<T> e_c, T refl_rate, T trans_rate) :
		center(c), radius(r), refractive_rate(refr_rate), surface_color(s_c), emission_color(e_c), reflection(refl_rate), transparency(trans_rate)
	{};

	const bool intersection(const Vec3<T>& source_point, const Vec3<T>& ray_direction, Vec3<T>& intersection_point) {

		Vec3<T> ray = center - source_point;

		T projection = ray.dot(ray_direction);

		if (projection < 0) {
			// 方向不对
			return false;
		}

		else {
			Vec3<T> perpendicular_vec = ray - ray_direction * projection;
			T length;
			length = radius * radius - perpendicular_vec.square_norm();
			if (length < 0) {
				// 未相交
				return false;
			}
			else {
				length = sqrt(length);
				T scale_0, scale_1;
				scale_0 = projection - length;
				scale_1 = projection + length;
				if (scale_0 < 0) {
					intersection_point = ray_direction * scale_1 + source_point;
				}
				else {
					intersection_point = ray_direction * scale_0 + source_point;
				}
				return true;
			}op
		}
	}
};

typedef Sphere<float> Spheref;
typedef Sphere<double> Spherel;

template <typename T>
class Plane {
private:
	Vec3<T> _pt_0, _pt_1, _pt_2;
public:
	Vec3<T> normal_vector
	Plane() {};
	Plane(Vec3<T> p_0, Vec3<T> p_1, Vec3<T> p_2) : _pt_0(p_0), _pt_1(p_1), _pt_2(p_2), normal_vector(get_normal_vector(p_0, p_1, p_2){};
	Plane(Vec3<T> p_0, Vec3<T> p_1, Vec3<T> p_2, Vec3<T> sign_vec): _pt_0(p_0), _pt_1(p_1), _pt_2(p_2) {
		normal_vector = get_normal_vector(p_0, p_1, p_2);
		if (normal_vector.dot(sign_vec) < 0) {
			normal_vector = -normal_vector;
		}
	};
	Plane(Vec3<T> pt, Vec3<T> n_vec) : normal_vector(n_vec), {};
};

template<typename T>
class Tetrahedron {
public:
	Vec3<T> v_0, v_1, v_2, v_3;
	
	Tetrahedron() {};
	Tetrahedron(Vec3<T> pt_0, Vec3<T> pt_1, Vec3<T> pt_2, Vec3<T> pt_3) : v_0(pt_0), v_1(pt_1), v_2(pt_2), v_3(pt_3) {};

	inline std::vector<Vec3<T>> vertices() {
		Vec3<T> *pts = [v_0, v_1, v_2, v_3];
		std::vector<Vec3<T>> verts(pts);
		return verts;
	}

	inline void opposite_surface(unsigned int index);
};

#endif // !_GEOMETRY