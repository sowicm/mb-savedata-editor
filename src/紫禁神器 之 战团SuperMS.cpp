
// �Ͻ����� ֮ ս��SuperMS.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "�Ͻ����� ֮ ս��SuperMS.h"
#include "MainFrm.h"
#include "MainCore.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMainApp::OnAppAbout)
END_MESSAGE_MAP()


// CMainApp ����

CMainApp::CMainApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("�Ͻ����� ֮ ս��SuperMS.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMainApp ����

CMainApp theApp;


// CMainApp ��ʼ��

BOOL CMainApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




    // ����������ã��Ѿ���������LoadCustomState���������С

    //pFrame->SetWindowPos(NULL, 100, 80, 810, 670, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int CMainApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMainApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMainApp �Զ������/���淽��

void CMainApp::PreLoadState()
{
    /*
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
    */
}

void CMainApp::LoadCustomState()
{
    // �������ڴ�С
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    m_pMainWnd->GetWindowPlacement(&wp);
    wp.rcNormalPosition.right = wp.rcNormalPosition.left + 810;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + 670;
    m_pMainWnd->SetWindowPlacement(&wp);

    // ��ȡ��Ϸ·���Ͱ汾

    CSettingsStoreSP regSP;
    // m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    CSettingsStore&  reg = regSP.Create(TRUE, FALSE);
    if (!reg.Open(_T("SOFTWARE\\Mount&Blade Warband\\")) ||
        !reg.Read(_T(""), theCore.m_strGamePath)         ||
        !reg.Read(_T("Version"), theCore.m_strGameVersion))
    {
        if (theCore.m_strGamePath.IsEmpty())
        {
            AfxMessageBox(_T("δ��⵽ս�Ű�װĿ¼�����ֶ�ָ����"));

            BROWSEINFO bi;
            ZeroMemory(&bi, sizeof(BROWSEINFO));
            bi.lpszTitle = _T("ѡ��ս����ϷĿ¼");
            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

            SHGetPathFromIDList(pidl, theCore.m_strGamePath.GetBuffer(MAX_PATH));
            theCore.m_strGamePath.ReleaseBuffer();
        }
        else
        {
            AfxMessageBox(_T("���棺δ��⵽ս�Ű汾�ţ����עhttp://zi-jin.com��ʱ���£�"));
        }
    }
    else if (theCore.m_strGameVersion.Compare(_T("1134")))
    {
        CString sz;
        sz.Format(_T("��⵽���ս�Ű汾��%s������������Ѽ��ݵİ汾��1.134�����עhttp://zi-jin.com"),
            theCore.m_strGameVersion);
        AfxMessageBox(sz);
    }
    reg.Close();

    
    if (theCore.m_strGamePath.Right(1) != _T("\\"))
        theCore.m_strGamePath.Append(_T("\\"));

    // ��֤�ڶ�ȡ����Ϣ֮����ִ������...
    if (!((CMainFrame*)m_pMainWnd)->GetD3D())
    {
        ((CMainFrame*)m_pMainWnd)->InitDevice();
        ((CMainFrame*)m_pMainWnd)->InitGeometry();
    }
    
}

void CMainApp::SaveCustomState()
{
}

// CMainApp ��Ϣ�������



// 
BOOL CMainApp::OnIdle(long lCount)
{
    static BOOL bNeedMore = TRUE;
    if (lCount == 0)
        bNeedMore = TRUE;
    if (bNeedMore)
    {
        bNeedMore = CWinAppEx::OnIdle(lCount);
    }
    ((CMainFrame*)m_pMainWnd)->Render();
    return TRUE; // ����TRUE��һֱ���ã�����0���ʾ������Ҫ����
}

