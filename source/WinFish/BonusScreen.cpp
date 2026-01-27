#include "BonusScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "Board.h"

#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/SoundManager.h"
#include "SexyAppFramework/SoundInstance.h"
#include "SexyAppFramework/Font.h"

Sexy::BonusScreen::BonusScreen(WinFishApp* theApp)
{
	mApp = theApp;
	mBubbleMgr = new BubbleMgr();

	mOverlay = new BonusScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mMouseVisible = false;
	mOverlay->mHasAlpha = true;
	mOverlay->Resize(0, 0, 640, 480);

	m0x94 = Rect(175, 230, 300, 225);
	mBubbleMgr->mBubbleBounds = Rect(m0x94.mX + 20, m0x94.mY - 15, m0x94.mWidth - 40, m0x94.mHeight-20);
	mBubbleMgr->SetBubbleConfig(10, 3);
	mBubbleMgr->UpdateALot();

	mMenuButton = MakeDialogButton2(1, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);
	UserProfile* aCurProf = mApp->mCurrentProfile;
	if (aCurProf->mTank == 5 && aCurProf->mLevel == 2)
		mMenuButton->mVisible = false;

	mContinueButton = MakeDialogButton2(0, this, "Click Here To Continue", IMAGE_MAINBUTTON);
	mContinueButton->Resize(186, 445, 264, mContinueButton->mHeight);
	mContinueButton->SetColor(0, Color(255, 240, 0));
	mContinueButton->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && mApp->mBoard)
	{
		mContinueButton->mLabel = "Click for Story Time";
		mMenuButton->mVisible = false;
	}

	mUnkButton1 = MakeDialogButton2(2, this, "", IMAGE_FATBUTTON);
	mUnkButton1->SetColor(0, Color(255, 255, 255));
	mUnkButton1->Resize(213, 200, 209, mUnkButton1->mHeight);
	mUnkButton1->SetVisible(true);
	m0xe0 = false;
	m0xc8 = 50;
	m0xc0 = 0;
	mBonusReward = 0;
	m0xc4 = aCurProf->mShells;
	Init();
	int aVal1 = mBonusReward / m0xc8;
	if (aVal1 < 15)
		m0xc8 = mBonusReward / 15;
	else if(aVal1 > 50)
		m0xc8 = mBonusReward / 50;

	m0xcc = 0;
	if (m0xc8 < 50)
		m0xc8 = 50;

	if (aCurProf->mBonusItemId > 5)
	{
		m0xd0 = 0;
		mUnkButton1->SetVisible(false);
	}
	else
	{
		m0xd0 = (aCurProf->mBonusItemId + 4) * 5000;
		if (m0xd0 > 50000)
			m0xd0 = 50000;
	}

	mMenuButton->SetDisabled(true);
	mContinueButton->SetDisabled(true);
	mUnkButton1->SetDisabled(true);
	mShellImage = nullptr;
	m0xd8 = 0;
	m0xdc = 0;
	m0xd4 = 0;
	m0xe1 = false;
	m0xb8 = mApp->GetCurrentTank();
}

Sexy::BonusScreen::~BonusScreen()
{
	if (mBubbleMgr)
		delete mBubbleMgr;
	if (mOverlay)
		delete mOverlay;
	if (mContinueButton)
		delete mContinueButton;
	if (mMenuButton)
		delete mMenuButton;
	if (mUnkButton1)
		delete mUnkButton1;
	if (mShellImage)
		delete mShellImage;
}

void Sexy::BonusScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mContinueButton);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mUnkButton1);
	theWidgetManager->AddWidget(mOverlay);
}

void Sexy::BonusScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mContinueButton);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mUnkButton1);
	theWidgetManager->RemoveWidget(mOverlay);
}

void Sexy::BonusScreen::Update()
{
	Widget::Update();
	if (mIsDown && m0xd4 > 0 && m0xd4 < 140)
		m0xd4 = 140;

	if (m0xd0 > 0 && m0xd0 <= mApp->mCurrentProfile->mShells && !m0xe1)
	{
		if (mUpdateCnt <= 10 || mUpdateCnt >= 180)
		{
			if (mUpdateCnt == 180)
			{
				mUnkButton1->mIsOver = mUnkButton1->Contains(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
			}
		}
		else
		{
			mUnkButton1->mIsOver = (((mUpdateCnt / 10) % 2) == 0);
		}
	}

	if (mUpdateCnt == 30)
	{
		mMenuButton->SetDisabled(false);
		mContinueButton->SetDisabled(false);
		mUnkButton1->SetDisabled(false);
	}
	else
	{
		if (m0xd4 > 0 && m0xd4 < 200)
		{
			m0xd8 = Rand() % 4;
			m0xd4++;
			m0xdc = Rand() % 2;
			if (m0xd4 == 141)
				mApp->PlaySample(SOUND_HATCH);
		}
		mBubbleMgr->Update();
		MarkDirty();
		if (m0xc0 < mBonusReward && mUpdateCnt > 30 && !m0xe0)
		{
			if (mUpdateCnt % 3 == 1)
			{
				m0xcc++;
				SoundInstance* anInst = mApp->mSoundManager->GetSoundInstance(SOUND_BONUSCOUNT);
				if (anInst)
				{
					anInst->AdjustPitch(m0xcc);
					anInst->Play(false, true);
				}
			}

			int aDisplayVal = m0xc8;
			if (mBonusReward < m0xc0 + m0xc8)
				aDisplayVal = mBonusReward - m0xc0;
			m0xc4 += aDisplayVal;
			m0xc0 += aDisplayVal;
			if (mApp->mCurrentProfile->mShells > m0xc4)
				m0xc4 = mApp->mCurrentProfile->mShells;
		}
	}
}

void Sexy::BonusScreen::Draw(Graphics* g)
{
	g->DrawImage(IMAGE_HATCHSCREEN, 0, 0);
	g->DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);

	if (mMenuButton->mVisible)
		g->DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);
	
	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(255, 200, 0, 255));

	SexyString aTitleStr = "BONUS RESULTS";
	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
		aTitleStr = "CHALLENGE RESULTS";
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
		aTitleStr = "TIME TRIAL RESULTS";

	WriteCenteredLine(g, 25, aTitleStr);

	mBubbleMgr->Draw(g);
	DrawMisc(g);
	if (m0xe0)
		DrawMisc2(g);
	else
		DrawMisc3(g);
}

void Sexy::BonusScreen::DrawOverlay(Graphics* g)
{
	int aVal = mApp->mCurrentProfile->mBonusItemId;
	if (mUnkButton1->mVisible)
	{
		g->SetFont(FONT_CONTINUUMBOLD14);
		g->SetColor(Color(0xffff00));
		SexyString aStr = "";
		if (aVal < 4)
		{
			aStr = StrFormat("Buy Bonus Pet #%d", aVal+1);
		}
		else if (aVal == 4)
		{
			aStr = StrFormat("Buy 4 Pet Limit");
		}
		else if (aVal == 5)
		{
			aStr = StrFormat("Buy 7 Pet Virtual Tank Limit");
			g->SetFont(FONT_CONTINUUMBOLD12OUTLINE);
		}

		int aStrWdth = g->GetFont()->StringWidth(aStr);
		int aBtnXCenter = mUnkButton1->mWidth / 2 + mUnkButton1->mX;
		int aBtnYPos = mUnkButton1->mY + 20;
		Rect aRect(218, 225, 200, 0);
		if (mUnkButton1->mIsDown && mUnkButton1->mIsOver)
		{
			aBtnXCenter++;
			aBtnYPos = mUnkButton1->mY + 21;
			aRect.mX = 219;
			aRect.mY = 226;
		}

		if (g->GetFont() == FONT_CONTINUUMBOLD14)
			DrawStringWithOutline(g, aStr, aBtnXCenter - aStrWdth / 2, aBtnYPos, FONT_CONTINUUMBOLD14OUTLINE, 0);
		else
			g->DrawString(aStr, aBtnXCenter - aStrWdth / 2, aBtnYPos);

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffff));
		SexyString aStr2 = StrFormat("for %s Shells", CommaSeperate(m0xd0).c_str());
		WriteWordWrapped(g, aRect, aStr2, -1, 0);
	}

	if (mShellImage != nullptr && m0xd4 > 139 && m0xd4 < 200)
	{
		int aXVel[8] = { 1, -11, -14, 12, -15, 11, 13, -1 };
		int aYVel[8] = { -10, -15, 3, -13, 10, 5, 14, 12 };
		int aTimeDelta = m0xd4 - 140;

		for (int i = 0; i < 8; i++)
		{
			int aX = aXVel[i] * aTimeDelta + 268;
			int aY = aYVel[i] * aTimeDelta + 70;

			g->DrawImageCel(mShellImage, aX, aY, i);
		}
	}
}

void Sexy::BonusScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
	if (theId == 2)
	{
		m0xe1 = true;
		mUnkButton1->mIsOver = mUnkButton1->Contains(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
	}
}

void Sexy::BonusScreen::ButtonDepress(int theId)
{
	if (theId == 1)
	{
		mApp->RemoveBonusScreen();
		mApp->LeaveGameBoard();
	}
	else if (theId == 0)
	{
		mApp->RemoveBonusScreen();
		if (mApp->mGameMode != WinFishApp::GAMEMODE_TIME_TRIAL && mApp->mGameSelector == nullptr)
		{
			if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
				mApp->SwitchToStoryScreen(m0xb8);
			else if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 2)
				mApp->SwitchToInterludeScreen();
			else
				mApp->SwitchToBoard(true, true);
		}
		else
			mApp->LeaveGameBoard();
	}
	else if (theId == 2)
	{
		if (mApp->mCurrentProfile->mShells < m0xd0)
		{
			mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Not Enough Shells", 
				StrFormat("Sorry, but you need more Shells to purchase %s.  Keep playing to earn more!", 
					mApp->mCurrentProfile->mBonusItemId < 4 ? "this Bonus Pet" : "this Bonus Upgrade"),
				"OK", Dialog::BUTTONS_FOOTER);
		}
		else if (mApp->mCurrentProfile->mBonusItemId < 4)
		{
			mApp->DoConfirmPurchaseDialog("Would you like to\nbuy this Bonus Pet?");
		}
		else
		{
			mApp->DoConfirmPurchaseDialog("Would you like to\nbuy this Bonus Upgrade?");
		}
	}
}

void Sexy::BonusScreen::DrawMisc(Graphics* g)
{
	if (mUnkButton1->mVisible)
		g->DrawImage(IMAGE_BONUSBUTTONOVERLAY, 200, 178);

	int aVal = mApp->mCurrentProfile->mBonusItemId;
	if (m0xe0)
		aVal--;

	if (aVal < 4)
	{
		if (m0xd4 < 140)
		{
			Image* anImg = GetImageById(aVal + IMAGE_EGGBRINKLEY_ID);
			g->DrawImage(anImg, (177 - anImg->mWidth) / 2 + 230 + m0xd8, (159 - anImg->mHeight) / 2 + 53 + m0xdc);
		}
	}
	else if (aVal < 6)
	{
		int anX = 270;
		int anXOffset = 97;
		if (aVal == 4)
		{
			anX = 275;
			anXOffset = 87;
		}
		Image* anImg = IMAGE_PETLIMITEGG;
		g->DrawImage(anImg, anX, 81);
		g->DrawImage(anImg, anX + (anXOffset - anImg->mWidth), 81);
		g->DrawImage(anImg, anX, 180 - anImg->mHeight);
		g->DrawImage(anImg, anXOffset - anImg->mWidth + anX, 180 - anImg->mHeight);

		if (aVal == 5)
		{
			g->DrawImage(anImg, (anXOffset - anImg->mWidth) / 2 - 60 + anX, (99 - anImg->mHeight) / 2 + 81);
			g->DrawImage(anImg, (anXOffset - anImg->mWidth) / 2 + anX, (99 - anImg->mHeight) / 2 + 81);
			g->DrawImage(anImg, (anXOffset - anImg->mWidth) / 2 + 60 + anX, (99 - anImg->mHeight) / 2 + 81);
		}
	}
	else
	{
		Image* anImg = IMAGE_BONUSAWARD;
		if (mApp->mCurrentProfile->m0x7c == -1)
			anImg = IMAGE_BONUSAWARD2;
		g->DrawImage(anImg, 240, 66);
	}
	g->DrawImage(IMAGE_HATCHREFLECTION, 240, 60);
}

void Sexy::BonusScreen::DrawMisc2(Graphics* g)
{
	Rect aDestWrap(m0x94.mX + 10, m0x94.mY + 20, m0x94.mWidth - 20, m0x94.mHeight);
	int aWhat = mApp->mCurrentProfile->mBonusItemId - 1;
	if (aWhat < 4)
	{
		SexyString aTitleStr = "";
		SexyString anInfoStr = "";
		Image* anImg = nullptr;
		int aSpeed = 2;
		switch (aWhat)
		{
		case 0:
			aTitleStr = "BRINKLEY the Scuba Diving Elephant!";
			anInfoStr = "Likes: Peach muffins, all\nthings brown and sticky\nDislikes: Arugula";
			anImg = IMAGE_BRINKLEY;
			aSpeed = 4;
			break;
		case 1:
			aTitleStr = "NOSTRADAMUS the Nose!";
			anInfoStr = "Little known fact:  NOSTRADAMUS\nis the long lost nose of ex-president\nRutherford B. Hayes.";
			anImg = IMAGE_NOSTRADAMUS;
			break;
		case 2:
			aTitleStr = "STANLEY the Startlingly Small Sea Serpent!";
			anInfoStr = "STANLEY knows no fear.. except\nthat of badgers, aprons,\nand badgers wearing aprons.";
			anImg = IMAGE_STANLEY;
			break;
		case 3:
			aTitleStr = "WALTER the Penguin!";
			anInfoStr = "All shells used to purchase\nWALTER are donated to\nthe Falafel Foundation.\nFree the falafels!";
			anImg = IMAGE_WALTER;
			break;
		default:
			return;
		}

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffff));

		WriteWordWrapped(g, aDestWrap, "You Have Bought", -1, 0);

		if (m0xd4 >= 141)
		{
			aDestWrap.mY += 20;
			g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
			g->SetColor(Color(0xffc800));

			WriteWordWrapped(g, aDestWrap, aTitleStr, -1, 0);

			g->DrawImageCel(anImg, 278, 90, (mUpdateCnt / aSpeed) % 10, 0);
			g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
			g->SetColor(Color::White);
			aDestWrap.mY = m0x94.mY + 110;
			WriteWordWrapped(g, aDestWrap, anInfoStr, -1, 0);
		}
	}
	else if (aWhat <= 5)
	{ // 118
		aDestWrap.mY = m0x94.mY + 20;

		g->SetColor(Color(0xffc800));
		g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
		WriteWordWrapped(g, aDestWrap, "Congratulations!", -1, 0);

		aDestWrap.mY += 35;
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffff));
		WriteWordWrapped(g, aDestWrap, "You can now use", -1, 0);

		aDestWrap.mY += 20;
		g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
		g->SetColor(Color(0xffc800));
		SexyString aPetStr = "";
		SexyString anIdkStr = "";
		if (aWhat + 1 == 5)
		{
			aPetStr = "FOUR";
			anIdkStr = "Four pets?!!!\nThat\'s INSANE!";
		}
		else if (aWhat == 5)
		{
			aPetStr = "SEVEN";
		}
		WriteWordWrapped(g, aDestWrap, aPetStr, -1, 0);

		aDestWrap.mY += 30;
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffff));
		WriteWordWrapped(g, aDestWrap, "pets at the same time!", -1, 0);

		if (aWhat == 5)
		{
			Font* aPrevFont = g->GetFont();
			g->SetFont(FONT_TINY);
			g->DrawString("*", aDestWrap.mX + 223, aDestWrap.mY + 5);
			aDestWrap.mY += 55;
			g->DrawString("*", aDestWrap.mX + 60, aDestWrap.mY + 6);
			WriteWordWrapped(g, aDestWrap, "Only applies to Virtual Tank.\nVoid where prohibited.", -1, 0);
			aDestWrap.mY -= 35;
			g->SetFont(aPrevFont);
		}
		else
			aDestWrap.mY += 35;

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffaa));
		WriteWordWrapped(g, aDestWrap, anIdkStr, -1, 0);
	}
}

void Sexy::BonusScreen::DrawMisc3(Graphics* g)
{
	Rect aDest1(m0x94.mX + 20, m0x94.mY + 20, m0x94.mWidth - 40, m0x94.mHeight); // loc68,64,60,5c
	int aXUnk3 = m0x94.mX + 30; // loc34
	int aWidthUnk1 = m0x94.mWidth-70; // loc58
	int aWidthUnk2 = m0x94.mWidth-60+ aDest1.mX; // loc54
	if (mTitle.length() > 0)
	{
		g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
		g->SetColor(Color(0xffc800));
		int anY = aDest1.mY + WriteWordWrapped(g, aDest1, mTitle, -1, 0);
		g->SetColor(Color(0xffffff));
		g->FillRect(aXUnk3, anY, aWidthUnk1, 1);
		g->SetColor(Color(0));
		g->FillRect(aXUnk3, anY+1, aWidthUnk1, 1);
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffff));
		g->DrawString(mCurScore1, aXUnk3, anY+20);
		g->SetColor(Color(0xffff00));

		int aStrWdth = g->GetFont()->StringWidth(mCurScore2);
		g->DrawString(mCurScore2, aWidthUnk2 - aStrWdth, anY + 20);
		g->SetColor(Color(0xffffff));
		g->DrawString(mBestScore1, aXUnk3, anY + 40);
		g->SetColor(Color(0xffff00));
		aStrWdth = g->GetFont()->StringWidth(mBestScore2);
		g->DrawString(mBestScore2, aWidthUnk2 - aStrWdth, anY + 40);
	}
	int aYOff = (mTitle.length() > 0 ? 80 : 0) + 30 + m0x94.mY;
	aDest1.mY = aYOff;
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color(0xffc800));
	aYOff += WriteWordWrapped(g, aDest1, "Shells", -1, 0);
	g->SetColor(Color(0xffffff));
	g->FillRect(aXUnk3, aYOff, aWidthUnk1, 1);
	g->SetColor(Color(0));
	g->FillRect(aXUnk3, aYOff+1, aWidthUnk1, 1);
	int aYIdk = aYOff + 20;
	aDest1.mY = aYIdk;
	g->SetColor(Color(0xffffff));
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->DrawString(mRewardString, aXUnk3, aYIdk);
	SexyString aRewardStr = CommaSeperate(m0xc0);
	g->SetColor(Color(0xffff00));
	g->DrawString(aRewardStr, aWidthUnk2 - g->GetFont()->StringWidth(aRewardStr), aYIdk);

	g->SetColor(Color(0xffffff));
	g->DrawString("New Balance", aXUnk3, aYOff + 40);

	SexyString aUnkStr = CommaSeperate(m0xc4);
	g->SetColor(Color(0xffff00));
	g->DrawString(aUnkStr, aWidthUnk2 - g->GetFont()->StringWidth(aUnkStr), aYOff + 40);

	if (mTitle.length() == 0)
	{
		aDest1.mY = 360;
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffaa));
		WriteWordWrapped(g, aDest1, "Keep playing to earn\nmore shells!", -1, 0);
	}
}

void Sexy::BonusScreen::Init()
{
	mTitle = "Level Info";
	mRewardString = "Bonus Reward";

	if (mApp->mBoard == nullptr)
	{
		static int sBonusScreenType = 0;

		switch (sBonusScreenType % 4)
		{
		case 0:
			mCurScore1 = "Level Time";
			mCurScore2 = "23:10";
			mBestScore1 = "Your Best Time";
			mBestScore2 = "15:23";
		case 1:
			mTitle = "Surviving Pets";
			mCurScore1 = "Your Score";
			mCurScore2 = "3";
			mBestScore1 = "Your Best Score";
			mBestScore2 = "8";
		case 2:
			mCurScore1 = "Your Score";
			mCurScore2 = "25,000";
			mBestScore1 = "Your Best Score";
			mBestScore2 = "32,125";
		case 3:
			mTitle = "";
		}
		sBonusScreenType++;
		return;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE)
	{
		if (mApp->mBoard->mTank != 5)
		{
			mBonusReward = mApp->mBoard->m0x3f4;
			if (mApp->mBoard->mLevel > 5)
				mTitle = "";
			mCurScore1 = "Level Time";
			mBestScore1 = "Your Best Time";
			mCurScore2 = GetPlayTimeString(mApp->mBoard->m0x3fc);
			mBestScore2 = GetPlayTimeString(mApp->mCurrentProfile->GetAdventureScore(mApp->mBoard->mTank, mApp->mBoard->mLevel));
		}
		else
		{
			int aVal = mApp->mCurrentProfile->mCyraxNum - 1;
			if (aVal < 1)
				aVal = 1;
			mBonusReward = aVal * 5000;
			if (mBonusReward > 25000)
				mBonusReward = 25000;
			mTitle = "Surviving Pets";
			mCurScore1 = "Your Score";
			mBestScore1 = "Your Best Score";
			mCurScore2 = StrFormat("%d", mApp->mBoard->mFishTypePetList->size() + mApp->mBoard->mOtherTypePetList->size());
			mBestScore2 = StrFormat("%d", mApp->mCurrentProfile->GetAdventureScore(5, 1));
		}
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		mBonusReward = mApp->mBoard->mMoney * 5 / 100;
		mRewardString = StrFormat("%d%% Bonus Award", mBonusReward);
		mCurScore1 = "Your Score";
		mBestScore1 = "Your Best Score";
		mCurScore2 = CommaSeperate(mApp->mBoard->mMoney);
		mBestScore2 = CommaSeperate(mApp->mCurrentProfile->GetTimeTrialScore(mApp->mBoard->mTank));
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
	{
		switch (mApp->mBoard->mTank)
		{
		case 0:
			mBonusReward = 2000;
			break;
		case 1:
			mBonusReward = 5000;
			break;
		case 2:
			mBonusReward = 10000;
			break;
		case 3:
			mBonusReward = 20000;
			break;
		}
		mCurScore1 = "Level Time";
		mBestScore1 = "Your Best Time";
		mCurScore2 = GetPlayTimeString(mApp->mBoard->m0x3fc);
		mBestScore2 = GetPlayTimeString(mApp->mCurrentProfile->GetChallengeScore(mApp->mBoard->mTank));
	}
	mApp->mCurrentProfile->AddShells(mBonusReward);
}

void Sexy::BonusScreen::ConfirmPurchase()
{
	mApp->StopMusic();
	mApp->PlayMusic(2, 54);

	if (mShellImage != nullptr)
		delete mShellImage;
	mShellImage = nullptr;

	UserProfile* aCurUser = mApp->mCurrentProfile;
	switch (aCurUser->mBonusItemId)
	{
	case 0:
		aCurUser->UnlockPet(GameObject::PET_BRINKLEY, true);
		break;
	case 1:
		aCurUser->UnlockPet(GameObject::PET_NOSTRADAMUS, true);
		break;
	case 2:
		aCurUser->UnlockPet(GameObject::PET_STANLEY, true);
		break;
	case 3:
		aCurUser->UnlockPet(GameObject::PET_WALTER, true);
		break;
	}
	if (aCurUser->mBonusItemId >= 0 && aCurUser->mBonusItemId < 4)
	{
		mShellImage = new MemoryImage(mApp);
		mShellImage->Create(IMAGE_EGGSHARDS->mWidth, IMAGE_EGGSHARDS->mHeight);
		mShellImage->SetImageMode(true, true);
		mShellImage->mNumCols = 8;
		Graphics gImg(mShellImage);
		Image* anImg = GetImageById(aCurUser->mBonusItemId + IMAGE_EGGBRINKLEY_ID);

		int anXPos = 2;
		while (anXPos < 802)
		{
			gImg.DrawImage(anImg, anXPos, 2);
			anXPos += 100;
		}

		int aNumOfPixels = mShellImage->mHeight * mShellImage->mWidth;
		ulong* aBitsOriginal = ((MemoryImage*)IMAGE_EGGSHARDS)->GetBits();
		ulong* aBitsMade = mShellImage->GetBits();
		for (int i = 0; i < aNumOfPixels; i++)
		{
			*aBitsMade = *aBitsMade & 0xffffff | *aBitsOriginal & 0xff000000;
			aBitsOriginal++;
			aBitsMade++;
		}
		m0xd4 = 1;
	}

	if (aCurUser->mBonusItemId >= 4)
	{
		int aVal = (aCurUser->mBonusItemId != 4) ? 7 : 4;
		if (aCurUser->m0xb4 < aVal)
			aCurUser->m0xb4 = aVal;
	}

	aCurUser->mBonusItemId++;
	aCurUser->AddShells(-m0xd0);
	m0xe0 = true;
	mUnkButton1->SetVisible(false);
	mApp->SaveCurrentUserData();
}

void Sexy::BonusScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
