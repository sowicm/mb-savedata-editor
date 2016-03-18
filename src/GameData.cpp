
#include "main.h"
#include "GameData.h"

#include "MainWnd.h"

cGameData::cGameData()
{
}

cGameData::~cGameData()
{
}


bool cGameData::Initialize()
{
    if (!GetGamePath())
        return false;

    if (!GetModules())
        return false;

    return true;
}

bool cGameData::GetGamePath()
{
    HKEY hKey;
    LONG rc;
    rc = RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Mount&Blade Warband\\"), &hKey);
    if (rc != ERROR_SUCCESS)
    {
        rc = RegOpenKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Mount&Blade Warband\\"), &hKey);
        if (rc != ERROR_SUCCESS)
        {
            MessageBox(NULL, _T("未检测到《骑马与砍杀：战团》的安装信息"), _T("Warning"), MB_OK | MB_ICONWARNING);
            goto error;
        }
    }
    
    DWORD dwType;
    DWORD cbData;
    rc = RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)m_strPath.BeginBuffer(261), &cbData);
    m_strPath.EndBuffer();
    if (rc != ERROR_SUCCESS || dwType != REG_SZ)
    {
        MessageBox(NULL, _T("未检测到《骑马与砍杀：战团》的安装路径"), _T("Warning"), MB_OK | MB_ICONWARNING);
        goto error;
    }

    rc = RegQueryValueExA(hKey, "Version", NULL, &dwType, (LPBYTE)m_strVersion, &cbData);
    if (rc != ERROR_SUCCESS || dwType != REG_SZ)
    {
        MessageBox(NULL, _T("未检测到《骑马与砍杀：战团》的版本号"), _T("Warning"), MB_OK | MB_ICONWARNING);
    }
    if (strcmp(m_strVersion, "1134"))
    {
        char str[1024];
        sprintf(str, "检测到你的战团版本是%s，而本程序最佳兼容的版本是1.134，请关注本程序的更新",
            m_strVersion);
        MessageBoxA(NULL, str, "Warning", MB_OK | MB_ICONWARNING);
    }
    
    RegCloseKey(hKey);

    return true;

error:
    RegCloseKey(hKey);
    return false;
}

#define THE_BUG 1

bool cGameData::GetModules()
{    
    WIN32_FIND_DATAA fd;
    cString str = m_strPath + "/Modules/";
    HANDLE hFindFile = FindFirstFileA(str + "*.*", &fd);
#if THE_BUG
    cPointersArray<cModuleInfo*> arr;
#endif
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
                strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
            {
                FILE *fp;
                if (fp = fopen(str + fd.cFileName + "/module.ini", "rb"))//if (PathFileExistsA(cString(fd.cFileName) + "/module.ini"))
                {
                    fclose(fp);
                    cModuleInfo *pmi = new cModuleInfo;
                    pmi->m_strName = fd.cFileName;
                    pmi->m_strPath = str + fd.cFileName;
#if THE_BUG
                    arr.Add(pmi);
#else
                    m_Modules.Add(pmi);
#endif
                }
            }
        } while (FindNextFileA(hFindFile, &fd));
    }
    FindClose(hFindFile);

#if THE_BUG
    m_Modules = arr;
    memset(&arr, 0, sizeof(arr));
#endif

    return true;
}