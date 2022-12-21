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


#ifndef  _KNIGHT_ACTIVE_H_
#define  _KNGIHT_ACTIVE_H_


#include "Algorithm.h"
#include <list>


typedef std::list<KnightPolygon>::iterator IteratorPoly;

class KnightActiveList{
public:
	//---�ͷ��ڴ�
	~KnightActiveList();
	KnightActiveList();
	
	//---��߱������α� ���
	void Reset(){
		m_edgeActive.clear();
		m_polygonActive.clear();
	}
	
	//---�Ѷ���β�����߱���
	void AddActivePoly(KnightPolygon &poly);
	
	//---�ѱ߲����߱���-----------------
	void AddActiveEdge(int poly_num,EdgeActive &edge_active);

	//---�õ���߱�
	std::list<EdgeActive>  *getEdgeActive();

	//---ɾ�������
	void eraseEdgeActive(std::list<EdgeActive>::iterator it);

	//---�õ�ָ�������ε�delta_y
	int getPolyDeltaY(int num);

	//---ɾ����ָ��������
	void erasePolyActive(int num);

	//---�Ի����ε�ÿ��delta_y��һ
	void PolySubDeltaY();

	//---��Y�仯��С��1��ÿ�������ɾ��-----------
	void PolyUpdate();

	//---��ӡ�� ��� �е���Ϣ----------------------
	void printEdge();

	//---��ӡ�� ������ �е���Ϣ-----
	void printPoly();

	//*****��ӡ��߱����Ϣ*************************
	void printEdgeInfo(int num);

private:
	std::list<EdgeActive> m_edgeActive;		// ��߱�
	std::list<KnightPolygon> m_polygonActive;	// �����α�
};

#endif