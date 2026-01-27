#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "WinFishApp.h"
#include "BubbleMgr.h"

namespace Sexy
{
	class HatchScreenOverlay : public Widget
	{
	public:
		HatchScreen* mScreen;

	public:
		virtual void			Draw(Graphics* g);
	};

	class HatchScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		BubbleMgr* mBubbleMgr;
		int m0x94;
		int mPetId;
		int m0x9c;
		int m0xa0;
		int m0xa4;
		DialogButton* mContinueButton;
		DialogButton* mMenuButton;
		HatchScreenOverlay* mOverlay;

	public:
		HatchScreen(WinFishApp* theApp, int thePetId);
		virtual ~HatchScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			DrawOverlay(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
	};
}

