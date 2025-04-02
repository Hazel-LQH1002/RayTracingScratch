#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H
#include "aabb.h"

class material;

class hit_record
{
public:
	point3 p;
	double t;
	shared_ptr<material> mat;
	vec3 normal;
	double u;
	double v;
	bool frontFace;

	void setFrontFace(const ray& r, const vec3& outward_normal)
	{
		frontFace = dot(r.direction(), outward_normal) < 0.0;
		normal = frontFace ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:

	virtual ~hittable() = default;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
	virtual aabb bounding_box() const = 0;
};

#endif