#ifndef QUAD_H
#define	QUAD_H
#include "hittable.h"
#include "hittable_list.h"

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

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat)
{
	auto sides = make_shared<hittable_list>();

	point3 min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
	point3 max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

	auto dx = vec3(max.x() - min.x(), 0, 0);
	auto dy = vec3(0, max.y() - min.y(), 0);
	auto dz = vec3(0, 0, max.z() - min.z());

	sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()), dx, dy, mat)); // front
	sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
	sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
	sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dz, dy, mat)); // left
	sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
	sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dx, dz, mat)); // bottom

	return sides;
}
#endif // !QUAD_H
