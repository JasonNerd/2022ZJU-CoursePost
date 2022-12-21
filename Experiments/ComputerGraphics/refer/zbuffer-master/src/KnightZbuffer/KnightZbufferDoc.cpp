// KnightZbufferDoc.cpp : CKnightZbufferDoc ���ʵ��
//

#include "stdafx.h"
#include "KnightZbuffer.h"

#include "KnightZbufferDoc.h"

//---
#include "KnightEngine/KnightOpenGL.h"
//---

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKnightZbufferDoc

IMPLEMENT_DYNCREATE(CKnightZbufferDoc, CDocument)

BEGIN_MESSAGE_MAP(CKnightZbufferDoc, CDocument)
END_MESSAGE_MAP()


// CKnightZbufferDoc ����/����

CKnightZbufferDoc::CKnightZbufferDoc()
{
	// TODO: �ڴ����һ���Թ������
	//-----��Ϊm_mesh����ȫ�ֱ��� ���Ա����ʹ��  m_loadobj���࣬�й��캯���������������ʹ��
	m_mesh = new KnightMesh;
}

CKnightZbufferDoc::~CKnightZbufferDoc()
{
	delete m_mesh;	// ɾ������,����ڴ�----
	m_mesh = NULL;
}

BOOL CKnightZbufferDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CKnightZbufferDoc ���л�

void CKnightZbufferDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

// CKnightZbufferDoc ���

#ifdef _DEBUG
void CKnightZbufferDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKnightZbufferDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKnightZbufferDoc ����


//=====����ģ��=====
bool CKnightZbufferDoc::LoadMesh(const char * file_path,EngineMain *rmain)
{
//	t.print("trace in CKnightZbufferDoc::LoadMesh\n");

	if(!m_mesh->LoadMesh(file_path)){
		return false;
	}
	//---rmianΪ������ƣ���ģ�͵Ĵ�С��������Ϣ��¼��������-----
	rmain->RegisterObjectInfo(m_mesh->getScale(),m_mesh->getCenter());
	//---���������ģ�ͷ����ӿ���-----
	rmain->FocusObject();

	return true;
}
