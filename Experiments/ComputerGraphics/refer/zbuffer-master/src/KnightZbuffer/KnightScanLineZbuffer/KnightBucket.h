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
// Description	:	Ͱ�ṹ(�����Ͱ/��Ͱ)
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_BUCKET_H_
#define _KNIGHT_BUCKET_H_


#include "Algorithm.h"


//====Ͱ��==============================
class KnightBucket{

public:

	KnightBucket(){
		m_edge.clear();
		m_polygon.clear();
	}

	~KnightBucket(){
		for(int i=0;i<(int)m_edge.size();++i){
			for(int j=0;j<(int)(m_edge[i].size());++j)
				m_edge[i].clear();
		}
		for(int i=0;i<(int)m_polygon.size();++i){
			for(int j=0;j<(int)m_polygon[i].size();++j)
				m_polygon[i].clear();
		}
		m_edge.clear();
		m_polygon.clear();
	}

	//---��������Ͱ��С��ע�⣬Ҫ����գ���
	void Reset(int height){
		for(int i=0;i<(int)m_edge.size();++i){
			for(int j=0;j<(int)(m_edge[i].size());++j)
				m_edge[i].clear();
		}
		for(int i=0;i<(int)m_polygon.size();++i){
			for(int j=0;j<(int)m_polygon[i].size();++j)
				m_polygon[i].clear();
		}
		m_edge.clear();
		m_polygon.clear();

		m_polygon.resize(height); 
		m_edge.resize(height);
	}
	
	//---�ѱ߼��� �߱�---------------------------------------
	//=== ����˵�x  ����ɨ���ߵ�x�仯��  Y�ı仯��  ��num�������  ��YͰ ���ֵzz
	bool AddEdge(double x,double delta_x,int delta_y,unsigned int num,unsigned int y,double zz);
	
	//---�Ѷ���� ���� �����Ͱ---------------------------------------
	// ƽ�����  numɨ���� delta_y������ɨ��������  color��ɫ yͰ
	bool AddPolygon(Triple<double> para, unsigned int num ,int delta_y, Triple<unsigned char> color, unsigned int y);
	
	//---���numͰ�ж���εĻ����򷵻�poly��������������������, ���򷵻�-1
	int HaveBucketPoly(int num , std::vector<KnightPolygon> &poly);

	//---���ص�num��ɨ�������poly_num������εĽ���,���ر߱�edge , ��������
	//---�ڱ�Ͱ��Ѱ������poly_num����εıߣ�������
	int HaveEdge(int num,int poly_num,std::vector<Edge> & edge);

	//---�õ�ָ���ı� ��num----num+delta_yr��Χ��Ѱ��
	//num:ɨ����  delta_yr:�仯��Yֵ poly_num:ָ���Ķ���� edge_tmp:���صı�
	bool getProperEdge(int num,int delta_yr,int poly_num,Edge & edge_tmp);

	//---��ӡ�� �ǿ� �����Ͱ�е���Ϣ
	void printBucketPloy();

	//---��ӡ�� �ǿձ�Ͱ�бߵ���Ϣ------------------
	void printBucketEdge();

	//---��ӡָ��Ͱ����Ϣ---------------------
	void printBucketEdge(int num);

private:
	std::vector<std::vector<KnightPolygon> > m_polygon;	// �����YͲ
	std::vector<std::vector<Edge> >	m_edge;	//��YͲ

};

#endif