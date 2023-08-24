
// SettingEditer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSettingEditerApp:
// このクラスの実装については、SettingEditer.cpp を参照してください
//

class CSettingEditerApp : public CWinApp
{
public:
	CSettingEditerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSettingEditerApp theApp;
