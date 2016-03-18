
#include "MainWnd.h"

#undef CreateWindow

cMainWnd *g_pWnd;

int APIENTRY _tWinMain(HINSTANCE hInst,
                       HINSTANCE hPrevInst,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    cMainWnd::m_hInst = hInst;
    cMainWnd wnd;
    g_pWnd = &wnd;

    if (wnd.CreateWindow() == -1)
    {
        MessageBox(NULL, _T("´´½¨´°¿ÚÊ§°Ü£¡"), _T("Error"), MB_ICONERROR | MB_OK);
        return -1;
    }

    if (!wnd.InitializeDevice())
        return -1;

    wnd.m_GameData.Initialize();

    if (!wnd.InitializeRes())
        return -1;

    if (!wnd.SetupGeometry())
        return -1;

    wnd.Run();

    return 0;
}