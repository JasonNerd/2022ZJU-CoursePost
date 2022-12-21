/////////////////////////////////////////////////////////////////////////////////
//       ___          ___                     ___          ___
//      /__/|        /__/\       ___         /  /\        /__/\         ___
//     |  |:|        \  \:\     /  /\       /  /:/_       \  \:\       /  /\
//     |  |:|         \  \:\   /  /:/      /  /:/ /\       \__\:\     /  /:/
//   __|  |:|     _____\__\:\ /__/::\     /  /:/_/::\  ___ /  /::\   /  /:/
//  /__/\_|:|____/__/::::::::\\__\/\:\__ /__/:/__\/\:\/__/\  /:/\:\ /  /::\
//  \  \:\/:::::/\  \:\~~\~~\/   \  \:\/\\  \:\ /~~/:/\  \:\/:/__\//__/:/\:\
//   \  \::/~~~~  \  \:\  ~~~     \__\::/ \  \:\  /:/  \  \::/     \__\/  \:\
//    \  \:\       \  \:\         /__/:/   \  \:\/:/    \  \:\          \  \:\
//     \  \:\       \  \:\        \__\/     \  \::/      \  \:\          \__\/
//      \__\/        \__\/                   \__\/        \__\/
//
//
// author		:	KNIGHT
// Description	:	Triple 3Ԫ�ṹ
//					����ģ����ʵ��
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_TRIPLE_H_
#define _KNIGHT_TRIPLE_H_

#include "../KnightMath/KnightMath.h"	// ��ѧ��

//-----������������-------------------------
template <typename T>  
struct Triple {
	T x, y, z;

	Triple() {}
	Triple(T ax, T ay, T az) : x(ax), y(ay), z(az) {}
	
	//-----ʸ���ӷ�----------------------
	Triple<T>& operator+=(const Triple<T>& rhs) {	
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	//-----ʸ������-----------------------
	Triple<T> operator-(const Triple<T>& rhs) {
		return Triple<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	//-----���������ʽ----------------
	Triple<T> operator*(const Triple<T>& rhs) {
		return Triple<T>(y * rhs.z - z * rhs.y,
						 z * rhs.x - x * rhs.z,
                         x * rhs.y - y * rhs.x);
	}

	//-----ʸ������ָ������-------------------
	Triple<T>& operator/=(const T& rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	//------ʸ���ĳ���---------------------------
	T VectorLen() const {return sqrt(square(x) + square(y) + square(z));}
};

#endif  