#include <stdio.h>
#include <tchar.h>

#include "stdafx.h"

#include "vec3.h"
#include "geometry.h"
#include "tracing.h"


int main() {
	Spherel s0(Vec3l(0.0, -10004., -20.), 10000., 1.1f, Vec3l(0.20, 0.20, 0.20), Vec3l(0.), 0., 0.0);
	Spherel s1(Vec3l(0.0, 0., -20.), 4., 1.1, Vec3l(1.0, 0.32, 0.36), Vec3l(0.), 1., 0.5);
	Spherel s2(Vec3l(5.0, -1., -15.), 2., 1.1, Vec3l(0.90, 0.76, 0.46), Vec3l(0.), 1., 0.0);
	Spherel s3(Vec3l(5.0, 0., -25.), 3., 1.1, Vec3l(0.65, 0.77, 0.97), Vec3l(0.), 1., 0.0);
	Spherel s4(Vec3l(-5.5, 0., -15.), 3., 1.1, Vec3l(0.90, 0.90, 0.90), Vec3l(0.), 1., 0.0);
	Spherel s_array[5] = { s0, s1, s2, s3, s4 };
	std::vector<Spherel> sphere_list(s_array, s_array + 5);
	//std::cout << "s3 center: " << s3.center << "s3 radius: " << s3.radius << std::endl;
	//std::cout << sphere_list.size() << std::endl;
	//std::cout << 3.0f * s3.center << std::endl;
	//std::cin.get();

	int width = 1024;
	int height = 720;
	double view_angle = 30;
	Vec3l source_point(0., 0., 0.);
	DWORD start_time, end_time;
	start_time = GetTickCount64();
	render(sphere_list, width, height, view_angle, source_point);
	end_time = GetTickCount64();
	std::cout << "耗时: " << end_time - start_time << std::endl;
	return 0;
}

