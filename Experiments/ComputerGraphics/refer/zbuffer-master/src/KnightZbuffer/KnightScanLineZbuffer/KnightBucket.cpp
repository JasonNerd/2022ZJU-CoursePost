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
// Description	:	Ͱʵ��(�����Ͱ/��Ͱ)
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "KnightBucket.h"
#include "../KnightGlobal/KnightGlobal.h"
#include "../KnightDebug/KnightDebug.h"


//---�ѱ߼��� �߱�---------------------------------------
//=== ����˵�x  ����ɨ���ߵ�x�仯��  Y�ı仯��  ��num�������  ��YͰ ���ֵzz
bool KnightBucket::AddEdge(double x,double delta_x,int delta_y,unsigned int num,unsigned int y,double zz)
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//t_scanLine << "KnightBucket::AddEdge. insert edge: ɨ������ " << y << " ���ڵ� " << num << " �������"  << std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	Edge edge;
	edge.delta_x = delta_x;
	edge.delta_y = delta_y;
	edge.num = num;
	edge.x = x;
	edge.z = zz;
	m_edge[y].push_back(edge);
	
	return true;
}


//---�Ѷ���� ���� �����Ͱ---------------------------------------
// ƽ�����  numɨ���� delta_y������ɨ��������  color��ɫ yͰ
bool KnightBucket::AddPolygon(Triple<double> para, unsigned int num , int delta_y, 
							  Triple<unsigned char> color, unsigned int y)
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//t_scanLine << "KnightBucket::AddPolygon. insert poly: ɨ������ " << y << " ���ڵ� " << num << " �������"  << std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	KnightPolygon polygon;
	polygon.a = para.x;
	polygon.b = para.y;
	polygon.c = para.z;
//	polygon.color = color;
	polygon.delta_y = delta_y;
	polygon.num = num;
	m_polygon[y].push_back(polygon);

	return true;
}


//---���numͰ�ж���εĻ����򷵻�poly��������������������, ���򷵻�-1
int KnightBucket::HaveBucketPoly(int num , std::vector<KnightPolygon> &poly)
{
	if(m_polygon[num].size()==0){
		return -1;
	}
	else{
		poly = m_polygon[num];
		
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << "KnightBucket::HaveBucketPoly. ɨ���� " << num << " ��Ͱ�еĶ������" << m_polygon[num].size() << " �� " << std::endl;
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		return (int)m_polygon[num].size();
	}

}

//---���ص�num��ɨ�������poly_num������εĽ���,���ر߱�edge , ��������
//---�ڱ�Ͱ��Ѱ������poly_num����εıߣ�������
int KnightBucket::HaveEdge(int num,int poly_num,std::vector<Edge> & edge)
{
	int countt=0;
	if(m_edge[num].size()==0){
		return -1;
	}
	else{
		for(int i=0;i<(int)m_edge[num].size();++i){
			if(m_edge[num][i].num == poly_num){
				edge.push_back(m_edge[num][i]);
				++countt;
			}	
		}
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
		//t_scanLine << "KnightBucket::HaveEdge. ɨ���� " << num << "  ��Ͱ�еı���" << countt << " �� " << std::endl;
		//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

		return countt;
	}
}


//---�õ�ָ���ı� ��num----num+delta_yr��Χ��Ѱ��
//num:ɨ����  delta_yr:�仯��Yֵ poly_num:ָ���Ķ���� edge_tmp:���صı�
bool KnightBucket::getProperEdge(int num,int delta_yr,int poly_num,Edge & edge_tmp)
{
	for(int i=num;i<=(int)(num+delta_yr);++i){
		for(int j=0;j<(int)m_edge[i].size();++j){
			if(m_edge[i][j].num == poly_num){
				edge_tmp = m_edge[i][j];
				return true;
			}
		}
	}
	return false;
}


//---��ӡ�� �ǿ� �����Ͱ�е���Ϣ
void KnightBucket::printBucketPloy()
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	for(int i=0;i<(int)m_polygon.size();++i){
		if(m_polygon[i].size() == 0)
			continue;
		t_scanLine << "poly buket " << i  << " : " ;
		for(int j=0;j<(int)m_polygon[i].size();++j)
			t_scanLine << m_polygon[i][j].num <<  " " ;
		t_scanLine << " " <<std::endl;
	}
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
}


//---��ӡ�� �ǿձ�Ͱ�бߵ���Ϣ------------------
void KnightBucket::printBucketEdge()
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	for(int i=0;i<(int)m_edge.size();++i){
		if(m_edge[i].size() == 0)
			continue;
		t_scanLine << "edge buket " << i <<  " : " ;
		for(int j=0;j<(int)m_edge[i].size();++j)
			t_scanLine << m_edge[i][j].num <<  " " ;
		t_scanLine << " " <<std::endl;
	}
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
}

//---��ӡָ��Ͱ����Ϣ---------------------
void KnightBucket::printBucketEdge(int num)
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	t_scanLine << "edge buket " << num <<  " : " <<std::endl;
	for(int j=0;j<(int)m_edge[num].size();++j)
		t_scanLine << m_edge[num][j].num 
		<< " delta_y:" << m_edge[num][j].delta_y << " delta_x: " << m_edge[num][j].delta_x 
		<< " x: " << m_edge[num][j].x << std::endl;
	t_scanLine << " " <<std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
}