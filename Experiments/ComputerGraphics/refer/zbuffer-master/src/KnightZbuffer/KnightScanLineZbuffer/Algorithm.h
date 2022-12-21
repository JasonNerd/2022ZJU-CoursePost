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
// Description	:	��������
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_ALGORITHM_H_
#define _KNIGHT_ALGORITHM_H_


#include "../KnightMath/KnightMath.h"		// �Զ�����ѧ��
#include "../KnightMesh/KnightObj.h"		// objģ�Ͷ���
#include "../KnightMesh/KnightTriple.h"		// 3Ԫ����
#include <list>


//---����α������α�--------------------
struct KnightPolygon{
	double a,b,c;	// ����η���
//	double d;
	int delta_y;	// ������ɨ������
//	Triple<unsigned char> color;	// ��ɫ
	unsigned int num;	// ����α��
//	KnightPolygon *next;	// ����
};

//---�߱�---------------------
struct Edge{
	double delta_x;	// x�仯��
	int delta_y;	// y�仯��
	unsigned int num;	//����α��
	double x;	// �ߵ��϶˵�X�����ֵ
	double z;	// ��߶�������ֵ,�����㷨����û���ᵽ���
//	Edge *next;		// ����
};

//---��߱�-------------
struct EdgeActive{
	double xl,delta_xl;
	double xr,delta_xr;
	double zl,delta_zx,delta_zy;
	unsigned int num;
	int delta_yl,delta_yr;
};

typedef std::list<EdgeActive>::iterator Iterator;

#endif