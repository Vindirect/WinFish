#ifndef _HELP_SCREEN__H_
#define _HELP_SCREEN__H_

#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"

namespace Sexy
{
	class WinFishApp;
	class DialogButton;
	class BubbleMgr;

	extern int gHelpScreenTab;

	class HelpScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		BubbleMgr* mBubbleMgr;
		DialogButton* mContinueButton;
		DialogButton* mMenuButton;
		DialogButton* mNextButton;
		DialogButton* mPreviousButton;
		int	mPageIdx;
		int	m0xa8;

	public:
		HelpScreen(WinFishApp* theApp, bool theFlag);
		virtual ~HelpScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			Draw(Graphics* g);

		Rect					GetVertTabRect(int theTabNum);
		Rect					GetLeftRectVertTab(int theTabNum, int theYWidth);
		void					CircleAroundCursor(int theUpdateCnt, int theX, int theY, int& theDestX, int& theDestY);

		void					DrawTabTitle(Graphics* g, SexyString theLine, SexyString theLine2);
		void					DrawVertTabText(Graphics* g, int theTabIdx, SexyString theLine, SexyString theLine2, int theHeightDrawOffset = 4);
		void					DrawInstructionsPage(Graphics* g);
		void					DrawInstrLeftPart(Graphics* g, Rect& theRect);
		void					DrawInstrMiddlePart(Graphics* g, Rect& theRect);
		void					DrawInstrRightPart(Graphics* g, Rect& theRect);
		void					DrawCursor(Graphics* g, int theX, int theY, bool doClick);
		void					DrawInstrText(Graphics* g, Rect& theRect, SexyString& theTitle, SexyString* theLines, int theNumOfLines);
		void					DrawTheBasicsPage(Graphics* g);
		void					DrawUpgradesPage(Graphics* g);
		void					DrawPetsPage(Graphics* g);
		void					DrawGameModesPage(Graphics* g);
		void					DrawGameModesPageText(Graphics* g, SexyString theString, int theX, int theY);
		void					DrawVirtualTankPage(Graphics* g);
		void					DrawVirtualTankFishPage(Graphics* g);
		void					DrawConfigVirtualTankPage(Graphics* g);
		void					DrawCreditsPage(Graphics* g);
		void					DrawCreditsPageLeftText(Graphics* g, SexyString theString, int theX, int theY);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
	};

}

#endif