#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "Common.h"

class Material;

struct HitRecord {
	Point3D point;
	Vector3 normal;
	shared_ptr<Material> material_ptr;
	float t;
	bool front_face;

	inline void set_face_normal(const Ray& ray, const Vector3& outward_normal)
	{
		front_face = dot(ray.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable  {
public:
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const = 0;
};

#endif // !HITTABLE_H
