#pragma once
#include "vector"

class Vector3f {
public:
	double x, y, z;
	Vector3f() : x(0.0), y(0.0), z(0.0) {};

	Vector3f(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline Vector3f operator = (const Vector3f &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline void leftDot(const double Rot[3][3])
	{
		Vector3f v;
		v = *this;
		this->x = Rot[0][0] * v.x + Rot[0][1] * v.y + Rot[0][2] * v.z;
		this->y = Rot[1][0] * v.x + Rot[1][1] * v.y + Rot[1][2] * v.z;
		this->z = Rot[2][0] * v.x + Rot[2][1] * v.y + Rot[2][2] * v.z;
	}
};

class Color {
public:
	double r, g, b;
	Color() : r(0.0), g(0.0), b(0.0) {};

	Color(double r, double g, double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	inline Color operator = (const Color &c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		return *this;
	}
};

inline Vector3f Normalize(const Vector3f &u) {
	double value = sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
	return Vector3f(u.x / value, u.y / value, u.z / value);
}

inline Vector3f Cross(const Vector3f &u, const Vector3f &v) {
	return Vector3f(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x);
}

inline void RotateAxes(Vector3f &v, const double Rot[3][3], Vector3f &center) {
	v.x -= center.x;
	v.y -= center.y;
	v.z -= center.z;
	v.leftDot(Rot);
	v.x += center.x;
	v.y += center.y;
	v.z += center.z;
}

typedef Vector3f Vertex3f;
typedef Vertex3f Normal;

class Face {
public:
	std::vector<Vertex3f> vertexes;
	Color color = Color();
	Normal normal;
};
