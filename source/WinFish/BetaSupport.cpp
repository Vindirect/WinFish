#include "BetaSupport.h"
#include "WinFishApp.h"

Sexy::BetaSupport::BetaSupport(WinFishApp* theApp)
{
	mApp = theApp;

	m0x14 = 0;
	m0x1c = 0;
	m0x128 = false;
	m0x129 = true;
	mHWND1 = 0;

	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);

	int height8 = MulDiv(8, 96, 72);
	int height10 = MulDiv(10, 96, 72);

	mTahomaFont = CreateFontA(-height8, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
	mArialFont = CreateFontA(-height8, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	mTahomaBoldFont = CreateFontA(-height10, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");

	ReleaseDC(hWnd, hDC);
}

Sexy::BetaSupport::~BetaSupport()
{
	if (mHWND1 != 0)
	{
		DoMessageLoop();
		DestroyWindow(mHWND1);
		mHWND1 = 0;
	}
	DeleteObject(mTahomaFont);
	DeleteObject(mArialFont);
	DeleteObject(mTahomaBoldFont);
	DoMessageLoop();
}

void Sexy::BetaSupport::DoMessageLoop()
{
	if (m0x129)
		return;

	MSG msg;

	while (true)
	{
		BOOL bRet = GetMessageA(&msg, NULL, 0, 0);

		if (bRet <= 0)
			break;

		if (!IsDialogMessageA(mHWND1, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (m0x129)
			break;
	}
}

bool Sexy::BetaSupport::Validate()
{
	// TODO
	return true;
}

bool Sexy::BetaSupport::Validation()
{
	// TODO
	return true;
}
