#pragma once

#ifndef VEC3_H
#define VEC3_H

class vec3
{
public:
	double e[3];

	vec3() :e{ 0,0,0 } {};

	vec3(double e0, double e1, double e2) : e{ e0,e1,e2 } {};

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }
	double& operator[](int i) { return e[i]; }
	double operator[](int i) const { return e[i]; }


	vec3& operator+=(const vec3& other)
	{
		e[0] += other.e[0];
		e[1] += other.e[1];
		e[2] += other.e[2];
	
		return *this;
	}

	vec3& operator*=(const vec3& other)
	{
		e[0] *= other.e[0];
		e[1] *= other.e[1];
		e[2] *= other.e[2];
	
		return *this;
	}
	
	vec3& operator/=(double t)
	{
		e[0] *= 1/t;
		e[1] *= 1/t;
		e[2] *= 1/t;

		return *this;
	}

	double length_sqr() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double length() const
	{
		return std::sqrt(length_sqr());
	}

	static double lenSqr(const vec3& vec)
	{
		return vec.e[0] * vec.e[0] + vec.e[1] * vec.e[1] + vec.e[2] * vec.e[2];
	}

	static double len(const vec3& vec)
	{
		double length_square = lenSqr(vec);
		return std::sqrt(length_square);
	}

	static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max)
	{
		return vec3(random_double(min,max), random_double(min, max), random_double(min, max));
	}
};

using point3 = vec3;

// Utility Function

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(double t, const vec3& v)
{
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}


inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

inline vec3 operator/(const vec3& v, double t)
{
	return (1/t) * v;
}

inline double dot(const vec3& v1, const vec3& v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1], v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2], v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}

inline vec3 random_unit_vector()
{
	while (true)
	{
		vec3 p = vec3::random(-1, 1);
		double lenSqr = p.length_sqr();
		if (lenSqr <= 1 && lenSqr > 1e-160)
		{
			return unit_vector(p);
		}
	}
}

inline vec3 random_unit_vector_on_hemisphere(const vec3& normal)
{
	vec3 p = random_unit_vector();
	return dot(p, normal) > 0.0 ? p : -p;
}
#endif // !VEC3_H
