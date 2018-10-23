#include "stdafx.h"
#include "vec3.h"
#include "geometry.h"


template<typename T>
Vec3<T> get_normal_vector(const Vec3<T> &pt_0, const Vec3<T> &pt_1, const Vec3<T> &pt_2) {
	Vec3<T> n_vec = (this->pt_0 - this->pt_1).cross(this->pt_2 - this->pt_1);
	return n_vec.normalize();
}

template<typename T>
Vec3<T> projection(Vec3<T> point, Ray<T> r) {
	Vec3<T> source = r.source;
	Vec3<T> direction = r.direction;
	return source + direction.dot(point - source) * direction;
}


