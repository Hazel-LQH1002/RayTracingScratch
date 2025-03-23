#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const { return false; }
};

class lambertian : public material
{
public:
	lambertian(const color& albedo):tex(make_shared<solid_color>(albedo)){}
	lambertian(shared_ptr<texture> tex):tex(tex){}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		scatter_direction = scatter_direction.near_zero() ? rec.normal : scatter_direction;
		scattered = ray(rec.p, scatter_direction, ray_in.time());
		attenuation = tex->value(rec.u,rec.v,rec.p);
		return true;
	}

private:
	shared_ptr<texture> tex;
};

class metal : public material
{
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1){}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto reflect_direction = reflect(ray_in.direction(), rec.normal);
		reflect_direction = unit_vector(reflect_direction) + fuzz * random_unit_vector();
		scattered = ray(rec.p, reflect_direction, ray_in.time());
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double refraction_index) : refraction_index(refraction_index){}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double eta_ratio = rec.frontFace ? 1.0 / refraction_index : refraction_index;
		vec3 unit_vector_of_incomeRay = unit_vector(ray_in.direction());

		double cos_theta = std::fmin(dot(-unit_vector_of_incomeRay, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		vec3 next_ray_direction;
		bool cannot_refract = eta_ratio * sin_theta > 1.0;
		if (cannot_refract || reflectance(cos_theta, eta_ratio) > random_double())
			next_ray_direction = reflect(unit_vector_of_incomeRay, rec.normal);
		else
			next_ray_direction = refract(unit_vector_of_incomeRay, rec.normal, eta_ratio);

		scattered = ray(rec.p, next_ray_direction, ray_in.time());
		return true;
	}

private:
	double refraction_index;

	// Use Schlick's approximation for reflectance
	static double reflectance(double cosine, double refraction_index)
	{
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5); 
	}
};
#endif