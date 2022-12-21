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
// Description	:	Mesh����ģ��
//					�����ʵ�ַ�ʽ MeshΪ������ ����OBJģ��
// Time			:	2008/12/20
// Email		:	KnightLiao@gmail.com	liaoqiqi@cad.zju.edu.cn
//////////////////////////////////////////////////////////////////////////////////


#ifndef _KNIGHT_MESH_H_
#define _KNIGHT_MESH_H_


#include "KnightObj.h"
#include "../KnightEngine/EngineMain.h"
#include "../KnightScanLineZbuffer/KnightScanLineZbuffer.h"


class KnightMesh {
public:
	//-----���Ʒ�ʽ-----
	//---Ӳ�������ģ��
	enum {DRAW_HARDWARE, DRAW_SOFTWARE};

	//-----���캯��,���ƹ���,��ֵ����-----
	KnightMesh() {
		DrawMode(DRAW_HARDWARE);
		m_scanline = new KnightScanLineZbuffer;
		m_draw_soft = false;
	}
	
//	KnightMesh(const KnightMesh& rhs);
	
	~KnightMesh();
	
	KnightMesh& operator=(const KnightMesh& rhs);

	//-----����mesh-----
	bool LoadMesh(const char* file_path);

	//-----���û��Ʒ�ʽ-----
	void DrawMode(int mode) {m_draw_mode = mode;}

	//-----���ػ��Ʒ�ʽ-----
	int DrawMode() const {return m_draw_mode;}

	//-----���ƺ���-----
	void Draw(EngineMain *,int render_mode);

	//---�õ�������---
	int getVertexNum();

	//---�õ�����---
	int getFaceNum();

	Triple<double>&  getCenter(){return m_obj.Center();}	// �������������

	double getScale(){return m_obj.Scale();}		// ģ�͵ĳߴ�

	bool IsValid(){return m_obj.IsValid();}	//	Mesh�Ƿ���Ч

private:
	//-----�ڲ����ƺ���-----
	//---Ӳ�� ��� ���ƺ���-----
	void DrawHardwareImpl(EngineMain *,int render_mode);

	void DrawSoftwareImpl();

	int m_draw_mode;	// ���Ʒ�ʽ:Ӳ����
	bool m_draw_soft;	// ��������Ƿ�ɹ�
	KnightObj m_obj;		// OBJģ��
	KnightScanLineZbuffer *m_scanline;	// ���ɨ����Zbuffer�㷨��
};

#endif