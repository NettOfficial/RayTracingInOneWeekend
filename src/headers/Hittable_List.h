#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <vector>

using std::shared_ptr;
using std::make_shared;

class Hittable_List : public Hittable {
public:
	Hittable_List() {};
	Hittable_List(shared_ptr<Hittable> object)
	{
		add(object);
	}

	void clear() { m_objects.clear(); }
	void add(shared_ptr<Hittable> object) { m_objects.push_back(object); }

	virtual	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const override;

public:
	std::vector<shared_ptr<Hittable>> m_objects;
};

bool Hittable_List::hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const
{
	HitRecord tmp_record;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for (const auto& object : m_objects)
	{
		if (object->hit(ray, t_min, closest_so_far, tmp_record))
		{
			hit_anything = true;
			closest_so_far = tmp_record.t;
			record = tmp_record;
		}
	}

	return hit_anything;
}

#endif // !HITTABLE_LIST_H
