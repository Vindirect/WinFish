#include "SimSetupScreen.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "WinFishApp.h"
#include "Res.h"
#include "Board.h"

Sexy::SimSetupScreen::SimSetupScreen(WinFishApp* theApp)
{
	m0x94 = 0;
	m0x98 = 0;
	mApp = theApp;
	m0x9c = false;
	mX = 0;
	mY = 0;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;
	mApp->StopMusic();
	mApp->PlayMusic(2, 0);
	m0x98 = 140;
	m0x94 = 60;
	mReturnButton = MakeDialogButton(9, this, "Return to Tank", FONT_JUNGLEFEVER10OUTLINE);
	mReturnButton->Resize(225, 420, 186, mReturnButton->mHeight);

	mSSButton = MakeDialogButton(8, this, "Screensaver...", FONT_JUNGLEFEVER10OUTLINE);
	mSSButton->Resize(80, 320, 186, mSSButton->mHeight);

	mPrevButton = MakeDialogButton2(0, this, "Prev", IMAGE_LEFTBUTTON);
	mPrevButton->Resize(m0x94 + 14, m0x98 + 134, 65, mPrevButton->mHeight);

	mNextButton = MakeDialogButton2(1, this, "Next", IMAGE_RIGHTBUTTON);
	mNextButton->Layout(0x4403, mPrevButton, 65, 0, 0, 0);

	mSellButton = MakeDialogButton2(2, this, "Sell", IMAGE_CENTERBUTTON);
	mSellButton->Layout(0x4402, mPrevButton, -2, 0, 0, 0);
	mSellButton->Layout(0x20000, mNextButton, 0, 0, 3, 0);

	mPrevButton->SetColor(0, Color::White);
	mNextButton->SetColor(0, Color::White);
	mSellButton->SetColor(0, Color::White);
	mReturnButton->SetColor(0, Color(255, 240, 0));
	mSSButton->SetColor(0, Color::White);

	mFishNamesCB = MakeCheckbox(3, this, mApp->mBoard->m0x4fc);
	mBubbulatorCB = MakeCheckbox(4, this, mApp->mBoard->mBubbulatorShown);
	mAttractorCB = MakeCheckbox(5, this, mApp->mBoard->mAlienAttractorShown);
	mShowHungryCB = MakeCheckbox(7, this, mApp->mBoard->mAlwaysShowWhenHungry);
	mDropShellsCB = MakeCheckbox(6, this, mApp->mBoard->m0x500);

	mFishNamesCB->Resize(340, 140, 46, 45);
	mBubbulatorCB->Layout(0x1203, mFishNamesCB);
	mAttractorCB->Layout(0x1203, mBubbulatorCB);
	mDropShellsCB->Layout(0x1203, mAttractorCB);
	mShowHungryCB->Layout(0x1203, mDropShellsCB);
}

Sexy::SimSetupScreen::~SimSetupScreen()
{
	if (mReturnButton)
		delete mReturnButton;
	if (mSSButton)
		delete mSSButton;
	if (mPrevButton)
		delete mPrevButton;
	if (mNextButton)
		delete mNextButton;
	if (mSellButton)
		delete mSellButton;
	if (mFishNamesCB)
		delete mFishNamesCB;
	if (mBubbulatorCB)
		delete mBubbulatorCB;
	if (mAttractorCB)
		delete mAttractorCB;
	if (mShowHungryCB)
		delete mShowHungryCB;
	if (mDropShellsCB)
		delete mDropShellsCB;
}

void Sexy::SimSetupScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mReturnButton);
	theWidgetManager->AddWidget(mSSButton);
	theWidgetManager->AddWidget(mPrevButton);
	theWidgetManager->AddWidget(mNextButton);
	theWidgetManager->AddWidget(mSellButton);
	theWidgetManager->AddWidget(mFishNamesCB);
	theWidgetManager->AddWidget(mBubbulatorCB);
	theWidgetManager->AddWidget(mAttractorCB);
	theWidgetManager->AddWidget(mShowHungryCB);
	theWidgetManager->AddWidget(mDropShellsCB);
}

void Sexy::SimSetupScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mReturnButton);
	theWidgetManager->RemoveWidget(mSSButton);
	theWidgetManager->RemoveWidget(mPrevButton);
	theWidgetManager->RemoveWidget(mNextButton);
	theWidgetManager->RemoveWidget(mSellButton);
	theWidgetManager->RemoveWidget(mFishNamesCB);
	theWidgetManager->RemoveWidget(mBubbulatorCB);
	theWidgetManager->RemoveWidget(mAttractorCB);
	theWidgetManager->RemoveWidget(mShowHungryCB);
	theWidgetManager->RemoveWidget(mDropShellsCB);
}

void Sexy::SimSetupScreen::Update()
{
	Widget::Update();
	MarkDirty();
}

void Sexy::SimSetupScreen::Draw(Graphics* g)
{
	g->DrawImageBox(Rect(-5, -5, 650, 490), IMAGE_SCREENBACK);
	g->DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 13, 400, 614);
	g->DrawImage(IMAGE_BOLT, 33, 385);
	g->DrawImage(IMAGE_BOLT, 587, 385);
	DrawVertCrease(g, IMAGE_VERTCREASE, 310, 105, 298);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 13, 100, 614);
	g->DrawImage(IMAGE_BOLT, 33, 115);
	g->DrawImage(IMAGE_BOLT, 587, 115);
	g->DrawImage(IMAGE_BOLT, 295, 115);
	g->DrawImage(IMAGE_BOLT, 295 + 30, 115);
	g->DrawImage(IMAGE_BOLT, 295, 115 + 271);
	g->DrawImage(IMAGE_BOLT, 295 + 30, 115 + 271);

	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(0xff, 200, 0));
	WriteCenteredLine(g, 25, "Tank Setup");
	
	g->DrawImage(IMAGE_TANKCHOOSER, m0x94, m0x98);
	g->DrawImage(GetImageById(mApp->mBoard->mCurrentBackgroundId + (IMAGE_TANKBACKDROP1_ID - 1)), m0x94 + 16, m0x98 + 10);
	
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color::White);
	SexyString aStr = "Current Backdrop";
	g->DrawString(aStr, (IMAGE_TANKCHOOSER->mWidth - g->GetFont()->StringWidth(aStr)) / 2 + m0x94, m0x98 - 5);

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color::White);
	DrawCheckboxString(g, "Show Fish Names", mFishNamesCB, nullptr);
	DrawCheckboxString(g, "Show Bubbulator", mBubbulatorCB, nullptr);
	DrawCheckboxString(g, "Enable Alien Attractor", mAttractorCB, nullptr);
	DrawCheckboxString(g, "Allow Fish To Drop Shells", mDropShellsCB, nullptr);
	DrawCheckboxString(g, "Always Show When", mShowHungryCB, "Fish Are Hungry");

	SexyString aInfoStr = "";
	if (mFishNamesCB->mIsOver)
		aInfoStr = "This checkbox controls whether or not to display the names of your fish in your Virtual Tank.";
	else if (mBubbulatorCB->mIsOver)
		aInfoStr = "This checkbox controls whether or not to display the Bubbulator in your Virtual Tank.";
	else if (mAttractorCB->mIsOver)
		aInfoStr = "This checkbox controls whether or not to display the Alien Attractor in your Virtual Tank.";
	else if (mDropShellsCB->mIsOver)
		aInfoStr = "This checkbox controls whether fish will drop shells in your Virtual Tank.";
	else if (mShowHungryCB->mIsOver)
		aInfoStr = "Virtual Fish only need to be fed three times per day.  After that, they will not look hungry unless you check this checkbox.";
	else if (mSSButton->mIsOver || mSSButton->mIsDown)
		aInfoStr = "Click to set your Virtual Tank as your computer\'s screensaver.\nThe screensaver will display the Virtual Tank belonging to the user who last enabled it.";
	else if (mSellButton->mIsOver || mSellButton->mIsDown)
		aInfoStr = "You can sell your tank backdrops if you\'re in need of more Shells.";
	else if (mNextButton->mIsOver || mNextButton->mIsDown || mPrevButton->mIsOver || mPrevButton->mIsDown)
		aInfoStr = "You can choose which tank backdrop to display in Virtual Tank.  You can buy additional backdrops from the Virtual Tank Store.";
	else
		aInfoStr = "Welcome to the Virtual Tank setup screen!";

	if (aInfoStr.length() > 0)
	{
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffaa));
		int aHght = GetWordWrappedHeight(g, 520, aInfoStr, -1);
		WriteWordWrapped(g, Rect(60,70 - aHght/2, 520, 0), aInfoStr, -1, 0);
	}

	g->SetColor(Color::White);
	aStr = StrFormat("The Screensaver is %s", mApp->mScreenSaverEnabled ? "enabled" : "disabled");
	int aStrX = mSSButton->mWidth / 2 + mSSButton->mX;
	int aStrY = mSSButton->mHeight + 20 + mSSButton->mY;
	g->DrawString(aStr, aStrX - g->GetFont()->StringWidth(aStr) / 2, aStrY);

	if (mApp->mScreenSaverEnabled)
	{
		aStr = StrFormat("User: %s", mApp->mScreenSaverUserName.c_str());
		g->DrawString(aStr, aStrX - g->GetFont()->StringWidth(aStr) / 2, aStrY + 15);
	}
}

void Sexy::SimSetupScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::SimSetupScreen::ButtonDepress(int theId)
{
	if (theId == 9)
	{
		mApp->RemoveSimSetupScreen();
		mApp->mBoard->m0x4fc = mFishNamesCB->mChecked;
		mApp->mBoard->mBubbulatorShown = mBubbulatorCB->mChecked;
		mApp->mBoard->mAlienAttractorShown = mAttractorCB->mChecked;
		mApp->mBoard->m0x500 = mDropShellsCB->mChecked;
		mApp->mBoard->mAlwaysShowWhenHungry = mShowHungryCB->mChecked;
		if (!mApp->mBoard->mAlienAttractorShown)
			mApp->mBoard->mAlienTimer = 3000;
		mApp->mBoard->StartMusic();
		if (m0x9c)
			mApp->SaveVirtualTankAndUserData();
		mApp->mBoard->PauseGame(false);
;	}
	else if (theId == 0 || theId == 1)
	{
		m0x9c = true;
		int aBGIdx = mApp->mBoard->mCurrentBackgroundId - 1;
		mApp->mCurrentProfile->mUnlockedBackgrounds[aBGIdx] = true;
		do
		{
			aBGIdx += (theId == 0 ? -1 : 1);
			if (aBGIdx < 0)
				aBGIdx = 5;
			if (aBGIdx > 5)
				aBGIdx = 0;
		} while (!mApp->mCurrentProfile->mUnlockedBackgrounds[aBGIdx]);
		mApp->mBoard->ChangeBackground(aBGIdx + 1);
	}
	else if (theId == 2)
	{
		int aNumOfBackdrops = 0;
		for (int i = 0; i < 6; i++)
		{
			if (mApp->mCurrentProfile->mUnlockedBackgrounds[i])
				aNumOfBackdrops++;
		}
		if (aNumOfBackdrops == 1)
		{
			mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Not Allowed", "You are not allowed to sell your only backdrop!", "OK", Dialog::BUTTONS_FOOTER);
		}
		else
		{
			mApp->DoAreYouSureSellDialog(StrFormat("Are you sure that you want to sell this backdrop?\n\nRefund Value: %d Shells", 10000));
		}
	}
	else if (theId == 8)
	{
		mApp->DoScreenSaverDialog();
	}
}

void Sexy::SimSetupScreen::CheckboxChecked(int theId, bool checked)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
	m0x9c = true;
	if (theId == 4 && checked && mApp->mCurrentProfile->mBubbulatorBought == 0)
	{
		mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Unavailable", "You must buy the bubbulator in order to use this option.", "OK", Dialog::BUTTONS_FOOTER);
		mBubbulatorCB->mChecked = false;
	}
	else if (theId == 5 && checked && mApp->mCurrentProfile->mAlienAttractorBought == 0)
	{
		mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Unavailable", "You must buy the alien attractor in order to use this option.", "OK", Dialog::BUTTONS_FOOTER);
		mAttractorCB->mChecked = false;
	}
}

void Sexy::SimSetupScreen::SellBackground(bool sell)
{
	if (sell)
	{
		m0x9c = true;
		int aCurBgIdx = mApp->mBoard->mCurrentBackgroundId - 1;
		int aNextFree = 0;
		for (; aNextFree < 6; aNextFree++)
		{
			if (mApp->mCurrentProfile->mUnlockedBackgrounds[aNextFree] && aCurBgIdx != aNextFree)
				break;
		}
		if (aNextFree != 6)
		{
			mApp->mCurrentProfile->mUnlockedBackgrounds[aCurBgIdx] = false;
			mApp->mCurrentProfile->AddShells(10000);
			int aNextShowBg = aCurBgIdx;
			while (!mApp->mCurrentProfile->mUnlockedBackgrounds[aNextShowBg])
			{
				aNextShowBg++;
				if (aNextShowBg < 0)
					aNextShowBg = 5;
				else if (aNextShowBg > 5)
					aNextShowBg = 0;
			}
			mApp->mBoard->ChangeBackground(aNextShowBg+1);
		}
	}
}
