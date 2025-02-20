#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const { return false; }
};

class lambertian : public material
{
public:
	lambertian(const color& albedo):albedo(albedo){}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		scatter_direction = scatter_direction.near_zero() ? rec.normal : scatter_direction;
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class metal : public material
{
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1){}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto reflect_direction = reflect(ray_in.direction(), rec.normal);
		reflect_direction = unit_vector(reflect_direction) + fuzz * random_unit_vector();
		scattered = ray(rec.p, reflect_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
	double fuzz;
};
#endif