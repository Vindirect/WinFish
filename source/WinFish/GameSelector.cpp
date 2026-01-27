#include "GameSelector.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/ButtonWidget.h"

Sexy::GameSelector::GameSelector(WinFishApp* theApp)
{
	mApp = theApp;
	mSparkleX = 0;
	mSparkleY = 0;
	mAdventureButton = new ButtonWidget(1, this);
	mAdventureButton->mDoFinger = true;
	mAdventureButton->mButtonImage = IMAGE_BLANK;
	mAdventureButton->mOverImage = IMAGE_BATTLETANKBUTTON;
	mAdventureButton->mDownImage = IMAGE_BATTLETANKBUTTOND;
	mAdventureButton->SetFont(FONT_JUNGLEFEVER15OUTLINE);
	mAdventureButton->mColors[ButtonWidget::COLOR_LABEL] = Color(0xff, 0xf0, 0);
	mAdventureButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 0xff);
	mAdventureButton->Resize(357, 48, 217, 66);

	mVirtualTankButton = new ButtonWidget(2, this);
	mVirtualTankButton->mDoFinger = true;
	mVirtualTankButton->mButtonImage = IMAGE_BLANK;
	mVirtualTankButton->mOverImage = IMAGE_BATTLETANKBUTTON;
	mVirtualTankButton->mDownImage = IMAGE_BATTLETANKBUTTOND;
	mVirtualTankButton->SetFont(FONT_JUNGLEFEVER15OUTLINE);
	mVirtualTankButton->mLabel = "Virtual Tank";
	mVirtualTankButton->mColors[ButtonWidget::COLOR_LABEL] = Color(0xff, 0xf0, 0);
	mVirtualTankButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 0xff);
	mVirtualTankButton->Resize(357, 287, 217, 66);

	mChallengeButton = new ButtonWidget(9, this);
	mChallengeButton->mDoFinger = true;
	mChallengeButton->mButtonImage = IMAGE_BLANK;
	mChallengeButton->mOverImage = IMAGE_MIDDLEBUTTON;
	mChallengeButton->mDownImage = IMAGE_MIDDLEBUTTOND;
	mChallengeButton->SetFont(FONT_JUNGLEFEVER15OUTLINE);
	mChallengeButton->mLabel = "Challenge";
	mChallengeButton->mColors[ButtonWidget::COLOR_LABEL] = Color(0xff, 0xf0, 0);
	mChallengeButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 0xff);
	mChallengeButton->Resize(359, 212, 213, 48);

	mTimeTrialButton = new ButtonWidget(8, this);
	mTimeTrialButton->mDoFinger = true;
	mTimeTrialButton->mButtonImage = IMAGE_BLANK;
	mTimeTrialButton->mOverImage = IMAGE_MIDDLEBUTTON;
	mTimeTrialButton->mDownImage = IMAGE_MIDDLEBUTTOND;
	mTimeTrialButton->SetFont(FONT_JUNGLEFEVER15OUTLINE);
	mTimeTrialButton->mLabel = "Time Trial";
	mTimeTrialButton->mColors[ButtonWidget::COLOR_LABEL] = Color(0xff, 0xf0, 0);
	mTimeTrialButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 0xff);
	mTimeTrialButton->Resize(359, 142, 213, 48);

	mOptionsButton = MakeDialogButton2(3, this, "Options", IMAGE_LEFTBUTTON);
	mOptionsButton->Resize(325, 412, 92, mOptionsButton->mHeight);

	mQuitButton = MakeDialogButton2(4, this, "Quit", IMAGE_RIGHTBUTTON);
	mQuitButton->Resize(514, 412, 89, mQuitButton->mHeight);

	mHallOfFameButton = MakeDialogButton2(6, this, "Hall of Fame", IMAGE_MAINBUTTON);
	mHallOfFameButton->Resize(401, 380, 127, mHallOfFameButton->mHeight);

	mHelpButton = MakeDialogButton2(7, this, "Help", IMAGE_CENTERBUTTON);
	mHelpButton->Resize(419, 412, 93, mHelpButton->mHeight);

	mGameSelectorOverlay = new GameSelectorOverlay();
	mGameSelectorOverlay->mScreen = this;
	mGameSelectorOverlay->mMouseVisible = false;
	mGameSelectorOverlay->mHasAlpha = true;
	mGameSelectorOverlay->Resize(0, 0, 640, 480);

	mMerylBlinkTimer = mApp->mSeed->Next() % 150 + 100;
	mMerylFlopTimer = mApp->mSeed->Next() % 500 + 200;

	mNotYouButton = NULL;

	if (!mApp->m0x883 && !mApp->IsSongPlaying(2, 0))
	{
		mApp->StopMusic();
		mApp->PlayMusic(2, 0);
	}
	if (mApp->mCurrentProfile && mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel > 1)
	{
		mApp->mCurrentProfile->mTank = 1;
		mApp->mCurrentProfile->mLevel = 1;
		mApp->mCurrentProfile->mFinishedGameCount = 0;
		mApp->mCurrentProfile->m0x58 = false;
	}

	UpdateAdventureButton();
	mHoverId = 0;
	mSpeechFadeTimer = 0;
	mPreviousHoverId = 0;
	mHoverChangeTimer = 0;

	mSandboxCheatCode = new CheatCode({ KEYCODE_UP, KEYCODE_UP ,KEYCODE_DOWN, KEYCODE_DOWN, 
		KEYCODE_LEFT, KEYCODE_RIGHT, KEYCODE_LEFT, KEYCODE_RIGHT, 'B', 'A'});
	mGiveCheatCode = new CheatCode("give");

	mSparkleAnimTimer = 0;
}

Sexy::GameSelector::~GameSelector()
{
	if (mAdventureButton)
		delete mAdventureButton;
	if (mVirtualTankButton)
		delete mVirtualTankButton;
	if (mOptionsButton)
		delete mOptionsButton;
	if (mQuitButton)
		delete mQuitButton;
	if (mChallengeButton)
		delete mChallengeButton;
	if (mTimeTrialButton)
		delete mTimeTrialButton;
	if (mHallOfFameButton)
		delete mHallOfFameButton;
	if (mHelpButton)
		delete mHelpButton;
	if (mGameSelectorOverlay)
		delete mGameSelectorOverlay;
	if (mNotYouButton)
		delete mNotYouButton;
	if (mSandboxCheatCode)
		delete mSandboxCheatCode;
	if (mGiveCheatCode)
		delete mGiveCheatCode;
}

void Sexy::GameSelector::Update()
{
	Widget::Update();
	mMerylBlinkTimer--;
	if (mMerylBlinkTimer < 1)
	{
		int aVal;
		if (mApp->mSeed->Next() % 7 == 0)
			aVal = mApp->mSeed->Next() % 15 + 15;
		else
			aVal = mApp->mSeed->Next() % 150 + 100;
		mMerylBlinkTimer = aVal;
	}

	mMerylFlopTimer--;
	if (mMerylFlopTimer < 1)
		mMerylFlopTimer = mApp->mSeed->Next() % 500 + 200;

	if (mSparkleAnimTimer < 1)
	{
		if (mSparkleAnimTimer == 0 && mApp->mCurrentProfile && mApp->mCurrentProfile->m0x80 && mUpdateCnt > 40)
		{
			mSparkleAnimTimer = 1;
			mSparkleX = 170;
			mSparkleY = 310;
			int aCnt = 0;
			int aRand1, aRand2;
			do {
				ulong aVal = GetBonusAwardUnk((MemoryImage*) IMAGE_BONUSAWARD, (aRand1 = Rand()) % IMAGE_BONUSAWARD->mWidth, (aRand2 = Rand()) % IMAGE_BONUSAWARD->mHeight);
				if ((aVal & 0xff000000) == 0xff000000) break;
				aCnt++;
			} while (aCnt < 100);

			mSparkleX += aRand1 % IMAGE_BONUSAWARD->mWidth;
			mSparkleY += aRand2 % IMAGE_BONUSAWARD->mHeight;
			mSparkleX = mSparkleX - IMAGE_SPARKLE->GetCelWidth() / 2;
			mSparkleY = mSparkleY - IMAGE_SPARKLE->GetCelHeight() / 2;
		}
	}
	else
	{
		mSparkleAnimTimer++;
		if (mSparkleAnimTimer > 59)
			mSparkleAnimTimer = 0;
	}

	MarkDirty();
	int aHover = 0;
	if (mAdventureButton->mIsOver || mAdventureButton->mIsDown)
		aHover = 1;
	else if (mTimeTrialButton->mIsOver || mTimeTrialButton->mIsDown)
		aHover = 2;
	else if (mChallengeButton->mIsOver || mChallengeButton->mIsDown)
		aHover = 3;
	else if (mVirtualTankButton->mIsOver || mVirtualTankButton->mIsDown)
		aHover = 4;
	else
	{
		if (mApp->mCurrentProfile && mApp->mCurrentProfile->mBonusItemId > 5)
		{
			Rect aBonusArea(220, 340, 50, 50);
			int aContains = aBonusArea.Contains(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
			if (aContains && mApp->mDialogMap.size() == 0)
				aHover = mApp->mCurrentProfile->m0x80 == 0 ? 5 : 6;
		}
	}
	UpdateHover(aHover);
	if (mSpeechFadeTimer > 0)
		mSpeechFadeTimer--;

	if (mNotYouButton->mVisible)
	{
		if (!mApp->mCurrentProfile || mHoverId != 0)
		{
			mNotYouButton->SetVisible(false);
			MarkDirty();	
			UpdateAdventureButton();
			return;
		}
	}

	if(!mNotYouButton->mVisible)
	if (mApp->mCurrentProfile && mHoverId == 0)
	{
		mNotYouButton->SetVisible(true);
		MarkDirty();
	}

	UpdateAdventureButton();
}

void Sexy::GameSelector::Draw(Graphics* g)
{
	g->DrawImage(IMAGE_SELECTORSCREEN, 0, 0);
	UserProfile* aProf = mApp->mCurrentProfile;
	if (aProf && aProf->mBonusItemId > 5)
	{
		g->DrawImage(aProf->m0x80 != 0 ? IMAGE_BONUSAWARD2 : IMAGE_BONUSAWARD, 170, 310);
		if (mSparkleAnimTimer > 0 && mSparkleAnimTimer <= IMAGE_SPARKLE->mNumCols)
		{
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			g->SetColorizeImages(true);
			g->SetColor(Color::White);
			g->DrawImageCel(IMAGE_SPARKLE, mSparkleX, mSparkleY, mSparkleAnimTimer - 1);
			g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		}
	}

	g->DrawImageAnim(IMAGE_MERYLBLINK, 139, 196, mMerylBlinkTimer);
	g->DrawImageAnim(IMAGE_MERYLTAIL, 39, 301, mMerylFlopTimer);

	if (mSpeechFadeTimer > 0)
	{
		int aVal = mSpeechFadeTimer * 255;
		int aVal2 = aVal / 5;
		DrawMerylSpeak(g, mHoverId, 255 - aVal2, true);
		DrawMerylSpeak(g, mPreviousHoverId, aVal2, false);
	}
	else
		DrawMerylSpeak(g, mHoverId, 255, true); // May not be correct
}

void Sexy::GameSelector::DrawOverlay(Graphics* g)
{
	UserProfile* aProf;
	if (!(aProf = mApp->mCurrentProfile))
		return;

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color(0xe1, 0xfa, 0xfa));
	SexyString anAdvString;
	if (aProf->mTank == 5 && aProf->mLevel == 1)
		anAdvString = GetCyraxEndGameString(aProf->mCyraxNum +1);
	else
	{
		if (aProf->mLevel == 6)
			anAdvString = StrFormat("Bonus Level %d", aProf->mTank);
		else if(aProf->mLevel != 1 || aProf->mTank != 1)
			anAdvString = StrFormat("Tank %d-%d", aProf->mTank, aProf->mLevel);
	}

	if (anAdvString.size() > 0)
	{
		int aStrWdth = g->GetFont()->StringWidth(anAdvString);
		g->DrawString(anAdvString, (0 - aStrWdth / 2) + 465, 107);
	}

	if (aProf->mShells > 0)
	{
		SexyString aStr = StrFormat("%d Shells", aProf->mShells);
		g->DrawString(aStr, (-g->GetFont()->StringWidth(aStr) / 2) + 465, 346);
	}
}

void Sexy::GameSelector::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mAdventureButton);
	theWidgetManager->AddWidget(mVirtualTankButton);
	theWidgetManager->AddWidget(mOptionsButton);
	theWidgetManager->AddWidget(mQuitButton);
	theWidgetManager->AddWidget(mChallengeButton);
	theWidgetManager->AddWidget(mTimeTrialButton);
	theWidgetManager->AddWidget(mHallOfFameButton);
	theWidgetManager->AddWidget(mHelpButton);

	if (!mNotYouButton)
	{
		mNotYouButton = new HyperlinkWidget(9999, this);
		mNotYouButton->SetFont(FONT_TINY);
		mNotYouButton->mColor = Color(0, 0, 120);
		mNotYouButton->mOverColor = Color(100, 100, 220);
		mNotYouButton->mDoFinger = true;
		mNotYouButton->mLabel = "If this is not you, click here.";
		mNotYouButton->mUnderlineSize = 1;
		mNotYouButton->SetVisible(true);
		int aStrWdth = mNotYouButton->mFont->StringWidth(mNotYouButton->mLabel);
		int aStrHght = mNotYouButton->mFont->GetHeight();
		mNotYouButton->Resize(97, 90, aStrWdth, aStrHght);
	}

	theWidgetManager->AddWidget(mNotYouButton);
	theWidgetManager->AddWidget(mGameSelectorOverlay);
}

void Sexy::GameSelector::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mAdventureButton);
	theWidgetManager->RemoveWidget(mVirtualTankButton);
	theWidgetManager->RemoveWidget(mOptionsButton);
	theWidgetManager->RemoveWidget(mQuitButton);
	theWidgetManager->RemoveWidget(mChallengeButton);
	theWidgetManager->RemoveWidget(mTimeTrialButton);
	theWidgetManager->RemoveWidget(mHallOfFameButton);
	theWidgetManager->RemoveWidget(mHelpButton);
	if(mNotYouButton)
		theWidgetManager->RemoveWidget(mNotYouButton);
	theWidgetManager->RemoveWidget(mGameSelectorOverlay);
}

void Sexy::GameSelector::OrderInManagerChanged()
{
	mWidgetManager->PutInfront(mGameSelectorOverlay, this);
	if(mNotYouButton)
		mWidgetManager->PutInfront(mNotYouButton, this);
	mWidgetManager->PutInfront(mHelpButton, this);
	mWidgetManager->PutInfront(mHallOfFameButton, this);
	mWidgetManager->PutInfront(mTimeTrialButton, this);
	mWidgetManager->PutInfront(mChallengeButton, this);
	mWidgetManager->PutInfront(mQuitButton, this);
	mWidgetManager->PutInfront(mOptionsButton, this);
	mWidgetManager->PutInfront(mVirtualTankButton, this);
	mWidgetManager->PutInfront(mAdventureButton, this);
}

void Sexy::GameSelector::KeyChar(SexyChar theChar)
{
	if (theChar == 'n') // TODO REMOVE THIS
		mApp->mCurrentProfile->NextLevel(); // REMOVE THIS

	if (mApp->mDebugKeysEnabled)
		return;
	if (mSandboxCheatCode->CheckCodeActivated(theChar))
		SandboxActivated();
	if (mGiveCheatCode->CheckCodeActivated(theChar))
		mApp->DoGiveDialog();
}

void Sexy::GameSelector::KeyDown(KeyCode theKey)
{
	if (mApp->mDebugKeysEnabled)
		return;
	if (mSandboxCheatCode->CheckCodeActivated(theKey))
		SandboxActivated();
	if (mGiveCheatCode->CheckCodeActivated(theKey))
		mApp->DoGiveDialog();
}

void Sexy::GameSelector::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::GameSelector::ButtonDepress(int theId)
{
	mApp->mRelaxMode = false;
	if (theId == 1)
	{
		mApp->mGameMode = WinFishApp::GAMEMODE_ADVENTURE;
		mApp->RemoveGameSelector();
		mApp->SwitchToBoard(true, true);
	}
	else if (theId == 2)
	{
		mApp->mGameMode = WinFishApp::GAMEMODE_VIRTUAL_TANK;
		mApp->RemoveGameSelector();
		mApp->SwitchToBoard(true, true);
	}
	else if (theId == 3)
	{
		mApp->DoOptionsDialog(true);
	}
	else if (theId == 4)
	{
		mApp->DoQuitDialog();
	}
	else if (theId == 6)
	{
		mApp->SwitchToHighScoreScreen();
	}
	else if (theId == 7)
	{
		mApp->SwitchToHelpScreen(false);
	}
	else if (theId == 8)
	{
		UserProfile* aProf = mApp->mCurrentProfile;
		if (aProf->mTank < 2 && !aProf->mFinishedGame)
		{
			mApp->DoDialog(14, true, "Not Yet!", "You\'ll need to complete a tank in Adventure Mode before this option becomes available.", "OK", Dialog::BUTTONS_FOOTER);
			return;
		}
		mApp->mGameMode = WinFishApp::GAMEMODE_TIME_TRIAL;
		mApp->RemoveGameSelector();
		if (mApp->LoadBoardGame())
			return;
		mApp->SwitchToTankScreen();
	}
	else if (theId == 9)
	{
		if (!mApp->mCurrentProfile->mFinishedGame)
		{
			mApp->DoDialog(14, true, "Not Yet!", "You\'ll need to beat Adventure Mode before this option becomes available.", "OK", Dialog::BUTTONS_FOOTER);
			return;
		}
		mApp->mGameMode = WinFishApp::GAMEMODE_CHALLENGE;
		mApp->RemoveGameSelector();
		if (mApp->LoadBoardGame())
			return;
		mApp->SwitchToTankScreen();
	}
	else if (theId == 9999)
	{
		mApp->DoWhoAreYouDialog();
	}
}

void Sexy::GameSelector::MouseDown(int x, int y, int theClickCount)
{
	if (x > 19 && x < 310 && y > 149 && y < 510 && mMerylFlopTimer > 15)
		mMerylFlopTimer = 15;
}

void Sexy::GameSelector::DrawMerylSpeak(Graphics* g, int theHoverId, int theAlphaValue, bool theDrawSpeechBubble)
{
	UserProfile* aProf = mApp->mCurrentProfile;
	int aTank = 1;
	int aLevel = 1;
	bool aFinishedGame = false;
	SexyString anUserName = "";
	if (aProf)
	{
		anUserName = aProf->mUserName;
		aTank = aProf->mTank;
		aLevel = aProf->mLevel;
		aFinishedGame = aProf->mFinishedGame;
	}
	if (theDrawSpeechBubble)
	{
		if(anUserName.empty() && theHoverId < 1)
			return;
		g->DrawImage(IMAGE_SELECTORSPEECHBUBBLE, 45, 20);
	}

	if (theHoverId > 0)
	{
		g->SetFont(FONT_BLAMBOTPRO8);
		g->SetColor(Color(0,0,100, theAlphaValue));
		SexyString aStrToDraw = "";
		switch (theHoverId)
		{
		case 1:
			if (aTank == 1 && aLevel == 1 && !aFinishedGame)
				aStrToDraw = "New to Insaniquarium?\nClick here to start your aquatic adventure!";
			else
				aStrToDraw = "Feed fish and fight aliens!\nClick here to continue\nyour adventure...";
			break;
		case 2:
			aStrToDraw = "How much money can you earn before time runs out?";
			break;
		case 3:
			aStrToDraw = "Can you fend off the increasingly difficult aliens?";
			break;
		case 4:
			aStrToDraw = "Buy and raise your own\ncustom fish, then use them\nas a screensaver!";
			break;
		case 5:
			aStrToDraw = "Wow!  Nice trophy!";
			break;
		case 6:
			aStrToDraw = "The pets wanted you to have this solid gold trophy since you are now the undisputed champion of all Insaniquarium!";
			break;
		default:
			break;
		}

		int aHght = GetWordWrappedHeight(g, 214, aStrToDraw, 20);
		int anY = 40 + (60 - aHght) / 2;
		WriteWordWrapped(g, Rect(64, anY, 214, 60), aStrToDraw, 20, 0);
	}
	else if (!anUserName.empty())
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, theAlphaValue));

		if (aProf && !aProf->m0x4c)
			g->DrawImage(IMAGE_WELCOMETO, (250 - IMAGE_WELCOMETO->mWidth) / 2 + 45, 33);
		else
			g->DrawImage(IMAGE_WELCOMEBACK, (250 - IMAGE_WELCOMETO->mWidth) / 2 + 45, 33);
		g->SetColorizeImages(false);

		anUserName += "!";
		g->SetFont(FONT_BLAMBOTPRO15);
		int aStrWdth = g->GetFont()->StringWidth(anUserName);
		g->SetColor(Color(0, 0, 100, theAlphaValue));
		g->DrawString(anUserName, ((250 - aStrWdth) / 2) + 45, 77);
		if (mNotYouButton)
		{
			g->SetFont(mNotYouButton->mFont);
			Color theColor;
			if (mNotYouButton->mIsOver)
				theColor = mNotYouButton->mOverColor;
			else
				theColor = mNotYouButton->mColor;

			theColor.mAlpha = theAlphaValue;
			g->SetColor(theColor);
			int aFontAscent = g->GetFont()->GetAscent();
			g->DrawString("If this is not you, click here.", mNotYouButton->mX,
				(aFontAscent + mNotYouButton->mHeight) / 2 - 1 + mNotYouButton->mY);
		}
	}
}

ulong Sexy::GameSelector::GetBonusAwardUnk(MemoryImage* theImage, int theUnk1, int theUnk2)
{
	if (theUnk1 >= 0 && theUnk2 >= 0 && theUnk1 <= theImage->mWidth && theUnk2 <= theImage->mHeight)
	{
		int aVal = theImage->mWidth * theUnk2 + theUnk1;
		if (theImage->mColorIndices != 0)
			return theImage->mColorTable[theImage->mColorIndices[aVal]];
		ulong* bits = theImage->GetBits();
		return bits[aVal];
	}

	return 0;
}

void Sexy::GameSelector::UpdateHover(int theHover)
{
	if (mHoverId != theHover)
	{
		mHoverChangeTimer++;
		if (mHoverChangeTimer - 1 > 4)
		{
			mSpeechFadeTimer = 5;
			mPreviousHoverId = mHoverId;
			mHoverId = theHover;
			mHoverChangeTimer = 0;
		}
	}
}

void Sexy::GameSelector::UpdateAdventureButton()
{
	UserProfile* aProf = mApp->mCurrentProfile;
	if (aProf)
	{
		if (aProf->mFinishedGame)
		{
			mAdventureButton->mLabel = "Bonus Adventure";
			return;
		}
		if(aProf->mTank == 1 && aProf->mLevel == 1)
		{
			mAdventureButton->mLabel = "Start Adventure";
			return;
		}
	}
	mAdventureButton->mLabel = "Adventure";
}

void Sexy::GameSelector::SandboxActivated()
{
	UserProfile* aProf = mApp->mCurrentProfile;
	if (!aProf || aProf->mBonusItemId <= 5)
		return;

	mApp->mGameMode = WinFishApp::GAMEMODE_SANDBOX;
	if (aProf->m0xb4 < 11)
		aProf->m0xb4 = 11;
	mApp->RemoveGameSelector();
	mApp->StartGame();
}

void Sexy::GameSelectorOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
