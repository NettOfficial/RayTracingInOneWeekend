#pragma once

#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray {
public:
	Ray() {};
	Ray(const Point3D& origin, const Vector3& direction) :
		m_origin(origin), m_direction(direction)
	{
	}

	Point3D origin() const { return m_origin; }
	Vector3 direction() const { return m_direction; }

	Point3D at(float t) const { return m_origin + t * m_direction; }

public:
	Point3D m_origin;
	Vector3 m_direction;
};

#endif // !RAY_H
