#ifndef QUAD_H
#define	QUAD_H
#include "hittable.h"

class quad : public hittable
{
public:

	quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat): Q(Q), u(u), v(v), mat(mat) 
	{
		vec3 n = cross(u, v);
		normal = unit_vector(n);
		D = dot(normal, Q);
		w = n / dot(n, n);

		setBoundingBox();
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		double denom = dot(normal, r.direction());

		// if ray is parallel to plane
		if (std::fabs(denom) < 1e-8)
		{
			return false;
		}

		//whether hit the infinite plane
		double t = (D - dot(normal, r.origin())) / denom;

		if (!ray_t.contains(t))
		{
			return false;
		}

		// if intersection point is within quad
		point3 intersectionPoint = r.at(t);
		vec3 p = intersectionPoint - Q;
		double alpha = dot(w, cross(p, v));
		double beta = dot(w, cross(u, p));

		bool alphaWithinZeroToOne = interval(0.0, 1.0).contains(alpha);
		bool betaWithinZeroToOne = interval(0.0, 1.0).contains(beta);

		if (!alphaWithinZeroToOne || !betaWithinZeroToOne)
		{
			return false;
		}

		rec.u = alpha;
		rec.v = beta;

		rec.t = t;
		rec.p = intersectionPoint;
		rec.mat = mat;
		rec.setFrontFace(r, normal);

		return true;
	}

	virtual void setBoundingBox()
	{
		aabb bbox1(Q, Q + u + v);
		aabb bbox2(Q + u, Q + v);
		bbox = aabb(bbox1, bbox2);
	}

	aabb bounding_box() const override { return bbox; }


private:
	point3 Q;
	vec3 u, v;
	shared_ptr<material> mat;
	aabb bbox;
	vec3 normal;
	double D;
	vec3 w;
};
#endif // !QUAD_H
