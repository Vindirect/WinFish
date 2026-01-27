#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"

namespace Sexy
{
	class WinFishApp;
	class BubbleMgr;
	class DialogButton;

	extern int gHighScoreScreenPage;

	class HighScoreScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		BubbleMgr* mBubbleMgr;
		DialogButton* mMenuButton;
		DialogButton* mAdvButton;
		DialogButton* mTimeTrialButton;
		DialogButton* mChallengeButton;
		DialogButton* mPersonalButton;
		int mPage;

		enum
		{
			PAGE_ADV,
			PAGE_TIME,
			PAGE_CHAL,
			PAGE_PERSONAL
		};

	public:
		HighScoreScreen(WinFishApp* theApp);
		virtual ~HighScoreScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			DrawGameModePage(Graphics* g);
		virtual void			DrawPersonalRecordsPage(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
	};
}