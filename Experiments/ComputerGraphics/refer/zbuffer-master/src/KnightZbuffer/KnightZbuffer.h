// KnightZbuffer.h : KnightZbuffer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CKnightZbufferApp:
// �йش����ʵ�֣������ KnightZbuffer.cpp
//

class CKnightZbufferApp : public CWinApp
{
public:
	CKnightZbufferApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKnightZbufferApp theApp;