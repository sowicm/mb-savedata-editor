
// 紫禁神器 之 战团SuperMS.h : 紫禁神器 之 战团SuperMS 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "GUIResource.h"


// CMainApp:
// 有关此类的实现，请参阅 紫禁神器 之 战团SuperMS.cpp
//

class CMainApp : public CWinAppEx
{
public:
	CMainApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

    virtual BOOL OnIdle(long lCount);

// 实现

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;
