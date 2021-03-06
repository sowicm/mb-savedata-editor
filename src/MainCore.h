#pragma once

#include "MainFrm.h"
#include "SMS.h"
#include "BrfLoader.h"

class CModuleInfo
{
public:
    CModuleInfo() {}

    CString Path;
};


class CMainFrame;
class CMainCore
{
public:
    CMainCore(void);
    ~CMainCore(void);

    

    void LoadModule(UINT iModule);

    void LoadIni();

public:

    CString                  m_strGamePath;
    CString                  m_strGameVersion;

    CZJArray <CModuleInfo>   m_Modules;
    
    LPWSTR                   m_ModulePath;

    CMainFrame              *m_pMainFrm;
    void                     SetMainFrm(CMainFrame* pMainFrm)
    {
        m_pMainFrm = pMainFrm;
        m_BrfLoader.m_pDevice = pMainFrm->GetDevice();
        m_BrfLoader.m_pManager = &pMainFrm->m_Manager;
    }

    char                    *m_strIniFile;
    CZJPointerArray <char*>  m_BrfFileNames;

    // Thread
    //HANDLE                  m_hThread;
    //DWORD                   m_ThreadID;
    void                    _LoadModule();

    CBrfLoader              m_BrfLoader;

    // For Render
    UINT                    m_nLoadedBrfFiles;
    UINT                    m_nTotalBrfFiles;
    CZJGUIProgressBar*      m_pProgressBar;
    CZJGUIStatic*           m_pLoadingFileText;
#if defined(DEBUG)
    DWORD                   m_dwStartTime;
    CZJGUIStatic*           m_pCostTimeText;
#endif
};

extern CMainCore theCore;


const wchar_t tmp_Skls[][16] = 
{
    L"刚体",
    L"骁勇",
    L"强掷",
    L"强弓",
    L"精武",
    L"盾防",
    L"跑动",
    L"骑术",
    L"骑射",
    L"劫掠",
    L"练兵",
    L"慧眼",
    L"谋略",
    L"行军",
    L"视野",
    L"物品管理",
    L"养生",
    L"医术",
    L"包扎",
    L"工程学",
    L"说服",
    L"俘虏管理",
    L"统御",
    L"交涉"
};