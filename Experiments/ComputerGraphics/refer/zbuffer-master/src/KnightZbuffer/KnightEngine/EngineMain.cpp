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


#include "stdafx.h"
#include "EngineMain.h"


EngineMain::EngineMain()
{
	m_engine = NULL;
	m_render_mode = RENDER_OPENGL;	// Ĭ����OPENGL
}


EngineMain::~EngineMain()
{
	RenderShutdown();
}


//=====��ʹ�����棬����DX��OPENGL
bool EngineMain::InitializeEngine(WinHWND hwd)
{
//	t.print("trace in EngineMain::InitializeEngine\n");

	//---��������---
	switch(m_render_mode){
	case RENDER_OPENGL:
		if(!CreateOpenGLRenderer(&m_engine))
			return false;
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}

	//---��ʹ������---
	if(!m_engine->Initialize(0,0,hwd,0))
		return false;

	//---���ñ�����ɫ---
	m_engine->SetClearCol(160.0f/255.0f,160.0f/255.0f,160.0f/255.0f);
	
	return true;
}


//=====��תģ��========================
void EngineMain::RotateObject(double x,double y)
{
//	t.print("trace in EngineMain::RotateObject\n");

	switch(m_render_mode){
	case RENDER_OPENGL:
		m_engine->RotateObject(x,y);
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}
}


//=====�ر�����
void EngineMain::ShutDownEngine()
{
	if(m_engine)
	{
		m_engine->Shutdown();
		delete m_engine;
		m_engine = NULL;
	}
}


//====��Ⱦ
void EngineMain::RenderLoop()
{
	if(!m_engine)
		return ;
	m_engine->StartRender(1,1,1);

	m_engine->EndRendering();
}


//=====�ر�����=====
void EngineMain::RenderShutdown()
{
	delete m_engine;
}


//=====�����ӿ�=====
void EngineMain::SetViewport(int x,int y,unsigned int w,unsigned int h)
{
//	t.print("trace in EngineMain::SetViewport\n");

	switch(m_render_mode){
	case RENDER_OPENGL:
		m_engine->SetViewport(x,y,w,h);
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}
}


//=====��ʵ����OBJģ�͵���Ⱦ================
bool EngineMain::RenderLoopObj(KnightObj *obj,int render_mode)
{
//	t.print("trace in EngineMain::RenderLoopObj\n");
	
	if(render_mode==1)//directx
		m_render_mode = RENDER_DIRECTX;
	else
		m_render_mode = RENDER_OPENGL;

	switch(m_render_mode){
	case RENDER_OPENGL:
		m_engine->StartRender(1,1,0);	// ������ɫ�������
		m_engine->renderInitial();	// ��Ⱦ��ʹ����
		m_engine->createVertexFace(obj);//---��䶥�㡢�桢��������---	
		m_engine->EndRendering();	// ������Ⱦ
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}
	
	return true;
}


//======���������е�ģ����Ϣ
void EngineMain::RegisterObjectInfo(double scale,  Triple<double> &center)
{
//	t.print("trace in EngineMain::RegisterObjectInfo\n");

	switch(m_render_mode){
	case RENDER_OPENGL:
		//---��䶥�㡢�桢��������---
		m_engine->RegisterObjectInfo(scale,center);
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}
}


//==============��Ҫ�ǰ�ģ�ͷ�����Ļ���м��λ��========================
void EngineMain::FocusObject()
{
//	t.print("trace in EngineMain::FocusObject\n");

	switch(m_render_mode){
	case RENDER_OPENGL:
		//---��䶥�㡢�桢��������---
		m_engine->FocusObject();
		break;
	case RENDER_DIRECTX:
		//----
		break;
	default:
		break;
	}
}


//=========
bool EngineMain::RenderInitialize()
{
	return true;
}


//===����������====================
void EngineMain::AntiAliasing(bool anti)
{
	m_engine->AntiAliasing(anti);
}


//===�Ŵ��Сģ��==================
void EngineMain::ZoomObject(double z_delta)
{
	m_engine->ZoomObject(z_delta * 0.001);
}


/*

KnightEngineInterface* EngineMain::GetEngine()
{
return m_engine;
}*/