// KnightZbufferDoc.h : CKnightZbufferDoc ��Ľӿ�
//


#pragma once

#include "KnightMesh/KnightMesh.h"

class CKnightZbufferDoc : public CDocument
{
protected: // �������л�����
	CKnightZbufferDoc();
	DECLARE_DYNCREATE(CKnightZbufferDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CKnightZbufferDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()



public:
	//=====����ģ��=====
	bool LoadMesh(const char * file_path,EngineMain *rmain);
	bool IsMeshValid(){return m_mesh->IsValid();}
public:
	//=====ģ��=====
	KnightMesh *m_mesh;
};


