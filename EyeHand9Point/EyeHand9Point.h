
// EyeHand9Point.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CEyeHand9PointApp:
// 有关此类的实现，请参阅 EyeHand9Point.cpp
//

class CEyeHand9PointApp : public CWinApp
{
public:
	CEyeHand9PointApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CEyeHand9PointApp theApp;
