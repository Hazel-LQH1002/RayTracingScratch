#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtHeader.h"
class interval
{
public:
	double min, max;

	interval():min(infinity),max(-infinity){}
	interval(double min, double max):min(min),max(max){}
	interval(const interval& a, const interval& b)
	{
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	double size() const
	{
		return max - min;
	}

	double contains(double x) const
	{
		return min <= x && x <= max;
	}

	double surrounds(double x) const
	{
		return min < x && x < max;
	}

	double clamp(double x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	interval expand(double delta) const
	{
		auto padding = delta / 2;
		return interval(min - padding, max + padding);
	}

	static const interval empty;
	static const interval universe;

};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);


#endif