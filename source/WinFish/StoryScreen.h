#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"

namespace Sexy
{
	class WinFishApp;
	class BubbleMgr;
	class HyperlinkWidget;
	class StoryScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		HyperlinkWidget* mBackToMainMenuButton;
		HyperlinkWidget* mNextButton;
		HyperlinkWidget* mBackButton;
		BubbleMgr* mBubbleMgr;
		Image* mTalkShowImg;
		int mStoryId;

	public:
		StoryScreen(WinFishApp* theApp, int theStoryId);
		virtual ~StoryScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		void ChangeStory(int theDirection);
		void LoadTalkShow();
	};
}