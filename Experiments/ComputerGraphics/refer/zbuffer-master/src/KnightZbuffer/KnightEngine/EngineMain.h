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
//--------------------------------------------------------


#ifndef _ENGINE_MAIN_H_
#define _ENGINE_MAIN_H_

#include "KnightEngine.h"

class EngineMain {
public:
	enum {RENDER_OPENGL, RENDER_DIRECTX};

	EngineMain();
	~EngineMain();
	bool InitializeEngine(WinHWND hwd);	// ��ʹ������
	void ShutDownEngine();	// �ر�����
	bool RenderInitialize();	// ��ʹ������
	void RenderLoop();		// ��Ⱦ
	bool RenderLoopObj(KnightObj *obj,int render_mode);	//����Ӳ����ȾOBJ
	void RenderShutdown();	// ��������

	//---��ת---
	void RotateObject(double x,double y);
	//---�����ӿ�λ��---
	void SetViewport(int x,int y,unsigned int w,unsigned int h);
	//---���������е�ģ����Ϣ
	void RegisterObjectInfo(double scale, Triple<double>& center);
	//---��ģ�ͷ����ӿڵ���
	void FocusObject();
	void setColor(float r,float g,float b){m_engine->SetClearCol( r, g, b);}
	void AntiAliasing(bool anti);
	void ZoomObject(double z_delta);

private:
	KnightEngineInterface *m_engine;	// ����
	int m_render_mode;	// ��Ⱦ��ʽ 
};

#endif