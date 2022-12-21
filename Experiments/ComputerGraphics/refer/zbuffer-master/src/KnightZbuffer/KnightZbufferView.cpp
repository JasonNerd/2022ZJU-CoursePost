// KnightZbufferView.cpp : CKnightZbufferView ���ʵ��
//

#include "stdafx.h"
#include "KnightZbuffer.h"

#include "KnightZbufferDoc.h"
#include "KnightZbufferView.h"

#include "KnightGlobal\KnightGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKnightZbufferView

IMPLEMENT_DYNCREATE(CKnightZbufferView, CView)

BEGIN_MESSAGE_MAP(CKnightZbufferView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CKnightZbufferView::OnFileOpen)
	ON_COMMAND(ID_32773, &CKnightZbufferView::onRenderSoft)
	ON_COMMAND(ID_32775, &CKnightZbufferView::OnRenderHardDirectX)
	ON_COMMAND(ID_32774, &CKnightZbufferView::OnRenderHardOpenGL)
	ON_COMMAND(ID_32777, &CKnightZbufferView::OnantialiasingOn)
	ON_COMMAND(ID_32782, &CKnightZbufferView::OnantialiasingOff)
	ON_COMMAND(ID_32780, &CKnightZbufferView::OnRotateAutomatic)
	ON_COMMAND(ID_32781, &CKnightZbufferView::OnRotateManual)
	ON_UPDATE_COMMAND_UI(ID_32773, &CKnightZbufferView::OnUpdateRenderSoft)
	ON_UPDATE_COMMAND_UI(ID_32775, &CKnightZbufferView::OnUpdateRenderHardDirectX)
	ON_UPDATE_COMMAND_UI(ID_32774, &CKnightZbufferView::OnUpdateRenderHardOpenGL)
	ON_UPDATE_COMMAND_UI(ID_32777, &CKnightZbufferView::OnUpdateantialiasingOn)
	ON_UPDATE_COMMAND_UI(ID_32782, &CKnightZbufferView::OnUpdateantialiasingOff)
	ON_UPDATE_COMMAND_UI(ID_32780, &CKnightZbufferView::OnUpdateRotateAutomatic)
	ON_UPDATE_COMMAND_UI(ID_32781, &CKnightZbufferView::OnUpdateRotateManual)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_32783, &CKnightZbufferView::OnAutoManual)
	ON_UPDATE_COMMAND_UI(ID_32783, &CKnightZbufferView::OnUpdateAutoManual)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CKnightZbufferView ����/����


CKnightZbufferView::CKnightZbufferView()
{
	// TODO: �ڴ˴���ӹ������
	m_viewengine = new EngineMain;	// ��������
	m_render_mode = renderSoft;		// �������
	m_rotate_mode = rotateManualAuto;	// �Զ�+�˹�
	m_anti_mode = antialiasingOff;	// ����������
	m_framecount = -1;		// ֡����
	m_dcptr = NULL;			//
}


CKnightZbufferView::~CKnightZbufferView()
{
	if(m_viewengine){
		delete m_viewengine;	// ɾ������
		m_viewengine = NULL;
	}
	if(m_dcptr){
		delete m_dcptr;
		m_dcptr = NULL;
	}
}


BOOL CKnightZbufferView::PreCreateWindow(CREATESTRUCT& cs)
{
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CView::PreCreateWindow(cs);
}


// CKnightZbufferView ����========================================================================================
void CKnightZbufferView::OnDraw(CDC* /*pDC*/)
{
	CKnightZbufferDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_viewengine->setColor(160.0f/255.0f,160.0f/255.0f,160.0f/255.0f);	// ���ñ�����ɫ 
	pDoc->m_mesh->Draw(m_viewengine,m_render_mode);	// ��ʼ����
	SwapBuffers(wglGetCurrentDC());	// ��������

	if(m_rotate_mode==rotateManual){	// �ֶ�ҲҪ��ʾ��״̬��
		char info[256];
		sprintf_s(info,"vertexes: %d , faces: %d .  KnightLiao@gmail.com  http://knightliao.blogspot.com ",
			GetDocument()->m_mesh->getVertexNum(),
			GetDocument()->m_mesh->getFaceNum());
		CStatusBar* barPtr =
			(CStatusBar*)(AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR));
		if(barPtr)
			barPtr->SetPaneText(0,info);
	}
}
//======================================================================================================================


// CKnightZbufferView ���

#ifdef _DEBUG
void CKnightZbufferView::AssertValid() const
{
	CView::AssertValid();
}

void CKnightZbufferView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKnightZbufferDoc* CKnightZbufferView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKnightZbufferDoc)));
	return (CKnightZbufferDoc*)m_pDocument;
}
#endif //_DEBUG


// CKnightZbufferView ��Ϣ�������
//------------- �� OBJ �ļ� ���� ------------------------------------------------------------------
void CKnightZbufferView::OnFileOpen()
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t.print("trace in  CKnightZbufferView::OnFileOpen()\n");
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	//---֧���ļ�����---------
	static const char kFileTypes[] = "OBJ files (*.obj)|*.obj|All Files (*.*)|*.*||";
	//---���ļ�����---------
	static CFileDialog FOpenDlg(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST|OFN_ENABLESIZING, (LPCTSTR) kFileTypes);
	if (FOpenDlg.DoModal() == IDOK) {
		if (GetDocument()->LoadMesh((FOpenDlg.GetPathName()),m_viewengine)) {
			//MessageBox(_T("load .obj file successful!"),_T( "success"));
		}
		else{
			MessageBox(_T("Failed to load .obj file!"), _T("Error"));
		}
	}

	//---���»��ƻ���---
	RedrawWindow();
}


//=====�����������==================
void CKnightZbufferView::onRenderSoft()
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t_stream.on_stream();
//	t_stream << " in onRenderSoft() " << std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	m_render_mode = renderSoft;
	RedrawWindow();
}

//=====����DirectX������=================
void CKnightZbufferView::OnRenderHardDirectX()
{
//	m_render_mode = renderHardDirectX;

	MessageBox(_T("��û�����DirectX�������..�����ڴ�.."), _T(":)"));

	RedrawWindow();
}


//=====����OpenGL������=============
void CKnightZbufferView::OnRenderHardOpenGL()
{
	m_render_mode = renderHardOpenGL;
	RedrawWindow();
}


void CKnightZbufferView::OnantialiasingOn()
{
	m_anti_mode = antialiasingOn;
	m_viewengine->AntiAliasing(true);	// �򿪷�����
	RedrawWindow();
}


void CKnightZbufferView::OnantialiasingOff()
{
	m_anti_mode = antialiasingOff;
	m_viewengine->AntiAliasing(false);	// �رշ�����
	RedrawWindow();
}


void CKnightZbufferView::OnAutoManual()
{
	OnRotateAutomatic();	// �Զ�
	m_rotate_mode = rotateManualAuto;	// ����ģʽ
	RedrawWindow();
}


void CKnightZbufferView::OnRotateAutomatic()
{
//	t.print("trace in CKnightZbufferView::OnRotateAutomatic\n");

	//---10�������һ��---
	m_draw_timer = SetTimer(1,10,NULL);	
	//---1000����(1��)����FPS������״̬��
	m_statistic_timer= SetTimer(2,1000,NULL);
	m_rotate_mode=rotateAutomatic;

	m_dx = (rand()-rand())*1e-5;
	m_dy = (rand()-rand())*1e-5;
	
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t_stream << "M_DX: " << m_dx << " M_DY: " << m_dy << std::endl;
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

	m_framecount = -1;
	RedrawWindow();
}


void CKnightZbufferView::OnRotateManual()
{
//	t.print("trace in CKnightZbufferView::OnRotateManual\n");

	KillTimer(m_draw_timer);
	KillTimer(m_statistic_timer);
	m_rotate_mode = rotateManual;
	RedrawWindow();
}


int CKnightZbufferView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++
//	t.on();
//	t_stream.on_stream();
//	t_scanLine.on_stream();
	//+++++++++++++++++++++++++++++++++++Debug+++++++++++++++++++++++++++++++++++++++++++++

//  t.print("trace in CKnightZbufferView::OnCreate\n");

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//---��ʾλ��---
	m_dcptr = new CClientDC(this);
	//---��ʹ������---
	m_viewengine->InitializeEngine(*m_dcptr);
	//---Ĭ�����Զ���ת+�˹�---
	OnAutoManual();

	return 0;
}


void CKnightZbufferView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

//	CView::OnTimer(nIDEvent);

	//---���û�е���ģ�ͣ���Ҫ��Ӧ----------
	if(!GetDocument()->IsMeshValid())
		return ;
	switch(nIDEvent){
		case 1:	//---���ڲ�ͣ�Ļ���---
			if(m_framecount==-1)
				m_framecount =0;
			++m_framecount;
			m_viewengine->RotateObject(m_dx,m_dy);
			//---���»���---
			RedrawWindow();
			break;
		case 2:	{//---����״̬��---
			static char info[256];
			if(m_framecount>=100){
				m_framecount = 100;
			}
			sprintf_s(info,"vertexes: %d , faces: %d , FPS: %d .  KnightLiao@gmail.com  http://knightliao.blogspot.com ",
				GetDocument()->m_mesh->getVertexNum(),
				GetDocument()->m_mesh->getFaceNum(),
				m_framecount);
			m_framecount = -1;
			CStatusBar* barPtr =
				(CStatusBar*)(AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR));
			if(barPtr)
				barPtr->SetPaneText(0,info);
			break;
		}
		default:
			break;
	}
}


void CKnightZbufferView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	//---�����ӿ�λ��---
	m_viewengine->SetViewport(0,0,cx,cy);
	RedrawWindow();
	// TODO: Add your message handler code here
}


void CKnightZbufferView::OnDestroy()
{
	delete m_dcptr;
	m_dcptr = NULL;
	delete m_viewengine;
	m_viewengine = NULL;

	CView::OnDestroy();
}


void CKnightZbufferView::OnMouseMove(UINT nFlags, CPoint point)
{
	//---�����û�е���ģ�ͣ���Ҫ��Ӧ�ú���-----------
	if(!GetDocument()->IsMeshValid())
		return ;
	//---���û���˹����ܣ���Ҳ��Ҫ��Ӧ�ú���---------
	if(!(m_rotate_mode==rotateManualAuto || m_rotate_mode==rotateManual))
		return ;
	static CPoint last(-1, -1);
	if (nFlags & MK_LBUTTON) {
		if (last.x < 0 || last.y < 0) last = point;
		m_viewengine->RotateObject(point.x - last.x, point.y - last.y);
		last = point;
		RedrawWindow();
	} else {
		last.x = last.y = -1;
	}

	//CView::OnMouseMove(nFlags, point);
}


BOOL CKnightZbufferView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

//	return CView::OnEraseBkgnd(pDC);		// �������һ��Ҫȥ��!!!!!
	return true;
}


BOOL CKnightZbufferView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//	return CView::OnMouseWheel(nFlags, zDelta, pt);
	
	//---���û�е���ģ�ͣ���Ҫ��Ӧ�ú���-------------
	if(!GetDocument()->IsMeshValid())
		return false;
	m_viewengine->ZoomObject(zDelta);
	RedrawWindow();
	return true;
}


//-------------------------------------------------------------------

void CKnightZbufferView::OnUpdateRenderSoft(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_render_mode==renderSoft);
}

void CKnightZbufferView::OnUpdateRenderHardDirectX(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_render_mode==renderHardDirectX);
}

void CKnightZbufferView::OnUpdateRenderHardOpenGL(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_render_mode==renderHardOpenGL);
}

void CKnightZbufferView::OnUpdateantialiasingOn(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_anti_mode==antialiasingOn);
}

void CKnightZbufferView::OnUpdateantialiasingOff(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_anti_mode==antialiasingOff);
}

void CKnightZbufferView::OnUpdateRotateAutomatic(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_rotate_mode==rotateAutomatic);
}

void CKnightZbufferView::OnUpdateRotateManual(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_rotate_mode==rotateManual);
}

void CKnightZbufferView::OnUpdateAutoManual(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_rotate_mode==rotateManualAuto);
}

//-------------------------------------------------------------
// CKnightZbufferView ��ӡ

BOOL CKnightZbufferView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CKnightZbufferView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CKnightZbufferView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}



