
// �Ͻ����� ֮ ս��SuperMS.h : �Ͻ����� ֮ ս��SuperMS Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "GUIResource.h"


// CMainApp:
// �йش����ʵ�֣������ �Ͻ����� ֮ ս��SuperMS.cpp
//

class CMainApp : public CWinAppEx
{
public:
	CMainApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

    virtual BOOL OnIdle(long lCount);

// ʵ��

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;
