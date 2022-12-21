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


#include "stdafx.h"
#include "KnightOpenGl.h"


//-=====����������� ѡ��OPENGL
bool CreateOpenGLRenderer(KnightEngineInterface **pObj)
{
//	t.print("trace in CreateOpenGLRenderer\n");

	if(!*pObj) 
		*pObj = new KnightOpenGL;
	else 
		return false;

	return true;
}


KnightOpenGL::~KnightOpenGL()
{
	Shutdown();
}


//====��ʹ����������================
bool KnightOpenGL::Initialize(int w,int h,WinHWND hdc,bool fullScreen) {

//	t.print("trace in KnightOpenGL::Initialize\n");

	//=====����OPENGL��ʹ��======
	//---�������ظ�ʽ����---
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		::GetDeviceCaps(hdc, BITSPIXEL),	// �õ�������ɫλ�� Number of adjacent color bits for each pixel
		0,0,0,0,0,0,0,0,
		0,0,0,0,0, 
		32,
		0,0, 
		PFD_MAIN_PLANE, 
		0, 
		0,0,0
	};
	//---�õ�������Ѹ�ʽ---
	//---match an appropriate pixel format supported by a device context to a given pixel format specification.
	int pixel_format;
	if(!(pixel_format = ChoosePixelFormat(hdc, &pfd))){
		//MessageBox("ChoosePixelFormat failed");
		return false;
	}
	//---�������ظ�ʽ---
	//---sets the pixel format of the specified device context to the format specified by the iPixelFormat index.
	SetPixelFormat(hdc, pixel_format, &pfd);
	//---�õ�OPENGL����������,�õ�һ�������DC���ݵ���Ⱦ���������óɹ�������Ⱦ�������������ɹ�����NULL---
	//---creates a new OpenGL rendering context, which is suitable for drawing on the device referenced by hdc. 
	//---The rendering context has the same pixel format as the device context.
	m_render_contex = wglCreateContext(hdc);
	//---���õ�ǰ����������,��Ⱦ������ǰ��,���óɹ�����GL_TRUE
	//---makes a specified OpenGL rendering context the calling thread's current rendering context
	wglMakeCurrent(hdc, m_render_contex);

	//=====�����Ѿ����ú�OPENGL����Ⱦ����������������OPENGL״̬��=====
	setStatus();
	return true;
}


//---���ó�ʹ״̬-------------
void KnightOpenGL::setStatus()
{
//	t.print("trace in KnightOpenGL::setStatus\n");

	glDisable(GL_DEPTH_TEST);	// �ر�ZBUFFER��Ȳ���*****************************
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);	// ����˫�����,���� ����
	glClearColor(160/255,160/255,160/255,1.0f);	// ����Ļ��ճɰ�ɫ Ĭ��
	glShadeModel(GL_FLAT);	// ʹ����ɫģʽ��ƽ����ɫ
	glEnable(GL_LIGHTING);	// ��������
	glEnable(GL_LIGHT0);	// �������յ�1
	static const float mat_amb_diff[] = {0.0f, 0.0f, 0.0f, 1.0f };	// ����ɢ���ͻ����ⷴ����ɫ
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
	m_z_delta = 0.0;	// ģ�ͷ���������������
	glDisable(GL_DEPTH_TEST);	// ������Ȳ���----****
}


//---��תģ��---
void KnightOpenGL::RotateObject(double x,double y)
{
//	t.print("trace in KnightOpenGL::RotateObject\n");

	double modleview_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modleview_matrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//---���Ƶ�ԭ��----
	glTranslated(0.0,0.0,m_z_delta);
	//---��ת---
	glRotated(sqrt((double)(x*x+y*y)),x,y,0.0);
	//---���ƻ�ԭ��---
	glTranslated(0.0,0.0, -m_z_delta);
	glMultMatrixd(modleview_matrix);
}


//=====�ر�����===============================
void KnightOpenGL::Shutdown()
{
//	t.print("trace in KnightOpenGL::Shutdown\n");

	wglMakeCurrent(NULL, NULL);
	if (m_render_contex != NULL) {
		wglDeleteContext(m_render_contex);
		m_render_contex = NULL;
	}
}


//=====���������ɫ==========================
void KnightOpenGL::SetClearCol(float r, float g, float b)
{
//	t.print("trace in KnightOpenGL::SetClearCol\n");

	//---���������ɫ---
	glClearColor(r,g,b,1);
	//---������Ļ����ɫ---
	glClear(GL_COLOR_BUFFER_BIT);
}


//=====��ʼ��Ⱦ�����û���=====================
void KnightOpenGL::StartRender(bool bColor, bool bDepth,
							   bool bStencil)
{
//	t.print("trace in KnightOpenGL::StartRender\n");

	if(bColor==true)
		glClear(GL_COLOR_BUFFER_BIT);
	if(bDepth==true)
		glClear(GL_DEPTH_BUFFER_BIT);
	if(bStencil)
		glClear(GL_STENCIL_BITS);
}


//=====������Ⱦ===================
void KnightOpenGL::EndRendering()
{
//	t.print("trace in KnightOpenGL::EndRendering\n");

	glFinish();
}


//===�����ӿ�==============
void KnightOpenGL::SetViewport(int x,int y,unsigned int w,unsigned int h)
{
//	t.print("trace in KnightOpenGL::SetViewport\n");

	glViewport(x,y,w,h);
}


//=====���ö��������Ϣ===========================
bool KnightOpenGL::createVertexFace(KnightObj* obj)
{
//	t.print("trace in KnightOpenGL::createVertexFace\n");

	std::vector<Triple<double> > vertex_ptr = obj->getVertex();
	std::vector<Triple<double> > normal_ptr = obj->getNormal();
	std::vector<ObjFace> face_ptr = obj->getFace();
	
	for(int i=0;i<(int)face_ptr.size();++i){
		glBegin(GL_POLYGON);
		glNormal3d(normal_ptr[i].x,normal_ptr[i].y,normal_ptr[i].z);
		t_stream << normal_ptr[i].x << " " << normal_ptr[i].y << " "<< normal_ptr[i].z << std::endl;
		for(int j=0;j<(int)face_ptr[i].vertexIndex.size();++j){
			glVertex3d(vertex_ptr[face_ptr[i].vertexIndex[j]].x,
				vertex_ptr[face_ptr[i].vertexIndex[j]].y,
				vertex_ptr[face_ptr[i].vertexIndex[j]].z);
			t_stream << vertex_ptr[face_ptr[i].vertexIndex[j]].x << " " << 
				vertex_ptr[face_ptr[i].vertexIndex[j]].y << " " << 
				vertex_ptr[face_ptr[i].vertexIndex[j]].z << std::endl;
		}
		glEnd();
	}

	t_stream.offFile_stream();
	return true;
}


//======����ǰ��׼��==================
bool KnightOpenGL::renderInitial()
{
//	t.print("trace in KnightOpenGL::renderInitial\n");

	// set 3Dprojection
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,viewport[2]/(double)viewport[3],1e-2,1e6);
	//----ɢ�����ɫ-------------
	const float color[] = {77.0f/255.0f, 91.0f/255.0f, 230.0f/255.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	// Ӳ�� ������Ȳ���---------
	glEnable(GL_DEPTH_TEST);
 
	return true;
}


//====����ģ������������Ϣ================
void KnightOpenGL::RegisterObjectInfo(double scale,  Triple<double> &center)
{
	m_scale = scale;
	m_center = center;
}


//=====��ģ�ͷ������ӿڿռ���============================
void KnightOpenGL::FocusObject() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-m_center.x, -m_center.y,-m_center.z - m_scale * 1.5);
	m_z_delta = -m_scale *1.5;
}


//==================�Ƿ���������=======================
void KnightOpenGL::AntiAliasing(bool anti){
	if(anti){
		//glEnable(GL_POINT_SMOOTH);
		//glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}
	else{
		//glDisable(GL_POINT_SMOOTH);
		//glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
}


//=================�Ŵ��Сģ��=================================
void KnightOpenGL::ZoomObject(double z_delta)
{	
	//---���ɹ���
	if(m_z_delta+z_delta*m_scale > -m_scale*1.1)
		return ;
	//---���ɹ�С
	if(m_z_delta+z_delta*m_scale < -m_scale * 10)
		return ;
	
	//---�õ�ģ���ӵ����==========
	double modelviewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,modelviewMatrix);

	//---�޸�ģ���ӵ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//---�Ŵ��Сģ��=
	glTranslated(0.0,0.0,z_delta*m_scale);
	//---���ֵ�仯--
	m_z_delta += z_delta*m_scale;
	//---������ǰ��ģ���ӵ����---
	glMultMatrixd(modelviewMatrix);
}


//���±�ʵ��û���õ���
//=======================================================================
void KnightOpenGL::ClearBuffers(bool bColor, bool bDepth,
								bool bStencil)
{

}

void KnightOpenGL::CalculateProjMatrix(float fov, float n, float f)
{

}


void KnightOpenGL::CalculateOrthoMatrix(float n, float f)
{
}


int KnightOpenGL::Render(int staticId)
{
	return 3;
}

int KnightOpenGL::CreateStaticBuffer(VertexType, PrimType, int totalVerts, int totalIndices,
									 int stride, void **data, unsigned int *indices, int *staticId)
{
	return 3;
}

void KnightOpenGL::OneTimeInit()
{

}