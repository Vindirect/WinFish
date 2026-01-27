#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/DialogButton.h"
#include "WinFishApp.h"
#include "BubbleMgr.h"

namespace Sexy
{
	class BonusScreenOverlay : public Widget
	{
	public:
		BonusScreen* mScreen;

	public:
		virtual void			Draw(Graphics* g);
	};

	class BonusScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp*				mApp;
		BubbleMgr*				mBubbleMgr;
		Rect					m0x94;
		BonusScreenOverlay*		mOverlay;
		DialogButton*			mMenuButton;
		DialogButton*			mContinueButton;
		DialogButton*			mUnkButton1;
		MemoryImage*			mShellImage;
		int						m0xb8;
		int						mBonusReward;
		int						m0xc0;
		int						m0xc4;
		int						m0xc8;
		int						m0xcc;
		int						m0xd0;
		int						m0xd4;
		int						m0xd8;
		int						m0xdc;
		bool					m0xe0;
		bool					m0xe1;
		SexyString				mTitle;
		SexyString				mCurScore1;
		SexyString				mCurScore2;
		SexyString				mBestScore1;
		SexyString				mBestScore2;
		SexyString				mRewardString;

	public:
		BonusScreen(WinFishApp* theApp);
		virtual ~BonusScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			DrawOverlay(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		void					DrawMisc(Graphics* g);
		void					DrawMisc2(Graphics* g);
		void					DrawMisc3(Graphics* g);
		void					Init();
		void					ConfirmPurchase();
	};
}