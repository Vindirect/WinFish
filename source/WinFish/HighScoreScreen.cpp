#include "HighScoreScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "BubbleMgr.h"
#include "HighScoreMgr.h"
#include "Res.h"

#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include <SexyAppFramework/D3DInterface.h>
#include <SexyAppFramework/DDInterface.h>

using namespace Sexy;

int Sexy::gHighScoreScreenPage = 1;

const double PI = 3.141590118408203;

Sexy::HighScoreScreen::HighScoreScreen(WinFishApp* theApp)
{
	mApp = theApp;

	mBubbleMgr = new BubbleMgr();
	mBubbleMgr->mBubbleBounds = Rect(0, 60, 640, 420);
	mBubbleMgr->mBubbleFishBounds = Rect(0, 80, 640, 350);
	mBubbleMgr->SetBubbleConfig(20, 3);
	mBubbleMgr->SetBubbleFishConfig(3, 3);
	mBubbleMgr->UpdateALot();

	mPage = gHighScoreScreenPage;
	mMenuButton = MakeDialogButton2(4, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);

	mAdvButton = MakeDialogButton2(PAGE_ADV, this, "Adventure", IMAGE_MAINBUTTON);

	mTimeTrialButton = MakeDialogButton2(PAGE_TIME, this, "Time Trial", IMAGE_MAINBUTTON);

	mChallengeButton = MakeDialogButton2(PAGE_CHAL, this, "Challenge", IMAGE_MAINBUTTON);

	mPersonalButton = MakeDialogButton2(PAGE_PERSONAL, this, "Personal", IMAGE_MAINBUTTON);

	mAdvButton->Resize(20, 470 - mAdvButton->mHeight, 120, mAdvButton->mHeight);
	mTimeTrialButton->Layout(LAY_SameBottom | LAY_Right | LAY_SameHeight | LAY_SameWidth,mAdvButton, 40);
	mChallengeButton->Layout(LAY_SameBottom | LAY_Right | LAY_SameHeight | LAY_SameWidth, mTimeTrialButton, 40);
	mPersonalButton->Layout(LAY_SameBottom | LAY_Right | LAY_SameHeight | LAY_SameWidth, mChallengeButton, 40);
}

Sexy::HighScoreScreen::~HighScoreScreen()
{
	mPage = gHighScoreScreenPage;
	delete mBubbleMgr;
	delete mMenuButton;
	delete mAdvButton;
	delete mTimeTrialButton;
	delete mChallengeButton;
	delete mPersonalButton;
}

void Sexy::HighScoreScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mAdvButton);
	theWidgetManager->AddWidget(mTimeTrialButton);
	theWidgetManager->AddWidget(mChallengeButton);
	theWidgetManager->AddWidget(mPersonalButton);
}

void Sexy::HighScoreScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mAdvButton);
	theWidgetManager->RemoveWidget(mTimeTrialButton);
	theWidgetManager->RemoveWidget(mChallengeButton);
	theWidgetManager->RemoveWidget(mPersonalButton);
}

void Sexy::HighScoreScreen::Update()
{
	Widget::Update();
	mBubbleMgr->Update();
	MarkDirty();
}

void Sexy::HighScoreScreen::Draw(Graphics* g)
{
	int aLumin = 107;
	for (int i = 0; i < 480; i += 4)
	{
		g->SetColor(Color(mApp->HSLToRGB(151, 138, aLumin)));
		g->FillRect(0, i, mWidth, 4);
		if (--aLumin < 50)
			aLumin = 50;
	}
	mBubbleMgr->Draw(g);
	DrawWaves(g, 60, mUpdateCnt);

	SexyString aGameModeStr = "";
	switch (mPage)
	{
	case PAGE_ADV:
		aGameModeStr = "Adventure";
		break;
	case PAGE_TIME:
		aGameModeStr = "Time Trial";
		break;
	case PAGE_CHAL:
		aGameModeStr = "Challenge";
		break;
	case PAGE_PERSONAL:
		aGameModeStr = "Personal Records";
		DrawPersonalRecordsPage(g);
		break;
	default:
		break;
	}
	if (mPage != PAGE_PERSONAL)
		DrawGameModePage(g);

	g->SetFont(FONT_CONTINUUMBOLD14);
	g->SetColor(Color::White);
	DrawStringWithOutline(g, aGameModeStr, 320 - g->GetFont()->StringWidth(aGameModeStr) / 2, 60, FONT_CONTINUUMBOLD14OUTLINE, 0);
	g->DrawImageBox(Rect(0, 0, 640, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);

	g->DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);

	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(0xff, 200, 0));
	WriteCenteredLine(g, 25, "Hall of Fame");
}

void Sexy::HighScoreScreen::DrawGameModePage(Graphics* g)
{
	float aRadVal = 280;
	bool loc56 = false;

	if (mPage == PAGE_ADV && mApp->mCurrentProfile->mFinishedGame)
	{
		loc56 = true;
		aRadVal = 240;
	}

	bool isAccel = mApp->Is3DAccelerated();
	D3DInterface* anInterface = mApp->mDDInterface->mD3DInterface;
	HighScoreMgr* aMgr = mApp->mHighScoreMgr;

	float anYOffset = 0;
	double aRad = 0;
	int aTextY = 100;

	for (int aTank = 1; aTank <= 4; aTank++)
	{
		HighScoreList* aCurList = &aMgr->mChallengeScores[aTank-1];
		if (mPage == PAGE_TIME)
			aCurList = &aMgr->mTimeTrialScores[aTank-1];

		int aTankStrXOffset = 380 + ((aTank % 2) * -320);

		aTextY = 100;
		if (aTank > 2)
			aTextY = aRadVal;

		g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
		g->SetColor(Color(0xff, 0xff, 100));

		SexyString aTankStr = StrFormat("Tank %d", aTank);
		int aTankStrWdth = g->GetFont()->StringWidth(aTankStr);

		if (isAccel)
		{
			aRad = ((aTextY * 2) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
			float aSinVal = sinf(aRad);
			anYOffset = aSinVal * 3;
			SexyTransform2D aTransform2D;
			aTransform2D.Translate(0, anYOffset);
			anInterface->PushTransform(aTransform2D);
		}
		g->DrawString(aTankStr, (200 - aTankStrWdth) / 2 + aTankStrXOffset, aTextY);

		if (isAccel)
			anInterface->PopTransform();

		int aFontHght = g->GetFont()->GetHeight();
		aTextY += aFontHght;

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color::White);
		for (int aLevel = 1; aLevel <= 5; aLevel++)
		{
			if (isAccel)
			{
				aRad = ((aTextY * 2) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
				float aSinVal = sinf(aRad);
				anYOffset = aSinVal * 3;
				SexyTransform2D aTransform2D;
				aTransform2D.Translate(0, anYOffset);
				anInterface->PushTransform(aTransform2D);
			}

			SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextY);
			SexyString aStr = "n";

			HighScoreList::iterator it;

			if (aCurList->size() >= aLevel)
			{
				it = aCurList->begin();
				std::advance(it, aLevel - 1);
			}

			if (mPage != PAGE_ADV)
			{
				if (aCurList->size() >= aLevel)
					aStr = it->mUserName;
			}
			else
			{
				aStr = aMgr->GetPerLevelUserName(aTank, aLevel);
				g->DrawString(StrFormat("%d-%d", aTank, aLevel), aTankStrXOffset - 30, aTextY);
			}

			g->DrawString(aStr, aTankStrXOffset, aTextY);

			aStr = "s";
			if (mPage != PAGE_TIME && aCurList->size() >= aLevel)
				aStr = GetPlayTimeString(it->mScore);
			else if (aCurList->size() >= aLevel)
				aStr = StrFormat("%d", it->mScore);

			int aStrWdth = g->GetFont()->StringWidth(aStr);
			g->DrawString(aStr, aTankStrXOffset + (200 - aStrWdth), aTextY);

			aTextY += 15;

			if (isAccel)
				anInterface->PopTransform();
		}
	}

	if (!loc56)
		return;

	if (isAccel)
	{
		double aRadians01 = (mUpdateCnt*3.0 *PI)/180.0+12.740893;
		float aSinVal = sinf(aRadians01);
		float anYOffset = aSinVal * 3;
		SexyTransform2D aTransform2D;
		aTransform2D.Translate(0, anYOffset);
		anInterface->PushTransform(aTransform2D);
	}

	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color(0xff, 0xff, 100));
	SexyString aStr = "Tank 5 (Surviving Pets)";
	int aStrWdth = g->GetFont()->StringWidth(aStr);
	g->DrawString(aStr, 320 - aStrWdth / 2, 365);

	if (isAccel)
		anInterface->PopTransform();

	int anY01 = g->GetFont()->GetHeight() + 365;
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color::White);

	HighScoreList* aPerLvlList = aMgr->GetPerLevelScoresList(5, 1);

	for (int i = 0; i < 3 && i < aPerLvlList->size(); i++)
	{
		if (isAccel)
		{
			double aRadians02 = ((anY01*2) * PI) / 180.0 + (mUpdateCnt*3.0*PI)/180;
			float aSinVal = sinf(aRadians02);
			float anYOffset = aSinVal * 3;
			SexyTransform2D aTransform2D;
			aTransform2D.Translate(0, anYOffset);
			anInterface->PushTransform(aTransform2D);
		}
		SexyString aUserName = "-";
		int aScore = -1;

		HighScoreList::iterator it = aPerLvlList->begin();
		std::advance(it, i);
		aUserName = it->mUserName;
		aScore = it->mScore;

		SetVaryingBlueColorGraphics(g, mUpdateCnt, anY01);
		g->DrawString(aUserName, 220, anY01);

		SexyString aStr = StrFormat("%d / 18", aScore);
		int aStrWdth = g->GetFont()->StringWidth(aStr);
		g->DrawString(aStr, 420 - aStrWdth, anY01);
		anY01 += 15;

		if (isAccel)
			anInterface->PopTransform();
	}
}

void Sexy::HighScoreScreen::DrawPersonalRecordsPage(Graphics* g)
{
	bool isAccel = mApp->Is3DAccelerated();
	D3DInterface* anInterface = mApp->mDDInterface->mD3DInterface;
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color(255, 255, 100));
	if (isAccel)
	{
		double aRadians01 = (mUpdateCnt * 3.0 * PI) / 180.0 + 3.8397212;
		float aSinVal = sinf(aRadians01);
		float anYOffset = aSinVal * 3;
		SexyTransform2D aTransform2D;
		aTransform2D.Translate(0, anYOffset);
		anInterface->PushTransform(aTransform2D);
	}

	SexyString aStr = "Adventure";
	int aStrWdth = g->GetFont()->StringWidth(aStr);
	g->DrawString(aStr, 320 - aStrWdth / 2, 110);

	if (isAccel)
	{
		anInterface->PopTransform();
		double aRadians01 = (mUpdateCnt * 3.0 * PI) / 180.0 + 10.122902;
		float aSinVal = sinf(aRadians01);
		float anYOffset = aSinVal * 3;
		SexyTransform2D aTransform2D;
		aTransform2D.Translate(0, anYOffset);
		anInterface->PushTransform(aTransform2D);
	}

	aStr = "Time Trial";
	aStrWdth = g->GetFont()->StringWidth(aStr);
	g->DrawString(aStr, 173 - aStrWdth / 2, 290);

	aStr = "Challenge";
	aStrWdth = g->GetFont()->StringWidth(aStr);
	g->DrawString(aStr, 466 - aStrWdth / 2, 290);

	if (isAccel)
		anInterface->PopTransform();

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color(0xffffff));

	int aXPosText = 20;//loc5c
	double aTextYPos = 140.0;
	for (int aTankNum = 1; aTankNum <= 4; aTankNum++)
	{
		aTextYPos = 140.0; // fvar7
		for (int aLevelNum = 1; aLevelNum <= 5; aLevelNum++)
		{
			if (isAccel)
			{
				double aRadians01 = ((aTextYPos + aTextYPos) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
				float aSinVal = sinf(aRadians01);
				float anYOffset = aSinVal * 3.0;
				SexyTransform2D aTransform2D;
				aTransform2D.Translate(0, anYOffset);
				anInterface->PushTransform(aTransform2D);
			}
			SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
			g->SetColor(Color(0xffff88));

			g->DrawString(StrFormat("%d-%d", aTankNum, aLevelNum), aXPosText, aTextYPos);

			int aScore = mApp->mCurrentProfile->GetAdventureScore(aTankNum, aLevelNum);

			SexyString aScoreStr = "-----";
			if (aScore >= 0)
				aScoreStr = GetPlayTimeString(aScore);

			SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
			int aScoreStrWdth = g->GetFont()->StringWidth(aScoreStr);
			g->DrawString(aScoreStr, aXPosText + (100 - aScoreStrWdth), aTextYPos);

			aTextYPos += 15;

			if (isAccel)
				anInterface->PopTransform();
		}
		aXPosText += 166;
	}

	int aFinalBossScore = mApp->mCurrentProfile->GetAdventureScore(5, 1);

	if (aFinalBossScore >= 0)
	{
		aTextYPos += 5;
		if (isAccel)
		{
			double aRadians01 = ((aTextYPos + aTextYPos) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
			float aSinVal = sinf(aRadians01);
			float anYOffset = aSinVal * 3.0;
			SexyTransform2D aTransform2D;
			aTransform2D.Translate(0, anYOffset);
			anInterface->PushTransform(aTransform2D);
		}
		SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
		g->SetColor(Color(0xffff88));

		g->DrawString("Final Boss", 220, aTextYPos);

		SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
		g->DrawString(StrFormat("%d/18 Pets Saved", aFinalBossScore), 320, aTextYPos);

		if (isAccel)
			anInterface->PopTransform();
	}

	for (int i = 0; i < 2; i++)
	{
		aTextYPos = 315;
		double aTextXPos = i * 293 + 83;
		for (int aTankNum = 1; aTankNum <= 4; aTankNum++)
		{
			if (isAccel)
			{
				double aRadians01 = ((aTextYPos + aTextYPos) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
				float aSinVal = sinf(aRadians01);
				float anYOffset = aSinVal * 3.0;
				SexyTransform2D aTransform2D;
				aTransform2D.Translate(0, anYOffset);
				anInterface->PushTransform(aTransform2D);
			}

			SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
			g->SetColor(Color(0xffff88));

			g->DrawString(StrFormat("Tank %d", aTankNum), aTextXPos, aTextYPos);
			int aScore = -1;
			if (i == 0)
				aScore = mApp->mCurrentProfile->GetTimeTrialScore(aTankNum);
			else
				aScore = mApp->mCurrentProfile->GetChallengeScore(aTankNum);

			SexyString aScoreStr = "-----";
			if (aScore >= 0)
			{
				if (i == 0)
					aScoreStr = CommaSeperate(aScore);
				else
					aScoreStr = GetPlayTimeString(aScore);
			}

			SetVaryingBlueColorGraphics(g, mUpdateCnt, aTextYPos);
			int aStrWdth = g->GetFont()->StringWidth(aScoreStr);
			g->DrawString(aScoreStr, aTextXPos + 170 - aStrWdth, aTextYPos);

			aTextYPos += 15;

			if (isAccel)
				anInterface->PopTransform();
		}
	}
}

void Sexy::HighScoreScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::HighScoreScreen::ButtonDepress(int theId)
{
	if (theId == 4)
		mApp->RemoveHighScoreScreen();
	else if (theId < 4)
		mPage = theId;
}

