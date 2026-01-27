#pragma once

#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/MemoryImage.h"
#include "WinFishApp.h"

namespace Sexy
{
	class GameSelector;
	class GameSelectorOverlay : public Widget
	{
	public:
		GameSelector* mScreen;

	public:
		virtual void			Draw(Graphics* g);
	};

	class GameSelector : public Widget, public ButtonListener
	{
	public:
		WinFishApp*				mApp;
		ButtonWidget*			mAdventureButton;
		ButtonWidget*			mVirtualTankButton;
		DialogButton*			mOptionsButton;
		DialogButton*			mQuitButton;
		ButtonWidget*			mChallengeButton;
		ButtonWidget*			mTimeTrialButton;
		DialogButton*			mHallOfFameButton;
		DialogButton*			mHelpButton;
		HyperlinkWidget*		mNotYouButton;
		int						mHoverId;
		int						mSpeechFadeTimer;
		int						mPreviousHoverId;
		int						mHoverChangeTimer;
		GameSelectorOverlay*	mGameSelectorOverlay;
		CheatCode*				mSandboxCheatCode;
		CheatCode*				mGiveCheatCode;
		int						mSparkleX;
		int						mSparkleY;
		int						mSparkleAnimTimer;
		int						mMerylBlinkTimer;
		int						mMerylFlopTimer;
		void*					m0xe4;

	public:
		GameSelector(WinFishApp* theApp);
		virtual ~GameSelector();

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			DrawOverlay(Graphics* g);

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			OrderInManagerChanged();

		virtual void			KeyChar(SexyChar theChar);
		virtual void			KeyDown(KeyCode theKey);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		virtual void			MouseDown(int x, int y, int theClickCount);

		void					DrawMerylSpeak(Graphics* g, int theHoverId, int theAlphaValue, bool theDrawSpeechBubble);
		ulong					GetBonusAwardUnk(MemoryImage* theImage, int theUnk1, int theUnk2);
		void					UpdateHover(int theHover);
		void					UpdateAdventureButton();
		void					SandboxActivated();

	};
}

