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
// Description	:	OpenGL��������
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef KNIGHT_OPENGL_H
#define KNIGHT_OPENGL_H


//---����ͷ�ļ�---
#include <gl/GL.h>
#include <gl/GLU.h>
#include "KnightEngineInterface.h"
#include "../KnightMesh/KnightObj.h"

//#include "../KnightMesh/KnightTriple.h"
//---����opengl��---
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")


class KnightOpenGL : public KnightEngineInterface{
public:

	KnightOpenGL(){}

	~KnightOpenGL();

	//---��ʹ��OPENGL��ʹ֮�뻷�����---
	bool Initialize(int w,int h, WinHWND mainWin,bool fullScreen);	

	//--�ر�����
	void Shutdown();

	//--���ñ�����ɫ 
	void SetClearCol(float r, float g, float b);

	//--��ʼ��Ⱦ
	void StartRender(bool bColor, bool bDepth, bool bStencil);

	//--������Ⱦ
	void EndRendering();

	void ClearBuffers(bool bColor, bool bDepth, bool bStencil);
	void CalculateProjMatrix(float fov, float n, float f);
	void CalculateOrthoMatrix(float n, float f);
	int CreateStaticBuffer(VertexType, PrimType,
		int totalVerts, int totalIndices,
		int stride, void **data, unsigned int *indices,
		int *staticId);

	//--��ת
	void RotateObject(double x,double y);

	//--��Ⱦ
	int Render(int staticId);

	//--�����ӿ�
	void SetViewport(int x,int y,unsigned int w,unsigned int h);

	//--��䶥�������Ϣ
	bool createVertexFace(KnightObj* obj);

	//--���Ƴ�ʹ��
	bool renderInitial();

	//--����������Ϣ
	void RegisterObjectInfo(double scale,  Triple<double> &center);

	//--��ģ�ͷ����ӿ���
	void FocusObject();
	void AntiAliasing(bool anti);
	void ZoomObject(double);

private:
	//---����OPENGL״̬--- 
	void OneTimeInit();
	void setStatus();	// ֻ���ڲ����ã���Initialize���ù�

	HGLRC m_render_contex;	// ����������
};

//=====����opengl���� 
bool CreateOpenGLRenderer(KnightEngineInterface **pObj);

#endif