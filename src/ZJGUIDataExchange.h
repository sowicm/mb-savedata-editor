#include "..\..\CZJGUI\CZJGUI\ZJGUI.h"
#include <afxwin.h>

void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, CString& value);
void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, DWORD& value);
void DDX_Text(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, int& value);

void DDX_Check(CDataExchange* pDX, CZJGUI& gui, UINT iView, int nIDC, bool& value);

/*
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, BYTE& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, short& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, int& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, UINT& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, long& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, LONGLONG& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, ULONGLONG& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_Text(_Inout_ CDataExchange* pDX, _In_ int nIDC, _Out_z_cap_(nMaxLen) LPTSTR value, _In_ int nMaxLen);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, float& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, double& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, COleCurrency& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, COleDateTime& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, GUID& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, DECIMAL& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, FILETIME& value);
*/