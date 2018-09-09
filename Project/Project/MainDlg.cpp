// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "Path.h"
#include "LeftPanListCtrl.h"
#pragma comment(lib, "winmm.lib")

#define CHAT_BG_IMAGE_NAME			_T("BuddyChatDlgBg.png")
#define CHAT_EXPAND_BG_IMAGE_NAME   _T("TitleBar\\MainPan.png")

//主菜单各个子菜单索引号
enum MAIN_PANEL_MEMU
{
	MAIN_PANEL_TRAYICON_INDEX,
	MAIN_PANEL_LEFTPAN_INDEX,
};

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

CMainDlg::CMainDlg(void)
{
	m_hAppIcon = NULL;

}

CMainDlg::~CMainDlg(void)
{
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	InitUI();

	LoadAppIcon(STATUS_ONLINE);
	m_TrayIcon.AddIcon(m_hWnd, WM_TRAYICON_NOTIFY, 1, m_hAppIcon, _T("网易云开始登录..."));


	// 成员初始化
	m_iPlaying = -1;
	m_iCount = 0;
	m_iVolume = 1000;
	m_nTick = 0;
	m_nAudioLen = 0;
	m_iPlayMode = 0;
	m_btnPlayPause = GetDlgItem(IDC_BTN_PLAY_PAUSE);
	m_btnNext = GetDlgItem(IDC_BTN_NEXT);
	m_btnBack = GetDlgItem(IDC_BTN_BACK);
	//m_btnStop = GetDlgItem(IDC_BTN_STOP);
	m_pBtnOpen = GetDlgItem(IDC_BTN_OPEN);
	// ListView
	m_listMusic = GetDlgItem(IDC_LIST_MUSIC);
	m_listMusic.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	LVCOLUMN lvc;
	lvc.mask = LVCF_WIDTH;
	lvc.cx = 264;
	m_listMusic.InsertColumn(0, &lvc);
	lvc.cx = 46;
	m_listMusic.InsertColumn(1, &lvc);
	m_progress = GetDlgItem(IDC_PROG_MUSIC);// Progress
	m_staticMode = GetDlgItem(IDC_STATIC_MODE);// Static
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


BOOL CMainDlg::InitUI()
{
	m_SkinDlg.SetTitleBarBgPic(_T("TitleBar\\TitleBar.png"));
	m_SkinDlg.SetBgColor(RGB(255, 255, 255));
	m_SkinDlg.SetMinSysBtnPic(_T("SysBtn\\btn_mini_normal.png"), _T("SysBtn\\btn_mini_highlight.png"), _T("SysBtn\\btn_mini_down.png"));;
	m_SkinDlg.SetMaxSysBtnPic(_T("SysBtn\\btn_max_normal.png"), _T("SysBtn\\btn_max_highlight.png"), _T("SysBtn\\btn_max_down.png"));
	m_SkinDlg.SetRestoreSysBtnPic(_T("SysBtn\\btn_restore_normal.png"), _T("SysBtn\\btn_restore_highlight.png"), _T("SysBtn\\btn_restore_down.png"));
	m_SkinDlg.SetCloseSysBtnPic(_T("SysBtn\\btn_close_normal.png"), _T("SysBtn\\btn_close_highlight.png"), _T("SysBtn\\btn_close_down.png"));
	
	m_SkinDlg.SubclassWindow(m_hWnd);


	m_SkinDlg.SetLeftPanPic(_T("TitleBar\\leftpan.png"));
	m_SkinDlg.SetPlayPanPic(_T("TitleBar\\playpan.png"));
	m_SkinDlg.SetTitleText(_T("网易云音乐"));
	
	m_SkinMenu.SetBgPic(_T("Menu\\menu_left_bg.png"), _T("Menu\\menu_right_bg.png"));
	m_SkinMenu.SetSelectedPic(_T("Menu\\menu_selected.png"));
	m_SkinMenu.SetSepartorPic(_T("Menu\\menu_separtor.png"));
	m_SkinMenu.SetTextColor(RGB(0, 20, 35));
	m_SkinMenu.SetSelTextColor(RGB(254, 254, 254));
	m_SkinMenu.LoadMenu(ID_MENU_MAIN_PANEL);


	DWORD dwMenuIDs[] = { ID_MENU_STOP, ID_MENU_PREV, ID_MENU_NEXT,
		ID_MENU_MINISIZE, ID_MENU_SETTING, ID_MENU_EXIT};
	CString strFileNames[] = { _T("Menu\\stop.png"), _T("Menu\\prev.png"),
		_T("Menu\\next.png"), _T("Menu\\minisize.png"), _T("Menu\\setting.png"),
		_T("Menu\\exit.png") };

	CSkinMenu PopupMenu = m_SkinMenu.GetSubMenu(MAIN_PANEL_TRAYICON_INDEX);
	for (int i = 0; i < 6; i++)
	{
		PopupMenu.SetIcon(dwMenuIDs[i], FALSE, strFileNames[i], strFileNames[i]);
	}


	InitTabCtrl();
	InitLeftListCtrl();

	ModifyStyle(0, WS_CLIPCHILDREN);
	ModifyStyleEx(WS_EX_APPWINDOW, 0);

	return TRUE;
}

void CMainDlg::UninitUI()
{

}

void CMainDlg::OnSysCommand(UINT nID, CPoint pt)
{
	if (nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	SetMsgHandled(FALSE);
}

void CMainDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 670;
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);


	CRect rcClient;
	GetClientRect(&rcClient);
	HDWP hdwp = ::BeginDeferWindowPos(18);

	if (m_TabCtrl.IsWindow())
	{
		int nCount = m_TabCtrl.GetItemCount();
		if (nCount > 0)
		{
			int nWidth = (rcClient.Width() - 2) / nCount;
			int nRemainder = (rcClient.Width() - 2) % nCount;

			for (int i = 0; i < nCount; i++)
			{
				m_TabCtrl.SetItemSize(i, 200, 34 , 0, 0);
			}

			m_TabCtrl.SetItemSize(nCount - 1, 200, 34, 0, 0);
		}

		m_TabCtrl.MoveWindow(0, 50, 200, rcClient.Height() -100);
	}

	if (m_leftpanSkinMenu.IsWindow())
	{
		m_leftpanSkinMenu.MoveWindow(200, 50, rcClient.Width() - 200, rcClient.Height() - 100);
	}

	m_btnBack = GetDlgItem(IDC_BTN_BACK);
	m_btnNext = GetDlgItem(IDC_BTN_NEXT);
	m_btnPlayPause = GetDlgItem(IDC_BTN_PLAY_PAUSE);
	m_progress = GetDlgItem(IDC_PROG_MUSIC);// Progress
	
	
	m_btnBack.MoveWindow(3, rcClient.Height() - 40, 70, 25);
	m_btnPlayPause.MoveWindow(73 + 3, rcClient.Height() - 40, 70, 25);
	m_btnNext.MoveWindow(73 + 3 + 73, rcClient.Height() - 40, 70, 25);

	m_progress.MoveWindow(73 + 3 + 73 + 73 + 100, rcClient.Height() - 30, rcClient.Width() - (3 + 73 + 73 + 73 + 110), 10);
	GetDlgItem(IDC_STATIC_TIME).MoveWindow(73 + 3 + 73 + 73 + 5, rcClient.Height() - 35, 80, 20);
	::EndDeferWindowPos(hdwp);

}

BOOL CMainDlg::LoadAppIcon(long nStatus)
{
	DestroyAppIcon();

	tstring strFileName;
	switch (nStatus)
	{
	case STATUS_ONLINE:
		strFileName = Hootina::CPath::GetAppPath() + _T("Image\\login2.ico");
		break;
	case STATUS_OFFLINE:
		strFileName = Hootina::CPath::GetAppPath() + _T("Image\\login.ico");
		break;
	}
	m_hAppIcon = AtlLoadIconImage(strFileName.c_str(), LR_DEFAULTCOLOR | LR_LOADFROMFILE,
		::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	return m_hAppIcon != NULL ? TRUE : FALSE;
}

void CMainDlg::DestroyAppIcon()
{
	if (m_hAppIcon != NULL)
	{
		::DestroyIcon(m_hAppIcon);
		m_hAppIcon = NULL;
	}
}

LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	m_TrayIcon.OnTimer(uMsg);

	LoadAppIcon(STATUS_ONLINE);
	static bool flag = false;
	if (flag == false)
	{
		CString strIconInfo(_T("网易云音乐已启动!"));
		m_TrayIcon.ModifyIcon(m_hAppIcon, strIconInfo, 1, TRUE, _T("温馨提示"), _T("您已经启动网易云......"), 1000);
		flag = true;
	}

	if (IDT_TIMER_PROGRESS == wParam)
	{
		++m_nTick;

		if (m_nTick > m_nAudioLen)
		{
			KillTimer(IDT_TIMER_PROGRESS);
			m_progress.SetPos(m_nTick);
			m_nTick = 0;
			::PostMessage(m_hWnd, WM_MUSIC_END, 0, 0);
		}
		else
		{
			m_progress.SetPos(m_nTick);
		}

		DrawProgressStatus();
	}

	return 0;

}

LRESULT CMainDlg::OnTrayIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_TrayIcon.OnTrayIconNotify(wParam, lParam);


	UINT uID = (UINT)wParam;
	UINT uIconMsg = (UINT)lParam;

	switch (uIconMsg)
	{
	case WM_LBUTTONUP:
		OnTrayIcon_LButtunUp();
		break;
	case WM_RBUTTONUP:
		OnTrayIcon_RButtunUp();
		break;
	case WM_MOUSEHOVER:
		OnTrayIcon_MouseHover();
		break;
	case WM_MOUSELEAVE:
		OnTrayIcon_MouseLeave();
		break;
	}
	return 0;
}

void CMainDlg::OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
{
	switch (nHotKeyID)
	{
	case 1001:
	{
				 if (::IsWindowVisible(m_hWnd))
					 ShowWindow(SW_HIDE);
				 else
					 OnTrayIconNotify(WM_TRAYICON_NOTIFY, NULL, WM_LBUTTONUP);
	}
		break;
	}
}

void CMainDlg::OnTrayIcon_LButtunUp()
{

	if (::IsWindowVisible(m_hWnd))
		ShowWindow(SW_HIDE);
	else if (IsWindow())
	{
		ShowWindow(SW_SHOW);
		SetFocus();
	}
}

void CMainDlg::OnTrayIcon_RButtunUp()
{
	POINT pt;
	int nPos = MAIN_PANEL_TRAYICON_INDEX;	
	GetCursorPos(&pt);

	::SetForegroundWindow(m_hWnd);

	CSkinMenu PopupMenu = m_SkinMenu.GetSubMenu(nPos);

	PopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);

	
}

void CMainDlg::OnTrayIcon_MouseHover()
{

}

void CMainDlg::OnTrayIcon_MouseLeave()
{

}


void CMainDlg::OnMenu_ShowMainPanel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IsWindow())
	{
		ShowWindow(SW_SHOW);
		SetFocus();
	}
}


void CMainDlg::OnMenu_Exit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IsWindow())
	{
		CloseDialog(IDOK);
	}
}

void CMainDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	m_SkinMenu.OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	m_SkinMenu.OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMainDlg::OnMenu_MINISIZE(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (::IsWindowVisible(m_hWnd))
		ShowWindow(SW_HIDE);
}


BOOL CMainDlg::InitTabCtrl()
{


	int nIndex = m_TabCtrl.AddItem(301, STCI_STYLE_BUTTON);
	m_TabCtrl.SetItemText(nIndex, _T("推荐"));

	nIndex = m_TabCtrl.AddItem(302, STCI_STYLE_BUTTON);
	m_TabCtrl.SetItemText(nIndex, _T("我的音乐"));

	nIndex = m_TabCtrl.AddItem(303, STCI_STYLE_BUTTON);
	m_TabCtrl.SetItemText(nIndex, _T("创建的歌单"));

	HDC hDlgBgDC = m_SkinDlg.GetBgDC();
	m_TabCtrl.SetBgPic(_T("MainTabCtrl\\main_tab_bkg.png"), CRect(5, 1, 5, 1));
	m_TabCtrl.SetItemsBgPic(NULL, _T("MainTabCtrl\\main_tab_highlight.png"), _T("MainTabCtrl\\main_tab_check.png"), CRect(5, 1, 5, 1));
	m_TabCtrl.SetItemsArrowPic(_T("MainTabCtrl\\main_tabbtn_highlight.png"), _T("MainTabCtrl\\main_tabbtn_down.png"));

	m_TabCtrl.SetTransparent(TRUE, hDlgBgDC);
	CRect rcTabCtrl(0, 50, 200, 50 + 32);
	m_TabCtrl.Create(m_hWnd, rcTabCtrl, NULL, WS_CHILD | WS_VISIBLE, NULL, ID_TABCTRL_MAIN, NULL);

	m_TabCtrl.SetCurSel(1);

	m_TabCtrl.ShowWindow(SW_SHOW);
	return TRUE;
}


BOOL CMainDlg::InitLeftListCtrl()
{
	m_leftpanSkinMenu.SetMargin(CRect(0, 0, 0, 0));
	m_leftpanSkinMenu.SetBgPic(_T("main_panel_bg2.png"));


	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcListCtrl(200, 50, rcClient.Width(), rcClient.Height() - 50);
	m_leftpanSkinMenu.Create(m_hWnd, rcListCtrl, NULL, WS_CHILD, NULL, ID_LEFTPAN_LISTCTRL);
	return true;
}

LRESULT CMainDlg::OnTabCtrlDropDown(LPNMHDR pnmh)
{
	return 1;
}

LRESULT CMainDlg::OnTabCtrlSelChange(LPNMHDR pnmh)
{
	int nCurSel = m_TabCtrl.GetCurSel();
	switch (nCurSel)
	{
	case 0:
		GetDlgItem(IDC_BTN_OPEN).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_MUSIC).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MODE).ShowWindow(SW_HIDE);
		m_leftpanSkinMenu.ShowWindow(SW_SHOW);
		m_leftpanSkinMenu.SetFocus();
		break;

	case 1:
		GetDlgItem(IDC_BTN_OPEN).ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_MUSIC).ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MODE).ShowWindow(SW_SHOW);
		m_leftpanSkinMenu.ShowWindow(SW_HIDE);
		break;

	case 2:
		GetDlgItem(IDC_BTN_OPEN).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_MUSIC).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MODE).ShowWindow(SW_HIDE);;
		m_leftpanSkinMenu.ShowWindow(SW_HIDE);
		break;
	}
	return 0;
}


LRESULT CMainDlg::OnLeftPanListDblClk(LPNMHDR pnmh)
{
	return 0;
}



//打开文件
LRESULT CMainDlg::OnBnClickedBtnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR lpFilter = L"音频文件 (*.mp3; *.wav)\0*.mp3;*.wav\0\0";
	CMultiFileDialog diag(NULL, NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, lpFilter, NULL);

	if (diag.DoModal() == IDCANCEL)
	{
		return 0;
	}

	TCHAR pszFilePath[260] = { 0 };
	UINT nBytes = diag.GetFirstPathName(pszFilePath, 260);
	// 遍历音频文件
	while (nBytes)
	{
		std::wstring strPath = pszFilePath;
		DWORD dwLen = m_player.GetAudioLength(pszFilePath); // 毫秒
		// 打开音频成功，保存信息
		if (dwLen)
		{
			std::wstring strTemp;
			m_vListPath.push_back(strPath); // 保存文件绝对路径
			GetFileTitleByFullPath(strPath, strTemp); // 取文件title
			m_listMusic.InsertItem(m_iCount, strTemp.c_str()); // 根据索引插入文件title

			GetFormatedTime(dwLen / 1000, strTemp);
			m_vTime.push_back(strTemp); // 保存时间
			m_listMusic.SetItemText(m_iCount, 1, strTemp.c_str()); // 根据索引插入播放时间

			++m_iCount;
		}

		nBytes = diag.GetNextPathName(pszFilePath, 260);
	}
	// 第一次打开文件 重置控件状态
	if (m_listMusic.GetSelectionMark() == -1)
	{
		m_listMusic.SetSelectionMark(0);
		m_listMusic.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_btnPlayPause.EnableWindow();
		m_btnBack.EnableWindow();
		m_btnNext.EnableWindow();
		m_btnBack.EnableWindow();
	}

	return 0;
}

BOOL CMainDlg::GetFileTitleByFullPath(const std::wstring& strFullPath, std::wstring& strTitle)
{
	BOOL b = FALSE;
	int iRight = strFullPath.rfind(L".");
	int iLeft = strFullPath.rfind(L"\\");

	if (iRight > iLeft && iRight != -1 && iLeft != -1)
	{
		b = TRUE;
		++iLeft;
		strTitle = strFullPath.substr(iLeft, iRight - iLeft);
	}

	return b;
}


/*--------------------------------- 辅助函数 -------------------------------*/

void CMainDlg::GetFormatedTime(DWORD dwTotalSeconds, std::wstring& strFormated)
{
	strFormated.clear();
	DWORD dwMinute = dwTotalSeconds / 60;
	DWORD dwSecond = dwTotalSeconds - dwMinute * 60;

	if (dwMinute >= 59 || dwSecond >= 59)
	{
		strFormated = L"00:00";
	}
	else
	{
		TCHAR chTime[8];
		DWORD dwMinDecade = 0;
		DWORD dwSecDecade = 0;
		if (dwMinute > 9)
		{
			dwMinDecade = dwMinute / 10;
		}

		if (dwSecond > 9)
		{
			dwSecDecade = dwSecond / 10;
		}

		wsprintf(chTime, L"%d%d:%d%d", dwMinDecade, dwMinute - dwMinDecade * 10, dwSecDecade, dwSecond - dwSecDecade * 10);
		strFormated = chTime;
	}
}

// 下一曲
LRESULT CMainDlg::OnBnClickedBtnNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_iPlaying != -1)
	{
		m_player.Stop();
		// 如果是最后一首就跳到第一首
		if (m_listMusic.GetSelectionMark() + 1 == m_iCount)
		{
			m_listMusic.SetSelectionMark(0);
			m_listMusic.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		}
		// 否则下一首
		else
		{
			int index = m_listMusic.GetSelectionMark();
			++index;
			m_listMusic.SetSelectionMark(index);
			m_listMusic.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
		}

		m_player.Play(GetSelectedMusicPath(), &m_nAudioLen);
		m_nAudioLen /= 1000;
		m_iPlaying = TRUE;
		SetWindowTextMusic();
		ResetProgressBar();
	}

	return 0;
}

LPCTSTR CMainDlg::GetSelectedMusicPath()
{
	LPCTSTR pszPath = NULL;
	int i = m_listMusic.GetSelectionMark();

	if (i != -1 && m_listMusic.GetItemCount() != 0)
	{
		pszPath = m_vListPath[i].c_str();
	}

	return pszPath;
}

// 播放 暂停
LRESULT CMainDlg::OnBnClickedBtnPlayPause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (TRUE == m_iPlaying)
	{
		m_btnPlayPause.SetWindowText(L"播放");
		m_player.Pause();
		m_iPlaying = FALSE;
		SuspendProgressBar();
	}
	else if (FALSE == m_iPlaying)
	{
		m_btnPlayPause.SetWindowText(L"暂停");
		m_player.Resume();
		m_iPlaying = TRUE;
		ResumeProgressBar();

	}
	// 无效标记 重新播放文件
	else if (-1 == m_iPlaying)
	{
		LPCTSTR pszFile = GetSelectedMusicPath();

		if (pszFile != 0)
		{
			m_btnPlayPause.SetWindowText(L"暂停");
			m_player.Play(pszFile, &m_nAudioLen);
			m_nAudioLen /= 1000;
			m_iPlaying = TRUE;
			SetWindowTextMusic();
			ResetProgressBar();
		}
	}

	//m_btnStop.EnableWindow(TRUE);
	SetWindowTextMusic();

	return 0;
}

// 上一曲
LRESULT CMainDlg::OnBnClickedBtnBack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_iPlaying != -1)
	{
		m_player.Stop();
		int index = m_listMusic.GetSelectionMark();
		// 如果是第一首就跳到最后一首
		if (index == 0)
		{
			m_listMusic.SetSelectionMark(m_iCount - 1);
			m_listMusic.SetItemState(m_iCount - 1, LVIS_SELECTED, LVIS_SELECTED);
		}
		else
		{
			--index;
			m_listMusic.SetSelectionMark(index);
			m_listMusic.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
		}

		m_player.Play(GetSelectedMusicPath(), &m_nAudioLen);
		m_nAudioLen /= 1000;
		m_iPlaying = TRUE;
		SetWindowTextMusic();
		ResetProgressBar();
	}
	return 0;
}

// 停止
LRESULT CMainDlg::OnBnClickedBtnStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//m_btnStop.EnableWindow(FALSE);
	if (TRUE == m_iPlaying)
	{
		//m_btnPlayPause.SetWindowText(L"播放");
		//m_player.Stop();
		m_btnPlayPause.SetWindowText(L"播放");
		m_player.Pause();
		m_iPlaying = FALSE;
		SuspendProgressBar();
	}

	//m_iPlaying = -1; // 设置播放无效标记
	//SetWindowTextMusic();

	return 0;
}

void CMainDlg::SetWindowTextMusic()
{
	int i = m_listMusic.GetSelectionMark();

	if (i != -1)
	{
		if (-1 == m_iPlaying)
		{
			::SetWindowText(m_hWnd, L"- Stop -");
		}
		else
		{
			TCHAR chTitle[260] = { 0 };
			m_listMusic.GetItemText(i, 0, chTitle, 260);
			std::wstring strTemp = chTitle;

			if (TRUE == m_iPlaying)
			{
				strTemp += L" - Playing";
			}
			else if (FALSE == m_iPlaying)
			{
				strTemp += L" - Paused";
			}
			::SetWindowText(m_hWnd, strTemp.c_str());
		}
	}
	else
	{
		::SetWindowText(m_hWnd, L"AudioPlayer");
	}
}

/*--------------------------------- 进度条 -------------------------------*/
void CMainDlg::ResetProgressBar()
{
	m_progress.SetRange32(0, m_nAudioLen + 1);  // 补贴除法丢失的毫秒精度
	m_progress.SetPos(0);
	m_nTick = 0;
	ResumeProgressBar();
}

void CMainDlg::SuspendProgressBar()
{
	KillTimer(IDT_TIMER_PROGRESS);
}

void CMainDlg::ResumeProgressBar()
{
	SetTimer(IDT_TIMER_PROGRESS, 1000);
}

void CMainDlg::DrawProgressStatus()
{
	int nIndex = m_listMusic.GetSelectionMark();

	if (nIndex != -1)
	{
		std::wstring strTimeRate;
		GetFormatedTime(m_nTick, strTimeRate);
		strTimeRate += L"/";
		strTimeRate += m_vTime[nIndex];
		GetDlgItem(IDC_STATIC_TIME).SetWindowText(strTimeRate.c_str());
	}
}