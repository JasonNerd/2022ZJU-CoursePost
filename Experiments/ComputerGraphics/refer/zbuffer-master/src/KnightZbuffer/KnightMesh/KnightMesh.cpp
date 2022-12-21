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
// Description	:	Mesh����ģ�� ���������ʵ��/Ӳ��ʵ�� ���� �ķ�װ
// Time			:	
//--------------------------------------------------------


#include "stdafx.h"
#include "KnightMesh.h"
#include "../KnightGlobal/KnightGlobal.h"
#include "../KnightDebug/KnightDebug.h"


KnightMesh::~KnightMesh()
{
	m_obj.ClearVerSur();//������ݣ���Ҫռ���ڴ�
	delete m_scanline;
	m_scanline = NULL;
}

//---����OBJģ�ͺ���--------------------------------------------------------
bool KnightMesh::LoadMesh(const char* file_path) {
	
//	t.print("trace in KnightMesh::LoadMesh");

	//---��ն��㡢�滺������---
	m_obj.ClearVerSur();

	int vertexNum,surfaceNum;
	//---��һ��ɨ��ͳ��֪��OBJ�ļ��Ķ��㡢��������
	if(!m_obj.ReloadObjFile(file_path,vertexNum,surfaceNum)){
		return false;
	}

	//---�������ö��㡢���VECTOR��С
	m_obj.ResetVerSur(vertexNum,surfaceNum);//---���������Vector���ô���(surface���������ô���)
											//��Ҫ���������Ƭʱ�ͻ�����ڴ����
	//---����Ƭ---
	int vn,sn;
	bool ret=m_obj.ReadPolygonal(file_path,vn,sn);
	if(vn!=vertexNum || sn!=surfaceNum || !ret){
		return false;
	}

	//---����OBJģ�͵���Ϣ-----------
	m_obj.SetProperties();

	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t_stream.print_stream("trace in KnightMesh::LoadMesh");
//	t_stream << "vertexNum:" << vertexNum << "  " <<"surfaceNum:"<< surfaceNum << std::endl;
	//t_stream.offFile_stream();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	m_draw_soft = true;		// ��������Ƿ�ɹ�-------

	return true;
}


//-----Ӳ�����ƺ���-----------------------------------------------
void KnightMesh::DrawHardwareImpl(EngineMain *rMain,int render_mode){

	rMain->RenderLoopObj(&m_obj,render_mode);
}


//-----���ģ����ƺ���----------------------------------------------
void KnightMesh::DrawSoftwareImpl(){

	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	//t_stream.off_stream();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	int vertex_num = m_obj.VertexNumber();
	Triple<double>* vertex = &(m_obj.getVertex())[0];
	int face_num = m_obj.SurfaceNumber();
	ObjFace *surface = &(m_obj.getFace())[0];
	Triple<double> *normal = &(m_obj.getNormal())[0];
	
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
/*	std::vector<Triple<double> > ver = m_obj.getVertex();
	for(int i=0;i<(int)ver.size();++i)
		t_stream << ver[i].x << " " << ver[i].y << " " << ver[i].z << std::endl;
	std::vector<ObjFace> face = m_obj.getFace();
	for(int i=0;i<(int)face.size();++i){
		for(int j=0;j<(int)surface[i].vertexIndex.size();++j){
			t_stream << surface[i].vertexIndex[j] << " ";
		}
		t_stream << " " << std::endl;
	}
*/	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
	
	if(m_draw_soft == false)	// �����һ���������ʧ��,��ô�Ͳ�Ҫ�ٴλ���-------
		return;
	
	//---�������ǰ�ĳ�ʹ����-------------
	m_scanline->InitialZbuffer(vertex_num,vertex,face_num
		,surface,normal);

	//---��������㷨----------------
	if(!m_scanline->StartAlgorithm())
		m_draw_soft = false;	// ����ʧ��,ֹͣ����...
}


//-----���ƺ���-------------------------------------------------------
void KnightMesh::Draw(EngineMain *rMain,int render_mode) {

	//---���m_obj��δ�����ڴ�,��Ҫ��----
	if(!m_obj.IsValid())
		return;

	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t_stream << " in draw "<< render_mode << std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	switch(render_mode){
		case 0:
			// soft
			m_draw_mode = DRAW_SOFTWARE;
			break;
		case 1:
			// directx
			m_draw_mode = DRAW_HARDWARE;
			break;
		case 2:
			// opengl
			m_draw_mode = DRAW_HARDWARE;
			break;
		default:
			break;
	}

	switch (m_draw_mode) {
	case DRAW_HARDWARE: DrawHardwareImpl(rMain,render_mode);
		break;
	case DRAW_SOFTWARE: DrawSoftwareImpl();
		break;
	default:
		break;
	}
}


//---�õ��������Ŀ
int KnightMesh::getVertexNum()
{
	if(m_obj.IsValid())
		return m_obj.VertexNumber();
	return 0;
}


//---�õ������Ŀ
int KnightMesh::getFaceNum()
{
	if(m_obj.IsValid())
		return m_obj.SurfaceNumber();
	return 0;
}