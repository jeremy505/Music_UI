#include "stdafx.h"
#include "LeftPanListCtrl.h"
//#include "GDIFactory.h"
#include <Gdiplus.h>

CLeftPanListCtrl::CLeftPanListCtrl(void)
{
	m_bTransparent = FALSE;
	m_hBgDC = NULL;
	m_clrBg = RGB(255, 255, 255);
	m_rcMargin = CRect(0, 0, 0, 0);
}

CLeftPanListCtrl::~CLeftPanListCtrl(void)
{

}

int CLeftPanListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

BOOL CLeftPanListCtrl::OnEraseBkgnd(CDCHandle dc)
{
	return TRUE;
}

void CLeftPanListCtrl::OnSize(UINT nType, CSize size)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	m_rcMargin = rcClient;;
}

void CLeftPanListCtrl::SetTransparent(BOOL bTransparent, HDC hBgDC)
{
	m_bTransparent = bTransparent;
	m_hBgDC = hBgDC;
}

void CLeftPanListCtrl::OnPaint(CDCHandle dc)
{

	CRect rcClient;
	GetClientRect(&rcClient);

	CPaintDC PaintDC(m_hWnd);

	CMemoryDC MemDC(PaintDC.m_hDC, rcClient);

	if (m_bTransparent)
		DrawParentWndBg(MemDC.m_hDC);


	if (m_lpBgImg != NULL && !m_lpBgImg->IsNull())
	{
		m_lpBgImg->Draw2(MemDC.m_hDC, m_rcMargin);
	}
}

BOOL CLeftPanListCtrl::SetBgPic(LPCTSTR lpszFileName)
{
	CSkinManager::GetInstance()->ReleaseImage(m_lpBgImg);
	m_lpBgImg = CSkinManager::GetInstance()->GetImage(lpszFileName);
	return (m_lpBgImg != NULL) ? TRUE : FALSE;
}


void CLeftPanListCtrl::SetMargin(const RECT* lpMargin)
{
	if (lpMargin != NULL)
		m_rcMargin = *lpMargin;
}


void CLeftPanListCtrl::DrawParentWndBg(HDC hDC)
{
	HWND hParentWnd = ::GetParent(m_hWnd);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	::ScreenToClient(hParentWnd, (LPPOINT)&rcWindow);
	::ScreenToClient(hParentWnd, ((LPPOINT)&rcWindow) + 1);

	::BitBlt(hDC, 0, 0, rcWindow.Width(), rcWindow.Height(), m_hBgDC, rcWindow.left, rcWindow.top, SRCCOPY);
}

void CLeftPanListCtrl::OnDestroy()
{

}