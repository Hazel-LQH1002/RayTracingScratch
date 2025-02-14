#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
class hit_record
{
public:
	point3 p;
	double t;
	vec3 normal;
	bool frontFace;

	void setFrontFace(const ray& r, const vec3& outward_normal)
	{
		frontFace = dot(r.direction(), outward_normal) < 0.0;
		normal = frontFace ? outward_normal : -outward_normal;
	}
};

class hittable
{
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif