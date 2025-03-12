#pragma once
#ifndef AABB_H
#define AABB_H
#include <algorithm>

class aabb
{
public:
	interval x, y, z;
	aabb(){}

	aabb(const interval& x, const interval& y, const interval& z):x(x),y(y),z(z){}
	aabb(const point3& a, const point3& b)
	{
		x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
	}
	aabb(const aabb& box0, const aabb& box1)
	{
		x = interval(box0.x, box1.x);
		y = interval(box0.y, box1.y);
		z = interval(box0.z, box1.z);
	}

	const interval& axis_interval(int n) const
	{
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const ray& r, interval ray_t) const
	{
		const point3& origin = r.origin();
		const vec3& direction = r.direction();
		
		//x,y,z three axis
		for (int i = 0; i < 3; i++)
		{
			const interval& inter = axis_interval(i);
			const double directInv = 1.0 / direction[i];

			double t0 = (inter.min - origin[i]) * directInv;
			double t1 = (inter.max - origin[i]) * directInv;

			ray_t.min = std::max(ray_t.min, std::min(t0, t1));
			ray_t.max = std::min(ray_t.max, std::max(t0, t1));

			if (ray_t.min >= ray_t.max)
				return false;
		}
		return true;
	}

	int longest_axis()
	{
		if (x.size() > y.size())
			return x.size() > z.size() ? 0 : 2;
		else
			return y.size() > z.size() ? 1 : 2;
	}

	static const aabb empty, universe;
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif
