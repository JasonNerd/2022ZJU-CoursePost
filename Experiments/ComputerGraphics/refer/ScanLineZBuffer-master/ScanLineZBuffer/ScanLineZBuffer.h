#ifndef __SCANLINEZBUFFER_H
#define __SCANLINEZBUFFER_H

#include <vector>
#include <list>
#include "Model.h"
#include "utils.h"
#include "Rasterizer.h"
#include "Shader.h"
#include <omp.h>
#include <algorithm>
#include <cassert>
#include <ctime>



struct Edge
{
	float x;//�ߵ��϶˵��x����
	float dx;//��������ɨ���߽����x�����,-1/k
	int dy;//�߿�Խ��ɨ������Ŀ
	int id;//����������εı��
};

//����Ļ�߱���μ��еĲ�ͬ��
//��ߵĽ��㣬������ߺ��ұߣ�����xl��xr
struct ActiveEdge
{
	float x; // �����x����
	float dx; // (�������)������ɨ���߽����x����֮��.-1/k
	int dy; // �Ժͽ������ڱ��ཻ��ɨ������Ϊ��ֵ���Ժ�����ÿ����һ��ɨ���߼�1
	float z; // ���㴦���������ƽ������ֵ
	float dzx; // ��ɨ���������߹�һ������ʱ�����������ƽ����������������ƽ�淽�̣�dzx = -a/c(c!=0)
	float dzy; // ��y���������ƹ�һ��ɨ����ʱ�����������ƽ����������������ƽ�淽�̣�dzy = b/c(c!=0)
	int id; // ��������ڵĶ���εı��
};

//����Ķ���α߱�Ԫ�أ�����¼�������ɫ��ͨ��id��ѯshader.model��face��color�õ�
//�����˻�߱�
struct Polygon
{
	float a, b, c, d;//���������ƽ��ķ���ϵ����a^2+b^2+c^2=1����������(a,b,c)
	int id;//����α��
	int dy;//����ο�Ծ��ɨ������Ŀ
	//Color3f color;
	std::vector< ActiveEdge> activeEdgeTable;
};

class ScanLineZBuffer
{
public:
	ScanLineZBuffer();
	~ScanLineZBuffer();
	void setSize(int width, int height);
	void ScanLineZBuffer::getSize(int& width, int& height);
	void run(Model& model);
	int** idBuffer;//��¼����ε�id�����ڲ�ѯmodel.faces[i].color
	bool needUpdate;//���ڴ�С�ı�ʱ����Ҫ���и��£�δ������ת��ʱ�����




private:
	int width, height;
	float* zBuffer;

	std::vector<std::list<Polygon>> polygonTable;
	std::vector<std::list<Edge>> edgeTable;
	std::vector<Polygon> activePolygonTable;//�����˻�߱�

	void release();//�ͷ�zBuffer��idBuffer
	void build(const Model& model);//��������α��߱�
	void addEdge(int y, Polygon* active_polygon);//����ر߼ӵ���߱���

};



#endif