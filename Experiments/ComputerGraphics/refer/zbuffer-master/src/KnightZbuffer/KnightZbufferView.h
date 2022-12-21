// KnightZbufferView.h : CKnightZbufferView ��Ľӿ�
//


#pragma once

#include "KnightEngine/EngineMain.h"

class CKnightZbufferView : public CView
{
protected: // �������л�����
	CKnightZbufferView();
	DECLARE_DYNCREATE(CKnightZbufferView)

// ����
public:
	CKnightZbufferDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CKnightZbufferView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnFileOpen();
	afx_msg void onRenderSoft();
	afx_msg void OnRenderHardDirectX();
	afx_msg void OnRenderHardOpenGL();
	afx_msg void OnantialiasingOn();
	afx_msg void OnantialiasingOff();
	afx_msg void OnRotateAutomatic();
	afx_msg void OnRotateManual();
	afx_msg void OnUpdateRenderSoft(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRenderHardDirectX(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRenderHardOpenGL(CCmdUI *pCmdUI);
	afx_msg void OnUpdateantialiasingOn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateantialiasingOff(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotateAutomatic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotateManual(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	//---һЩö�ٱ���--------------
	enum {renderSoft,renderHardDirectX,renderHardOpenGL,antialiasingOn,antialiasingOff,rotateAutomatic,
		rotateManual,rotateManualAuto};
	CClientDC* m_dcptr;//---��Ⱦ����---
	UINT_PTR m_draw_timer,m_statistic_timer;//---ͳ��FPS---
	EngineMain *m_viewengine;//---�������
	int m_render_mode;	// ��Ⱦ��ʽ(�����Ӳ��)
	int m_rotate_mode;	// ��ת����(�Զ�����Ϊ���Զ�+��Ϊ)
	int m_anti_mode;	// �Ƿ�����
	int m_framecount;	// ֡ͳ�� 
	double m_dx,m_dy;	// ��ת����������

	//===ʱ�亯��====
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	// �����޸ģ��������˸
	afx_msg void OnAutoManual();
	afx_msg void OnUpdateAutoManual(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // KnightZbufferView.cpp �еĵ��԰汾
inline CKnightZbufferDoc* CKnightZbufferView::GetDocument() const
   { return reinterpret_cast<CKnightZbufferDoc*>(m_pDocument); }
#endif

