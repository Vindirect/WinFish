#pragma once

#include "MoneyDialog.h"
#include "WinFishApp.h"

namespace Sexy
{
	class ContinueDialog : public MoneyDialog, public ButtonListener
	{
	public:
		DialogButton*			mContinueButton;
		DialogButton*			mRestartButton;

	public:
		ContinueDialog(WinFishApp* theApp);
		virtual ~ContinueDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int				GetPreferredHeight(int theWidth);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
	};
}

