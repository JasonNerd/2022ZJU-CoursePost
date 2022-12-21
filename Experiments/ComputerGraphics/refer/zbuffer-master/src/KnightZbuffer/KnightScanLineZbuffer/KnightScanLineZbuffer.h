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
// Description	:	ɨ����Zbufferʵ��
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef  _KNIGHT_SCANLINEZBUFFER_H_
#define  _KNIGHT_SCANLINEZBUFFER_H_


#include "Algorithm.h"
#include "KnightActiveList.h"
#include "KnightBucket.h"
#include "KnightBuffer.h"
#include <gl/GL.h>
#include <gl/GLU.h>


class KnightScanLineZbuffer{

public:

	//---��ʹ��,���� ���(������/��߶�)   Ͱ(�����Ͱ/��Ͱ)   ֡����----
	KnightScanLineZbuffer();

	//---��ĳ�ʹ��
	//---������ ��������ָ�� ���� ������ָ�� ��������ָ��---
	void InitialZbuffer(int vetex_num,Triple<double> *vertex,
		int face_num, ObjFace *surface , Triple<double> *normal);

	//---��������----
	~KnightScanLineZbuffer();
	
	//====ɨ����Zbuffer�㷨============
	bool StartAlgorithm();
	
	//---��ʹ����Ͱ �����Ͱ
	//---�Ѷ������ݶ������� ������д��Ͱ����----
	//---YͰֵһ��ͳһȡ��-----------------
	void setBucket();
	
	//---��ʹ��֡���� Zbuffer����---
	void InitialBuffers();
	
	//---ɨ�������㷨���------
	bool MainAlgorithm();

	//---������ε�YͰ��������µĶ�����漰��ɨ���ߣ�������������λ����
	//---����������֮�� ҲҪ�Ѹö�����뵱ǰɨ�����ཻ�ı߶� ���뵽��߶Ա�----
	void CheckPolygonBucket_PutItToActive(int num,int &err_code);

	//=====����ÿһ��ɨɨ����========
	//--numΪɨ���� error_codeΪ�������---
	void DealWithActiveEdge(int num,int &err_code);

	//---�Ѹö������ɨ���ߵĽ���߶Է���߻����-----------------------------------
	//---numΪɨ����  poly_numΪ����α��  paraΪ���� a b c
	//--func Ϊ0 ��ʾ�ǵ������µĶ����ʱ ������
	//--func Ϊ1 ��ʾ�ǵ�����ʧʱ �¼����˱߶Ե����Σ�������Ҫ��delta_ylr����һ
	//--err_code Ϊ���صĴ������-----
	bool PutEdgeToAcitive(int num,int poly_num,Triple<double> &para,int func,int &err_code);

	//---ͼ�λ��ƺ���---------
	void DrawLine();


private:
	
	void SetScene();

	//---����ͶӰ����-------
	void ProjectScene();

	int m_viewport[4];		// �ӿھ���
	double m_modelview[16];	// ģ�ͱ任����
	double m_projection[16];	// ͶӰ����
	std::vector<Triple<double> > m_projectionVertex;	// ͶӰ����
	int m_vertex_num,m_surface_num;	// ������������
	Triple<double> *m_vertex;	// ����
	Triple<double> *m_normal_x;	// ����
	std::vector<Triple<double> > m_normal;
	ObjFace *m_surface;	// ��
	KnightActiveList *m_activeList;	// �����α� ��߱�
	KnightBucket *m_bucket;	// �����Ͱ �� ��Ͱ
	KnightZBuffer m_zBuffer;	// zbuffer
	KnightFrameBuffer<Triple<unsigned char> > *m_frameBuffer;	// ֡����
	
	//---�����ɫ����ֵ,֮���Բ��洢�� �����YͰ �е�ԭ�����Ϊ�˿��ٲ��ҵ������ɫ��Ϣ
	//--��Ϊ���Ի�߶Խ��д���ʱ,û�а취���̵õ���߱����ɫ��Ϣ,���ǵ������α���ȥ����
	std::vector<Triple<unsigned char> > m_color;	//�����ɫ
};

#endif