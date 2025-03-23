#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

class texture
{
public:
	virtual ~texture() = default;
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
	solid_color(const color& albedo):albedo(albedo){}
	solid_color(double r, double g, double b):albedo(color(r,g,b)){}
	color value(double u, double v, const point3& p) const override { return albedo; }
private:
	color albedo;

};

class checker_texture : public texture
{
public:
	checker_texture(double scale, shared_ptr<texture> even_tex, shared_ptr<texture> odd_tex):odd(odd_tex), even(even_tex), inv_scale(1.0 / scale){}
	checker_texture(double scale, const color& color1, const color& color2):odd(make_shared<solid_color>(color1)), even(make_shared<solid_color>(color2)), inv_scale(1.0 / scale){}
	color value(double u, double v, const point3& p) const override
	{
		int intX = int(std::floor(inv_scale * p.x()));
		int intY = int(std::floor(inv_scale * p.y()));
		int intZ = int(std::floor(inv_scale * p.z()));
		bool isEven = (intX + intY + intZ) % 2 == 0;
		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}
private:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
	double inv_scale;
};

#endif // !TEXTURE_H
