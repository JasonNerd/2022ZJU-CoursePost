#pragma once
template <typename T>
class Triple
{
public:
	T x, y, z;
	Triple(T _x, T _y, T _z):x(_x), y(_y), z(_z){}
	Triple(T v[3]):x(v[0]), y(v[1]), z(v[2]){}
	Triple(){}
	~Triple(void){}

	void operator = (const Triple<T>& a){
		x = a.x;
		y = a.y;
		z = a.z;
	}

	Triple<T> operator + (const Triple<T>& a/*, Point b*/){
		return Triple<T>(x+a.x, y+a.y, z+a.z);
	}

	Triple<T> operator + (GLdouble a/*, Point b*/){
		return Triple<T>(x+a, y+a, z+a);
	}

	Triple<T> operator * (GLdouble a/*, Point b*/){
		return Triple<T>(x*a, y*a, z*a);
	}

	Triple<T> operator - (const Triple<T>& a/*, Point b*/){
		return Triple<T>(x-a.x, y-a.y, z-a.z);
	}
	bool operator == (const Triple<T>& a/*, Point b*/){
		return (x==a.x&&y==a.y&&z==a.z);
	}
	bool operator != (const Triple<T>& a/*, Point b*/){
		return !((*this)==a);
	}

};

