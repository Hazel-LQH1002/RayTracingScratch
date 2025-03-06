#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

inline bool isInRange(const double& testNum, const double& min, const double& max)
{
    return (testNum > min && testNum < max);
}

class sphere : public hittable
{
public:

	sphere(const point3& static_centre, double radius, shared_ptr<material> mat):centre(static_centre, vec3(0,0,0)),radius(std::fmax(0,radius)), mat(mat){}
	sphere(const point3& moving_centre1, const point3& moving_centre2, double radius, shared_ptr<material> mat):centre(moving_centre1,moving_centre2 - moving_centre1),radius(std::fmax(0,radius)), mat(mat){}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
        point3 current_centre = centre.at(r.time());
        vec3 CQ = current_centre - r.origin();
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
            if (!ray_t.surrounds(root))
            {
                root = (-b + std::sqrt(determ)) / (2 * a);
                if (!ray_t.surrounds(root))
                    return false;
            }
            rec.t = root;
            rec.p = r.at(root);
            vec3 outwardNormal = unit_vector(rec.p - current_centre);
            rec.setFrontFace(r, outwardNormal);
            rec.mat = mat;
            return true;
        }
	}

private:
    double radius;
    ray centre;
    shared_ptr<material> mat;
};


#endif