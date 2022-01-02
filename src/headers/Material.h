#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Common.h"
#include "Hittable.h"

class Material {
public: 
	virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) :
		m_albedo(albedo)
	{
	}

	virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vector3 scatter_direction = record.normal + rand_unit_vector();

		//Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = record.normal;

		scattered = Ray(record.point, scatter_direction);
		attenuation = m_albedo;
		return true;
	}

public:
	Color m_albedo;
};

class Metal : public Material {
public:
	Metal(const Color& albedo, float f) :
		m_albedo(albedo), fuzz(f < 1 ? f : 1)
	{
	}

	virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = reflect(unit_vector(ray_in.direction()), record.normal);
		scattered = Ray(record.point, reflected + fuzz * rand_in_unit_sphere());
		attenuation = m_albedo;
		return dot(scattered.direction(), record.normal) > 0;
	}

public:
	Color m_albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float index_of_refraction) :
		m_index_of_refraction(index_of_refraction)
	{
	}

	virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.f, 1.f, 1.f);
		float refraction_ratio = record.front_face ? (1.f / m_index_of_refraction) : m_index_of_refraction;

		Vector3 unit_dir = unit_vector(ray_in.direction());
		float cos_theta = fmin(dot(-unit_dir, record.normal), 1.f);
		float sin_theta = sqrt(1.f - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.f;
		Vector3 dir;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
			dir = reflect(unit_dir, record.normal);
		else
			dir = refract(unit_dir, record.normal, refraction_ratio);

		scattered = Ray(record.point, dir);
		return true;
	}

public:
	float m_index_of_refraction;

private:
	static float reflectance(float cos, float refraction_ratio)
	{
		//Use Schlick's approximation for reflectance
		float r0 = (1.f - refraction_ratio) / (1.f + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1.f - r0) * pow(1 - cos, 5);
	}
};

#endif // !MATERIAL_H
