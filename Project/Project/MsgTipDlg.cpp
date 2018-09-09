#include "stdafx.h"
#include "MsgTipDlg.h"
#include "Path.h"
#include "Utils.h"

CMsgTipDlg::CMsgTipDlg(void)
{

}

CMsgTipDlg::~CMsgTipDlg(void)
{
}

BOOL CMsgTipDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE);

	return FALSE;
}

BOOL CMsgTipDlg::OnEraseBkgnd(CDCHandle dc)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	return TRUE;
}

void CMsgTipDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_dwTimerId)
	{
		RECT rcWindow;
		GetWindowRect(&rcWindow);

		POINT pt = {0};
		::GetCursorPos(&pt);

		if (!::PtInRect(&rcWindow, pt) && !::PtInRect(&m_rcTrayIcon2, pt))
		{
			KillTimer(m_dwTimerId);
			m_dwTimerId = NULL;
			DestroyWindow();
		}
	}
}

void CMsgTipDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);

	SIZE szRoundCorner = {4,4};
	if (!IsIconic()) 
	{
		if (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)
		{
			RECT rcWindow;
			::GetWindowRect(m_hWnd, &rcWindow);
			::OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);
			rcWindow.right++;
			rcWindow.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWindow.left, 
				rcWindow.top, rcWindow.right, rcWindow.bottom, 
				szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(m_hWnd, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
	}
}

void CMsgTipDlg::OnClose()
{
	DestroyWindow();
}

void CMsgTipDlg::OnDestroy()
{
	SetMsgHandled(FALSE);

	UnInitCtrls();			// 反初始化控件

	if (m_dwTimerId != NULL)
	{
		KillTimer(m_dwTimerId);
		m_dwTimerId = NULL;
	}
}


// 初始化控件
BOOL CMsgTipDlg::InitCtrls()
{
	

	return TRUE;
}

// 反初始化控件
BOOL CMsgTipDlg::UnInitCtrls()
{
	if (m_ListCtrl.IsWindow())
		m_ListCtrl.DestroyWindow();

	if (m_lnkCancelFlash.IsWindow())
		m_lnkCancelFlash.DestroyWindow();

	if (m_lnkShowAll.IsWindow())
		m_lnkShowAll.DestroyWindow();

	return TRUE;
}

// 自动调整对话框大小
void CMsgTipDlg::SetDlgAutoSize()
{

}

// 自动调整控件大小
void CMsgTipDlg::SetCtrlsAutoSize()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nMsgSenderCnt = 0;


	int cxListCtrl = 204;
	int cyListCtrl = nMsgSenderCnt* m_nListItemHeight + 5;

	if (m_ListCtrl.IsWindow())
		m_ListCtrl.MoveWindow(4, 36, cxListCtrl, cyListCtrl);
	if (m_lnkCancelFlash.IsWindow())
		m_lnkCancelFlash.MoveWindow(12, rcClient.bottom - 22, 48, 14);
	if (m_lnkShowAll.IsWindow())
		m_lnkShowAll.MoveWindow(156, rcClient.bottom - 22, 48, 14);
}
