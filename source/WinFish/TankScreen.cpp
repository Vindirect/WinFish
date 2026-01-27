#include "TankScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::TankScreen::TankScreen(WinFishApp* theApp)
{
	for (int i = 0; i < 8; i+=2)
	{
		m0xa8[i] = 0;
		m0xa8[i + 1] = 0;
	}
	mApp = theApp;
	mX = 0;
	mY = 0;
	mWidth = theApp->mWidth;
	mHeight = theApp->mHeight;
	mMenuButton = MakeDialogButton2(99, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);
	mStoriesButton = MakeDialogButton(4, this, "Stories", FONT_JUNGLEFEVER12OUTLINE);
	mStoriesButton->Resize(245, 247, 150, mStoriesButton->mHeight);
	mStoriesButton->SetColor(0, Color(255, 240, 0));
	if (mApp->mCurrentProfile->m0x7c == 0 || mApp->mGameMode != WinFishApp::GAMEMODE_CHALLENGE)
		mStoriesButton->mVisible = false;
	m0xa8[0] = 70;
	m0xa8[1] = 71;
	m0xa8[2] = 350;
	m0xa8[3] = 71;
	m0xa8[4] = m0xa8[0];
	m0xa8[6] = m0xa8[2];
	m0xa8[5] = mStoriesButton->mVisible ? 284 : 264;
	m0xa8[7] = mStoriesButton->mVisible ? 284 : 264;

	for (int i = 0; i < 4; i++)
	{
		mTankButtons[i] = MakeDialogButton2(i, this, StrFormat("Tank %d", i + 1), IMAGE_MAINBUTTON);
		mTankButtons[i]->SetFont(FONT_JUNGLEFEVER12OUTLINE);
		mTankButtons[i]->Resize(m0xa8[i * 2] + 14, m0xa8[i * 2 + 1] + 134, 193, mTankButtons[i]->mHeight);
	}
}

Sexy::TankScreen::~TankScreen()
{
	for (int i = 0; i < 4; i++)
		if (mTankButtons[i])
			delete mTankButtons[i];
	if (mMenuButton)
		delete mMenuButton;
	if (mStoriesButton)
		delete mStoriesButton;
}

void Sexy::TankScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	for (int i = 0; i < 4; i++)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
		{
			if (mApp->mCurrentProfile->mTank >= i + 2 || mApp->mCurrentProfile->mFinishedGame)
				mTankButtons[i]->SetDisabled(false);
			else
			{
				mTankButtons[i]->SetDisabled(true);
				mTankButtons[i]->mComponentImage = nullptr;
				mTankButtons[i]->mFrameNoDraw = true;
				mTankButtons[i]->SetColor(0, Color::White);
			}
		}
		else
		{
			if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && i > 0 && !mApp->mCurrentProfile->m0x78[i-1])
			{
				mTankButtons[i]->SetDisabled(true);
				mTankButtons[i]->mComponentImage = nullptr;
				mTankButtons[i]->mFrameNoDraw = true;
				mTankButtons[i]->SetColor(0, Color::White);
			}
			else
				mTankButtons[i]->SetDisabled(false);
		}
		theWidgetManager->AddWidget(mTankButtons[i]);
	}


	theWidgetManager->AddWidget(mStoriesButton);
}

void Sexy::TankScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	for (int i = 0; i < 4; i++)
		theWidgetManager->RemoveWidget(mTankButtons[i]);
	theWidgetManager->RemoveWidget(mStoriesButton);
}

void Sexy::TankScreen::Update()
{
	MarkDirty();
}

void Sexy::TankScreen::Draw(Graphics* g)
{
	g->DrawImageBox(Rect(-5, -5, 650, 490), IMAGE_SCREENBACK);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 13, 130, 614);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 13, mStoriesButton->mVisible ? 350 : 320, 614);
	DrawVertCrease(g, IMAGE_VERTCREASE, 180, 100, 369);
	DrawVertCrease(g, IMAGE_VERTCREASE, 460, 100, 369);
	DrawBolts(g, 50, 110, true, true);
	DrawBolts(g, 30, 150, true, true);
	DrawBolts(g, 320, 100, false, true);
	DrawBolts(g, 320, 158, false, true);
	g->DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);
	g->DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);

	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(255, 200, 0));
	SexyString aStr;
	if(mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
		aStr = "Time Trial";
	else if(mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
		aStr = "Challenge";
	else
		aStr = "Choose a Tank";
	WriteCenteredLine(g, 25, aStr);
	
	g->SetColor(Color::White);
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
		aStr = "How much money can you earn before time runs out?";
	else if(mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
		aStr = "Can you fend off the increasingly hungry aliens?";
	else
		aStr = "";

	if(!aStr.empty())
		WriteCenteredLine(g, 60, aStr);

	for (int i = 0; i < 4; i++)
	{
		g->DrawImage(IMAGE_TANKCHOOSER, m0xa8[i * 2], m0xa8[i * 2 + 1]);
		if (!mTankButtons[i]->mDisabled)
		{
			int anId = i;
			if (i > 1)
				anId = i + 1;

			g->DrawImage(GetImageById(anId + IMAGE_TANKBACKDROP1_ID), m0xa8[i * 2] + 16, m0xa8[i * 2 + 1] + 10);
		}
		else
		{
			Rect aDrawStrRect = Rect(m0xa8[i * 2] + 25, m0xa8[i * 2 + 1] + 43, 170, 200);
			g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
			g->SetColor(Color::White);
			if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
				WriteWordWrapped(g, aDrawStrRect, StrFormat("Complete Tank %d\nin Adventure Mode\nto Unlock this Tank", i + 1), -1, 0);
			else if(mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
				WriteWordWrapped(g, aDrawStrRect, StrFormat("Complete Tank %d\nin Challenge Mode\nto Unlock this Tank", i + 1), -1, 0);
		}
	}
}

void Sexy::TankScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::TankScreen::ButtonDepress(int theId)
{
	if (theId == 99)
	{
		mApp->RemoveTankScreen();
		mApp->SwitchToGameSelector();
	}
	else if (theId < 4)
	{
		mApp->RemoveTankScreen();
		mApp->mTankGameModeChoose = theId+1;
		mApp->SwitchToBoard(true, true);
	}
	else if (theId == 4)
	{
		mApp->RemoveTankScreen();
		mApp->SwitchToStoryScreen(-1);
	}
}

void Sexy::TankScreen::DrawBolts(Graphics* g, int xOffset, int yOffset, bool drawRight, bool drawBottom)
{
	g->DrawImage(IMAGE_BOLT, xOffset, yOffset);

	if (drawRight)
		g->DrawImage(IMAGE_BOLT, 640 - xOffset, yOffset);

	if (drawBottom)
		g->DrawImage(IMAGE_BOLT, xOffset, 480 - yOffset);

	if (drawRight && drawBottom)
		g->DrawImage(IMAGE_BOLT, 640 - xOffset, 480 - yOffset);
}
