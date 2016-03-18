
// 紫禁神器 之 战团SuperMS.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "紫禁神器 之 战团SuperMS.h"
#include "MainFrm.h"
#include "MainCore.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMainApp::OnAppAbout)
END_MESSAGE_MAP()


// CMainApp 构造

CMainApp::CMainApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("紫禁神器 之 战团SuperMS.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMainApp 对象

CMainApp theApp;


// CMainApp 初始化

BOOL CMainApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




    // 这个方法不好，已经换成了在LoadCustomState那里调整大小

    //pFrame->SetWindowPos(NULL, 100, 80, 810, 670, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int CMainApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMainApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMainApp 自定义加载/保存方法

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
    // 调整窗口大小
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    m_pMainWnd->GetWindowPlacement(&wp);
    wp.rcNormalPosition.right = wp.rcNormalPosition.left + 810;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + 670;
    m_pMainWnd->SetWindowPlacement(&wp);

    // 读取游戏路径和版本

    CSettingsStoreSP regSP;
    // m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    CSettingsStore&  reg = regSP.Create(TRUE, FALSE);
    if (!reg.Open(_T("SOFTWARE\\Mount&Blade Warband\\")) ||
        !reg.Read(_T(""), theCore.m_strGamePath)         ||
        !reg.Read(_T("Version"), theCore.m_strGameVersion))
    {
        if (theCore.m_strGamePath.IsEmpty())
        {
            AfxMessageBox(_T("未检测到战团安装目录，请手动指定！"));

            BROWSEINFO bi;
            ZeroMemory(&bi, sizeof(BROWSEINFO));
            bi.lpszTitle = _T("选择战团游戏目录");
            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

            SHGetPathFromIDList(pidl, theCore.m_strGamePath.GetBuffer(MAX_PATH));
            theCore.m_strGamePath.ReleaseBuffer();
        }
        else
        {
            AfxMessageBox(_T("警告：未检测到战团版本号！请关注http://zi-jin.com及时更新！"));
        }
    }
    else if (theCore.m_strGameVersion.Compare(_T("1134")))
    {
        CString sz;
        sz.Format(_T("检测到你的战团版本是%s，而本程序最佳兼容的版本是1.134，请关注http://zi-jin.com"),
            theCore.m_strGameVersion);
        AfxMessageBox(sz);
    }
    reg.Close();

    
    if (theCore.m_strGamePath.Right(1) != _T("\\"))
        theCore.m_strGamePath.Append(_T("\\"));

    // 保证在读取了信息之后再执行以下...
    if (!((CMainFrame*)m_pMainWnd)->GetD3D())
    {
        ((CMainFrame*)m_pMainWnd)->InitDevice();
        ((CMainFrame*)m_pMainWnd)->InitGeometry();
    }
    
}

void CMainApp::SaveCustomState()
{
}

// CMainApp 消息处理程序



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
    return TRUE; // 返回TRUE则一直调用，返回0则表示不再需要处理
}

