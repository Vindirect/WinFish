#pragma once

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/HTTPTransfer.h"

namespace Sexy
{
	class WinFishApp;
	class BetaSupport
	{
	public:
		WinFishApp* mApp;
		HFONT mTahomaFont;
		HFONT mArialFont;
		HFONT mTahomaBoldFont;
		int m0x14;
		HWND mHWND1;
		int m0x1c;

		HTTPTransfer mHTTPTrans1;
		bool m0x128;
		bool m0x129;
		SexyString mStr1;
		SexyString mStr2;
		SexyString mStr3;
		SexyString mStr4;

		StringStringMap mStrMap;

	public:
		BetaSupport(WinFishApp* theApp);
		virtual ~BetaSupport();

		void DoMessageLoop();
		bool Validate();
		bool Validation();
	};
}

