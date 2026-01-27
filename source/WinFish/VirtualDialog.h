#pragma once

#include "MoneyDialog.h"
#include "BubbleMgr.h"

namespace Sexy
{
	class VirtualDialog : public MoneyDialog, public ButtonListener
	{
	public:
		BubbleMgr*				mBubbleMgr;
		Rect					mFishBoxRect;

	public:
		VirtualDialog(WinFishApp* theApp);
		virtual ~VirtualDialog();

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
	};
}
