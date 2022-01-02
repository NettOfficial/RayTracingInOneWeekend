#pragma once

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

inline float random_float();
inline float random_float(float min, float max);

class Vector3 {
public:
	Vector3() :
		el{ 0,0,0 }
	{
	}

	Vector3(float el0, float el1, float el2) :
		el{ el0, el1, el2 }
	{
	}

	inline float x() const { return el[0]; }
	inline float y() const { return el[1]; }
	inline float z() const { return el[2]; }
	inline float r() const { return el[0]; }
	inline float g() const { return el[1]; }
	inline float b() const { return el[2]; }

	inline Vector3 operator-() const { return Vector3(-el[0], -el[1], -el[2]); }
	inline float operator [](int i) const { return el[i]; }
	inline float& operator [](int i) { return el[i]; }

	//Method declarations

	inline Vector3& operator+=(const Vector3& vec);
	inline Vector3& operator*=(const Vector3& vec);
	inline Vector3& operator*=(const float t);
	inline Vector3& operator/=(const Vector3& vec);
	inline Vector3& operator/=(const float t);

	inline float length() const { return sqrt(length_squared()); }
	inline float length_squared() const { return el[0] * el[0] + el[1] * el[1] + el[2] * el[2]; }

	inline static Vector3 random() { return Vector3(random_float(), random_float(), random_float()); }
	inline static Vector3 random(float min, float max) { return Vector3(random_float(min, max), random_float(min, max), random_float(min, max)); }

	inline bool near_zero() const
	{
		//Return true if the vector is close to zero in all dimensions.
		const float s = 1e-8;
		return (fabs(el[0]) < s) && (fabs(el[1]) < s) && (fabs(el[2]) < s);
	}

public:
	float el[3];
};

//Methods

inline std::ostream& operator<<(std::ostream& out, const Vector3& vec)
{
	return out << vec.el[0] << ' ' << vec.el[1] << ' ' << vec.el[2];
}

inline std::istream& operator>>(std::istream& in, Vector3& vec) 
{
	in >> vec.el[0] >> vec.el[1] >> vec.el[2];
	return in;
}

inline Vector3& Vector3::operator+=(const Vector3& vec)
{
	el[0] += vec.el[0];
	el[1] += vec.el[1];
	el[2] += vec.el[2];
	return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& vec)
{
	el[0] *= vec.el[0];
	el[1] *= vec.el[1];
	el[2] *= vec.el[2];
	return *this;
}

inline Vector3& Vector3::operator*=(const float t)
{
	el[0] *= t;
	el[1] *= t;
	el[2] *= t;
	return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& vec)
{
	el[0] /= vec.el[0];
	el[1] /= vec.el[1];
	el[2] /= vec.el[2];
	return *this;
}

inline Vector3& Vector3::operator/=(const float t)
{ 
	return *this *= 1 / t; 
}

//Utility functions

inline Vector3 operator+(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.el[0] + vec2.el[0], vec1.el[1] + vec2.el[1], vec1.el[2] + vec2.el[2]);
}

inline Vector3 operator+(const Vector3& vec, const float t)
{
	return Vector3(vec.el[0] + t, vec.el[1] + t, vec.el[2] + t);
}

inline Vector3 operator+(const float t, const Vector3& vec)
{
	return vec + t;
}

inline Vector3 operator-(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.el[0] - vec2.el[0], vec1.el[1] - vec2.el[1], vec1.el[2] - vec2.el[2]);
}

inline Vector3 operator*(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.el[0] * vec2.el[0], vec1.el[1] * vec2.el[1], vec1.el[2] * vec2.el[2]);
}

inline Vector3 operator/(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.el[0] / vec2.el[0], vec1.el[1] / vec2.el[1], vec1.el[2] / vec2.el[2]);
}

inline Vector3 operator*(const Vector3& vec, const float t)
{
	return Vector3(vec.el[0] * t, vec.el[1] * t, vec.el[2] * t);
}

inline Vector3 operator*(const float t, const Vector3& vec)
{
	return vec * t;
}

inline Vector3 operator/(const Vector3& vec, const float t)
{
	return Vector3(vec.el[0] / t, vec.el[1] / t, vec.el[2] / t);
}

inline float dot(const Vector3& vec1, const Vector3& vec2)
{
	return vec1.el[0] * vec2.el[0] + vec1.el[1] * vec2.el[1] + vec1.el[2] * vec2.el[2];
}

inline Vector3 cross(const Vector3& vec1, const Vector3& vec2) 
{
	return Vector3(vec1.el[1] * vec2.el[2] - vec1.el[2] * vec2.el[1],
		vec1.el[2] * vec2.el[0] - vec1.el[0] * vec2.el[2],
		vec1.el[0] * vec2.el[1] - vec1.el[1] * vec2.el[0]);
}

inline Vector3 unit_vector(Vector3 vec)
{
	return vec / vec.length();
}

inline Vector3 rand_in_unit_sphere()
{
	while (true)
	{
		Vector3 vec = Vector3::random(-1, 1);
		if (vec.length_squared() >= 1) continue;
		return vec;
	}
}

inline Vector3 rand_unit_vector()
{
	return unit_vector(rand_in_unit_sphere());
}

inline Vector3 rand_in_hemisphere(const Vector3& normal) {
	Vector3 in_unit_sphere = rand_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

inline Vector3 reflect(const Vector3& vec, const Vector3& normal)
{
	return vec - 2 * dot(vec, normal) * normal;
}

inline Vector3 refract(const Vector3& vec, const Vector3& normal, float etai_over_etat)
{
	float cos_teta = fmin(dot(-vec, normal), 1.f);
	Vector3 ray_out_perp = etai_over_etat * (vec + cos_teta * normal);
	Vector3 ray_out_parallel = -sqrt(fabs(1.f - ray_out_perp.length_squared())) * normal;
	return ray_out_perp + ray_out_parallel;
}

inline Vector3 rand_in_unit_disk()
{
	while (true)
	{
		Vector3 point = Vector3(random_float(-1.f, 1.f), random_float(-1.f, 1.f), 0.f);
		if (point.length_squared() >= 1)
			continue;
		return point;
	}
}

// Aliases
using Point3D = Vector3; //For 3D point
using Color = Vector3; //For Color(RGB)

#endif // !VEC3_H
