#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/MemoryImage.h"
#include "WinFishApp.h"
#include "FishButtonWidget.h"
#include "BubbleMgr.h"

namespace Sexy
{
	class SimFishScreenOverlay : public Widget
	{
	public:
		SimFishScreen* mScreen;

	public:
		virtual void			Draw(Graphics* g);
	};

	class SimFishScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		BubbleMgr* mBubbleMgr;
		DialogButton* mReturnButton;
		DialogButton* mMenuButton;
		DialogButton* mHideShowButton;
		DialogButton* mSellButton;
		DialogButton* mRenameButton;
		DialogButton* mHideAllButton;
		DialogButton* mShowAllButton;
		SimFishScreenOverlay* mOverlay;
		FishButtonWidget* mObjectButtons[20];
		FishButtonWidget* mSelectedFishButton;
		MemoryImage* mBackgroundImage;
		int m0x10c;
		int m0x110;
		int m0x114;
		bool m0x118;
		bool m0x119;
		bool m0x11a;

	public:
		SimFishScreen(WinFishApp* theApp);
		virtual ~SimFishScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			OrderInManagerChanged();
		virtual void			DrawOverlay(Graphics* g);

		virtual void			ButtonPress(int theId, int theClickCount);
		virtual void			ButtonDepress(int theId);
		virtual void			ButtonMouseEnter(int theId);
		virtual void			ButtonMouseLeave(int theId);

		void					CreateFishButton(int theButtonId,GameObject* theObject);
		SexyString				GetAdditionalNotes(GameObject* theObject);
		SexyString				GetLastSpecialLike(GameObject* theObject);
		void					DetermineShowHideForButtons();
		void					RenameSelectedFish(SexyString theName);
		void					SellSelectedObject(bool sell);
	};
}