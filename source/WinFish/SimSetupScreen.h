#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/CheckboxListener.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/Checkbox.h"
#include "WinFishApp.h"

namespace Sexy
{
	class SimSetupScreen : public Widget, public ButtonListener, public CheckboxListener
	{
	public:
		WinFishApp* mApp;
		int m0x94;
		int m0x98;
		bool m0x9c;
		DialogButton* mReturnButton;
		DialogButton* mPrevButton;
		DialogButton* mNextButton;
		DialogButton* mSellButton;
		DialogButton* mSSButton;
		Checkbox* mFishNamesCB;
		Checkbox* mBubbulatorCB;
		Checkbox* mAttractorCB;
		Checkbox* mDropShellsCB;
		Checkbox* mShowHungryCB;

	public:
		SimSetupScreen(WinFishApp* theApp);
		virtual ~SimSetupScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		virtual void			CheckboxChecked(int theId, bool checked);

		void					SellBackground(bool sell);
	};
}