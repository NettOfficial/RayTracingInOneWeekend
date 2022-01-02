#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

class Camera {
public:
	Camera(Point3D lookfrom, Point3D lookat, Vector3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist)
	{
		float theta = degrees_to_radians(vfov);
		float h = tan(theta / 2);
		float viewport_height = 2.0f * h;
		float viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - focus_dist * w;

		lens_radius = aperture / 2.f;
	}

	Ray get_ray(float s, float t) const
	{
		Vector3 rand_d = lens_radius * rand_in_unit_disk();
		Vector3 offset = u * rand_d.x() + v * rand_d.y();
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}

public:
	Point3D origin;
	Point3D lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 w, u, v;
	float lens_radius;
};

#endif // !CAMERA_H