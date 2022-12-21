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
// Description	:	��������ӿ�
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_ENGINEINTERFACE_H_
#define _KNIGHT_ENGINEINTERFACE_H_


#include "KnightDefine.h"
#include "../KnightGlobal/KnightGlobal.h"	// ������
#include "../KnightMesh/KnightTriple.h"
#include "../KnightMesh/KnightObj.h"


class KnightEngineInterface
{
public:

	KnightEngineInterface() : m_screenWidth(0),
		m_screenHeight(0), m_near(0), m_far(0) { }
	virtual ~KnightEngineInterface() {}	// ������������������Ա�֤������������ʱ���Ե�����ȷ����������

	//---��ʹ��---
	virtual bool Initialize(int w, int h,WinHWND mainWin, bool fullScreen) = 0;	

	//---��ʱ����---
	virtual void OneTimeInit() = 0;

	//---�����Ⱦϵͳ--- 
	virtual void Shutdown() = 0;

	//---����̨������ɫ����Ϊָ����ɫ---
	virtual void SetClearCol(float r, float g, float b) = 0;

	//---�����³���---
	virtual void StartRender(bool bColor, bool bDepth,bool bStencil) = 0;
	
	virtual void ClearBuffers(bool bColor, bool bDepth,
		bool bStencil) = 0;

	//---�����³���---
	virtual void EndRendering() = 0;

	//---����ͶӰ---
	virtual void CalculateProjMatrix(float fov, float n, float f) = 0;

	//---����ͶӰ---
	virtual void CalculateOrthoMatrix(float n, float f) = 0;

	//---������̬���㻺��---
	virtual int CreateStaticBuffer(VertexType, PrimType,
		int totalVerts, int totalIndices,
		int stride, void **data, unsigned int *indices,
		int *staticId) = 0;

	//---������������ʾ����Ļ��
	virtual int Render(int staticId) = 0;

	//---��ת
	virtual void RotateObject(double x,double y)=0;

	//---�ӿ�λ�� 
	virtual void SetViewport(int x,int y,unsigned int w,unsigned int h)=0;

	virtual bool createVertexFace(KnightObj* obj)=0;
	virtual bool renderInitial()=0;
	virtual void RegisterObjectInfo(double scale,  Triple<double> &center) = 0;
	virtual void FocusObject() =0;
	virtual void AntiAliasing(bool anti) = 0;
	virtual void ZoomObject(double)=0;


protected:	// ������������������ĳ�Ա����
	int m_screenWidth;	// ��Ļ���
	int m_screenHeight;	// ��Ļ�߶�
	bool m_fullscreen;	// �Ƿ�ȫ��

	WinHWND m_mainHandle;//���ھ��

	float m_near;	// ͶӰ����Ľ�����ֵ
	float m_far;	// Զ����ֵ

	double m_z_delta;	// z����
	Triple<double> m_center;	//����λ�� 
	double m_scale;	//��С
};

#endif
