#pragma once

#include "resource.h"
#include "CustomMsgDef.h"
#include "SkinLib/SkinLib.h"
#include "ImageEx.h"

class CFlamingoClient;

class CMsgTipDlg : public CDialogImpl<CMsgTipDlg>
{
public:
	CMsgTipDlg(void);
	~CMsgTipDlg(void);

	enum { IDD = IDD_MSGTIPDLG };

	BEGIN_MSG_MAP_EX(CMsgTipDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		//MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_SIZE(OnSize)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()

public:


private:
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	BOOL OnEraseBkgnd(CDCHandle dc);
	void OnTimer(UINT_PTR nIDEvent);
	void OnSize(UINT nType, CSize size);
	void OnClose();
	void OnDestroy();
	

	BOOL InitCtrls();			// 初始化控件
	BOOL UnInitCtrls();			// 反初始化控件
	void SetDlgAutoSize();		// 自动调整对话框大小
	void SetCtrlsAutoSize();	// 自动调整控件大小


public:
	HWND			m_hMainDlg;
	CRect			m_rcTrayIcon;
	CRect			m_rcTrayIcon2;		//外部传进来的任务栏图标区域

private:
	CSkinDialog m_SkinDlg;
	CSkinListCtrl m_ListCtrl;
	CSkinHyperLink m_lnkCancelFlash, m_lnkShowAll;
	DWORD m_dwTimerId;
	int m_nListItemHeight;
};
