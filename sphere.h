#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

class sphere : public hittable
{
public:

	sphere(point3& centre, double radius):centre(centre),radius(std::fmax(0,radius)){}

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override
	{
        vec3 CQ = centre - r.origin();
        double a = r.direction().length_sqr();
        double b = -2.0 * dot(r.direction(), CQ);
        double c = CQ.length_sqr() - radius * radius;

        double determ = b * b - 4 * a * c;

        if (determ < 0.0)
        {
            return false;
        }
        else
        {
            double root = (-b - std::sqrt(determ)) / (2 * a);
            if (!isInRange(root,ray_tmin,ray_tmax))
            {
                root = (-b + std::sqrt(determ)) / (2 * a);
                if (!isInRange(root, ray_tmin, ray_tmax))
                    return false;
            }
            rec.t = root;
            rec.p = r.at(root);
            vec3 outwardNormal = unit_vector(rec.p - centre);
            rec.setFrontFace(r, outwardNormal);
            return true;
        }
	}

private:
    double radius;
    point3 centre;
};

inline bool isInRange(const double& testNum, const double& min, const double& max)
{
    return (testNum > min && testNum < max);
}
#endif