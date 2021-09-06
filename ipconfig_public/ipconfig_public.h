
// ipconfig_public.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "Resource.h"		// メイン シンボル


// CipconfigpublicApp:
// このクラスの実装については、ipconfig_public.cpp を参照してください
//

class CipconfigpublicApp : public CWinApp
{
public:
	CipconfigpublicApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CipconfigpublicApp theApp;
