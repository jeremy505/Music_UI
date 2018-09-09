#pragma once
#include "SkinLib/SkinLib.h"


class CLeftPanListCtrl : public CWindowImpl<CLeftPanListCtrl, CWindow>
{
public:
	CLeftPanListCtrl(void);
	~CLeftPanListCtrl(void);

	BEGIN_MSG_MAP_EX(CLeftPanListCtrl)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()


public:
	BOOL SubclassWindow(HWND hWnd);
	BOOL SetBgPic(LPCTSTR lpszFileName);
	void SetMargin(const RECT* lpMargin);
	void SetTransparent(BOOL bTransparent, HDC hBgDC);

private:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	void OnPaint(CDCHandle dc);
	void OnSize(UINT nType, CSize size);
	void DrawParentWndBg(HDC hDC);
	void OnDestroy();
private:
	COLORREF	m_clrBg;
	CImageEx*				m_lpBgImg;
	CRect					m_rcMargin;
	CSkinScrollBar			m_VScrollBar;
	BOOL					m_bTransparent;
	HDC						m_hBgDC;

	HDC			m_hMemDC;
	HBITMAP		m_hMemBmp, m_hOldBmp;
};