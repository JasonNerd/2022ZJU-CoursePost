#ifndef __MODEL_H
#define __MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include "Vec.h"

class Vertex
{
public:
	Point3f point;
	Color3f color;
	Vec3f normal;//���㷨����
};

class Face
{
public:
	std::vector<int> vertexIdx;
	std::vector<int> normalIdx;//obj�ļ��ṩ���淨���������㷨������������
	Vec3f normal;//��3��ȷ�����淨����
	Color3f color;//Ϊ��Ԫ�ϸ������ƽ����ɫ
};

//���ڼ���obj�ļ���ģ��
class Model
{
public:
	std::vector<Vertex> vertexes;
	std::vector<Face> faces;
	std::vector<Vec3f> normals;//���㷨����
	Point3f center_point;
	
	bool load(const std::string& path);
	Model(){};
	Model(const std::string& path);
	void rotate(float** RotateMat, std::vector<Vertex>& vertexes);
	
	//void transform(float theta);
};

#endif