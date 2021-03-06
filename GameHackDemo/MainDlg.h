// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GameHelper.h"
#include "..\GameHack\Core\GameUtility.h"
#include "..\GameHack\Core\InjectDll.h"
#include "..\GameHack\Core\ModuleHide.h"

#pragma comment(lib,"WS2_32.lib")
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
    public CMessageFilter, public CIdleHandler
{
private:
    CGameHelper m_gameHelper;
public:
    enum { IDD = IDD_MAINDLG };

    virtual BOOL PreTranslateMessage(MSG* pMsg)
    {
        return CWindow::IsDialogMessage(pMsg);
    }

    virtual BOOL OnIdle()
    {
        UIUpdateChildWindows();
        return FALSE;
    }

    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
    COMMAND_HANDLER(IDC_BUTTON2, BN_CLICKED, OnBnClickedButton2)
    END_MSG_MAP()

    // Handler prototypes (uncomment arguments if needed):
    //  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

		TestPipe();

        return TRUE;
    }

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        // unregister message filtering and idle updates
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);
        pLoop->RemoveMessageFilter(this);
        pLoop->RemoveIdleHandler(this);

        m_gameHelper.Stop();
        return 0;
    }

    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }

    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        // TODO: Add validation code
        CloseDialog(wID);
        return 0;
    }

    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CloseDialog(wID);
        return 0;
    }

    void CloseDialog(int nVal)
    {
        m_gameHelper.Stop();
        DestroyWindow();
        ::PostQuitMessage(nVal);
    }

    void GetConfigPath(CString& sPath) const
    {
        TCHAR sModulePath[MAX_PATH] = {0};
        GetModuleFileName(NULL, sModulePath, MAX_PATH);
        CString sModule = sModulePath;
        sModule.Truncate(sModule.ReverseFind(_T('\\')) + 1);
        sPath = sModule + _T("GameHack.ini");
    }

	void GetGameInfo(CString& sGameName,CString& sInjectDll)
	{
		const CString strGameName = _T("5pkmir2_hb.dat");
		const CString strDllPath = _T("D:\\git\\GameHack\\bin\\Debug\\GameHack.dll");
		sGameName=strGameName;
		sInjectDll=strDllPath;
	}

    LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        const CString sGameName = _T("5pkmir2_hb.dat");
        CString sInfo;

        if(m_gameHelper.Start(sGameName))
        {
            sInfo = _T("��F8������Ҵ���");
        }
        else
        {
            sInfo = _T("�������ʧ��");
        }

        CStatic ctrlInfo = GetDlgItem(IDC_STATIC_INFO);
        ctrlInfo.SetWindowText(sInfo);
        return 0;
    }

    LRESULT OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_gameHelper.Stop();

        CStatic ctrlInfo = GetDlgItem(IDC_STATIC_INFO);
        ctrlInfo.SetWindowText(_T(""));
        return 0;
    }

    void Test()
    {

		const CString strGameName = _T("notepad++.exe");
		const CString strDllPath = _T("H:\\Project\\GameHack\\bin\\Debug\\GameHack.dll");

		HMODULE h=LoadLibrary(strDllPath);
		CModuleHide moduleHide;
		moduleHide.LDRHide(h);
		return ;

        DWORD dwPID = GetPidFromName(strGameName);
        CInjectDll injectDll;

        if(!injectDll.ApcInject(dwPID, strDllPath))
        {
            MessageBox(_T("ע��ʧ��"));
			return ;
        }



        if(!injectDll.ApcUnInjectDll(dwPID, strDllPath))
        {
            MessageBox(_T("ж��ʧ��"));
        }
    }

	void TestPipe()
	{

	}
};
