// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CustomMsgDef.h"
#include "SkinLib/SkinLib.h"
#include "TrayIcon.h"
#include "MsgTipDlg.h"
#include "LeftPanListCtrl.h"

#include "MCIPlayer.h"
#include "resource.h"
#define IDT_TIMER_PROGRESS 10
#define PROGRESS_LEFT      270
#define PROGRESS_TOP       178
#define PROGRESS_RIGHT     350
#define PROGRESS_BOTTOM    202
#define RGB_BACKGROUND     RGB(240, 240, 200)
#define ASIGHT                  L"                               BY ASIGHT"
#define PLAYMODE_SEQUENTIAL     L"播放模式 - 顺序播放"
#define PLAYMODE_RANDOM         L"播放模式 - 随机播放"
#define PLAYMODE_SINGLE         L"播放模式 - 单曲循环"

#define WM_MUSIC_END    WM_USER + 360

enum ONLINE_STATUS
{
	STATUS_OFFLINE = 0,			//离线
	STATUS_ONLINE = 1,			//在线//一直在綫
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };
	CMainDlg(void);
	~CMainDlg(void);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_MEASUREITEM(OnMeasureItem)
		MSG_WM_DRAWITEM(OnDrawItem)
	//	MSG_WM_TIMER(OnTimer)
		MSG_WM_SIZE(OnSize)
		MSG_WM_HOTKEY(OnHotKey)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(ID_MENU_SHOW_MAIN_PANEL, OnMenu_ShowMainPanel)
		//COMMAND_ID_HANDLER_EX(ID_BTN_MAIN_MENU, OnBtn_MainMenu)					//主菜单按钮
		COMMAND_ID_HANDLER_EX(ID_MENU_EXIT, OnMenu_Exit)
		COMMAND_ID_HANDLER_EX(ID_MENU_MINISIZE, OnMenu_MINISIZE)
		COMMAND_HANDLER(IDC_BTN_OPEN, BN_CLICKED, OnBnClickedBtnOpen)

		
		
		COMMAND_HANDLER(ID_MENU_STOP, BN_CLICKED, OnBnClickedBtnStop)
		
		COMMAND_HANDLER(ID_MENU_NEXT, BN_CLICKED, OnBnClickedBtnNext)
		COMMAND_HANDLER(ID_MENU_PREV, BN_CLICKED, OnBnClickedBtnBack)
		COMMAND_HANDLER(IDC_BTN_NEXT, BN_CLICKED, OnBnClickedBtnNext)
		COMMAND_HANDLER(IDC_BTN_PLAY_PAUSE, BN_CLICKED, OnBnClickedBtnPlayPause)
		COMMAND_HANDLER(IDC_BTN_BACK, BN_CLICKED, OnBnClickedBtnBack)
	//	COMMAND_HANDLER(IDC_BTN_STOP, BN_CLICKED, OnBnClickedBtnStop)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		//MESSAGE_HANDLER(WM_TIMER, OnTimer)
		//MESSAGE_HANDLER(WM_MUSIC_END, OnMusicEnd)


		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		//MESSAGE_HANDLER(WM_MUSIC_END, OnMusicEnd)
		//COMMAND_HANDLER(IDC_STATIC_MODE, STN_CLICKED, OnStnClickedStaticMode)


		MESSAGE_HANDLER_EX(WM_TRAYICON_NOTIFY, OnTrayIconNotify)

		NOTIFY_HANDLER_EX(ID_TABCTRL_MAIN, TCN_DROPDOWN, OnTabCtrlDropDown)
		NOTIFY_HANDLER_EX(ID_TABCTRL_MAIN, TCN_SELCHANGE, OnTabCtrlSelChange)
		NOTIFY_HANDLER_EX(ID_LEFTPAN_LISTCTRL, NM_DBLCLK, OnLeftPanListDblClk)
	
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

private:
	CSkinDialog				m_SkinDlg;										//主框架对话框
	CMsgTipDlg				m_MsgTipDlg;
	CSkinMenu				m_SkinMenu;										//左下角带头像的菜单
	CLeftPanListCtrl        m_leftpanSkinMenu;					//主对话框左侧的菜单 
	BOOL InitUI();
	void OnSysCommand(UINT nID, CPoint pt);
	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void OnSize(UINT nType, CSize size);
	BOOL LoadAppIcon(long nStatus);
	LRESULT OnTrayIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey);
	//void OnBtn_MainMenu(UINT uNotifyCode, int nId, CWindow wndCtl);
	void OnMenu_ShowMainPanel(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMenu_Exit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMenu_MINISIZE(UINT uNotifyCode, int nID, CWindow wndCtl);
	BOOL InitTabCtrl();		// 初始化Top工具栏

	BOOL InitLeftListCtrl();

	LRESULT OnTabCtrlDropDown(LPNMHDR pnmh);
	LRESULT OnTabCtrlSelChange(LPNMHDR pnmh);
	LRESULT OnLeftPanListDblClk(LPNMHDR pnmh);

	void OnTrayIcon_LButtunUp();
	void OnTrayIcon_RButtunUp();
	void OnTrayIcon_MouseHover();
	void OnTrayIcon_MouseLeave();

	void DestroyAppIcon();
	void UninitUI();
	CSkinButton				m_btnMainMenu;
	CSkinTabCtrl			m_TabCtrl;
	HICON							m_hAppIcon;
	CTrayIcon						m_TrayIcon;

	HRGN							m_hHotRgn;


private://播放器
	int         m_iPlaying;         // -1 无效；1播放中；0暂停
	int         m_iVolume;          // 音量
	DWORD       m_nAudioLen;        // 播放时间
	int         m_iCount;           // 播放项个数（0-N）可以作为最后一项索引
	MCIPlayer   m_player;           // 播放控制
	CButton     m_btnPlayPause;
	CButton     m_btnNext;
	CButton     m_btnBack;
	CButton     m_btnStop;
	CButton     m_pBtnOpen;
	DWORD       m_nTick;

	int         m_iPlayMode;        // 0 顺序; 1 随机; 2单曲
	CStatic                   m_staticMode;
	CListViewCtrl             m_listMusic;
	CProgressBarCtrl          m_progress;
	std::vector<std::wstring> m_vListPath;      // 播放列表路径
	std::vector<std::wstring> m_vTime;          // 播放时间列表
	void        SetWindowTextMusic();// 标题栏显示播放状态
	inline void ResetProgressBar();
	inline void SuspendProgressBar();
	inline void ResumeProgressBar();
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void        DrawProgressStatus();
	LPCTSTR     GetSelectedMusicPath();
public:

	BOOL GetFileTitleByFullPath(const std::wstring& strFullPath, std::wstring& strTitle);
	void GetFormatedTime(DWORD dwTotalSeconds, std::wstring& strFormated);
	LRESULT OnBnClickedBtnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnNext(WORD, WORD, HWND, BOOL&);        // 下一曲
	LRESULT OnBnClickedBtnPlayPause(WORD, WORD, HWND, BOOL&);   // 播放 暂停
	LRESULT OnBnClickedBtnBack(WORD, WORD, HWND, BOOL&);        // 上一曲
	LRESULT OnBnClickedBtnStop(WORD, WORD, HWND, BOOL&);        // 停止
};
