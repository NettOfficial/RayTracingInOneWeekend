#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable {
public:
	Sphere() {};
	Sphere(Point3D center, float radius, shared_ptr<Material> material_ptr) :
		m_center(center), m_radius(radius), m_material_ptr(material_ptr)
	{
	}

	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const override;

public:
	Point3D m_center;
	float m_radius;
	shared_ptr<Material> m_material_ptr;
};

bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const
{
	Vector3 distance = ray.origin() - m_center;
	//Equasion a*t^2 + b*t + c = r^2. Solving with t
	float a = ray.direction().length_squared();
	float half_b = dot(ray.direction(), distance);
	float c = distance.length_squared() - m_radius * m_radius;

	float D = half_b * half_b - a * c;
	if (D < 0)
		return false;
	float sqrtD = sqrt(D);

	float root = (-half_b - sqrtD) / a;
	if (root<t_min || root > t_max)
	{
		root = (-half_b + sqrtD) / a;
		if (root<t_min || root > t_max)
			return false;
	}
	
	record.t = root;
	record.point = ray.at(record.t);
	Vector3 outward_normal = (record.point - m_center) / m_radius;
	record.set_face_normal(ray, outward_normal);
	record.material_ptr = m_material_ptr;

	return true;
}

#endif // !SPHERE_H
