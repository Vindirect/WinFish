#include "HelpScreen.h"
#include "WinFishAppCommon.h"
#include "WinFishApp.h"
#include "BubbleMgr.h"
#include "Res.h"

#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include <SexyAppFramework/D3DInterface.h>
#include <SexyAppFramework/DDInterface.h>

using namespace Sexy;

int Sexy::gHelpScreenTab = 1;
const double PI = 3.141590118408203;

Sexy::HelpScreen::HelpScreen(WinFishApp* theApp, bool instructions)
{
	mApp = theApp;
	mBubbleMgr = new BubbleMgr();
	mContinueButton = MakeDialogButton(0, this, "Click Here To Continue", nullptr);
	mContinueButton->Resize(210, 415, 220, mContinueButton->mHeight);
	mContinueButton->SetColor(DialogButton::COLOR_LABEL, Color(255, 240, 0));

	mMenuButton = MakeDialogButton2(1, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);

	mPreviousButton = MakeDialogButton2(3, this, "Previous", IMAGE_MAINBUTTON);
	mPreviousButton->Resize(190, 430, 120, mPreviousButton->mHeight);

	mNextButton = MakeDialogButton2(2, this, "Next", IMAGE_MAINBUTTON);
	mNextButton->Layout(LAY_SameBottom | LAY_Right | LAY_SameHeight | LAY_SameWidth, mPreviousButton, 20);

	if (!instructions)
	{
		mPageIdx = gHelpScreenTab;
		mContinueButton->mVisible = false;
		if (mApp->mBoard)
		{
			mMenuButton->Resize(480, 4, 125, mMenuButton->mHeight);
			mMenuButton->mLabel = "Back to Game";
		}
		mBubbleMgr->mBubbleBounds = Rect(0, 80, 640, 400);
		mBubbleMgr->mBubbleFishBounds = Rect(0, 80, 640, 350);
	}
	else
	{
		mPageIdx = 0;
		mNextButton->mVisible = false;
		mPreviousButton->mVisible = false;
		mBubbleMgr->mBubbleBounds = Rect(0, 60, 640, 420);
	}
	m0xa8 = -1;
	mBubbleMgr->SetBubbleConfig(20, 3);
	mBubbleMgr->UpdateALot();
}

Sexy::HelpScreen::~HelpScreen()
{
	if(mPageIdx > 0)
		gHelpScreenTab = mPageIdx;
	if (mBubbleMgr)
		delete mBubbleMgr;
	if (mContinueButton)
		delete mContinueButton;
	if (mMenuButton)
		delete mMenuButton;
	if (mNextButton)
		delete mNextButton;
	if (mPreviousButton)
		delete mPreviousButton;
}

void Sexy::HelpScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mContinueButton);
	theWidgetManager->AddWidget(mNextButton);
	theWidgetManager->AddWidget(mPreviousButton);
}

void Sexy::HelpScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mContinueButton);
	theWidgetManager->RemoveWidget(mNextButton);
	theWidgetManager->RemoveWidget(mPreviousButton);
}

void Sexy::HelpScreen::Update()
{
	Widget::Update();

	if (mPageIdx != m0xa8)
	{
		m0xa8 = mPageIdx;
		if (mPageIdx == 8)
		{
			mBubbleMgr->SetBubbleFishConfig(3, 3);
		}
		else
		{
			mBubbleMgr->SetBubbleFishConfig(0, 0);

			// Fish Scatter
			std::list<BubbleFish>::iterator itFish = mBubbleMgr->mFishList.begin();
			while (itFish != mBubbleMgr->mFishList.end())
			{
				BubbleFish* aFish = &(*itFish);
				if (mBubbleMgr->mBubbleFishBounds.mWidth / 2 + mBubbleMgr->mBubbleFishBounds.mX <= aFish->mX)
				{
					if (aFish->mVX < 0.0)
						aFish->mTurnTimer = 5;
					aFish->mVX = 20.0;
				}
				else
				{
					if (aFish->mVX > 0.0)
						aFish->mTurnTimer = 5;
					aFish->mVX = -20.0;
				}

				++itFish;
			}
		}
	}
	mBubbleMgr->Update();
	MarkDirty();
}

void Sexy::HelpScreen::Draw(Graphics* g)
{
	if (mPageIdx == 0)
		g->DrawImageBox(Rect(-5, -5, 650, 490), IMAGE_SCREENBACK);
	else
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
		DrawWaves(g, 80, mUpdateCnt);
		if (mPageIdx != 8)
		{
			Rect aTabRect1 = GetVertTabRect(0);
			Rect aTabRect2 = GetVertTabRect(1);
			Rect aTabRect4 = GetVertTabRect(3);
			int aYStartOffset = (aTabRect2.mY - aTabRect1.mY - aTabRect1.mHeight) / 2;

			Rect aDrawRect = Rect(aTabRect1.mX, aTabRect1.mY - aYStartOffset,
				aTabRect1.mWidth, aTabRect4.mY + aTabRect4.mHeight + aYStartOffset - aTabRect1.mY + aYStartOffset);

			g->SetColor(Color(0, 0, 0, 30));
			g->FillRect(aDrawRect);
			g->SetColor(Color(0x88aaaa));
			g->DrawRect(aDrawRect);
			for (int i = 0; i < 3; i++)
			{
				Rect aTabRect = GetVertTabRect(i);
				g->FillRect(aTabRect.mX,aTabRect.mHeight + aTabRect.mY + aYStartOffset,aTabRect.mWidth,1);
			}
		}
	}

	switch (mPageIdx)
	{
	case 0:
		DrawInstructionsPage(g);
		break;
	case 1:
		DrawTheBasicsPage(g);
		break;
	case 2:
		DrawUpgradesPage(g);
		break;
	case 3:
		DrawPetsPage(g);
		break;
	case 4:
		DrawGameModesPage(g);
		break;
	case 5:
		DrawVirtualTankPage(g);
		break;
	case 6:
		DrawVirtualTankFishPage(g);
		break;
	case 7:
		DrawConfigVirtualTankPage(g);
		break;
	case 8:
		DrawCreditsPage(g);
		break;
	}
	
	SexyString aTabName = "";
	switch (mPageIdx)
	{
	case 1:
		aTabName = "THE BASICS";
		break;
	case 2:
		aTabName = "UPGRADES";
		break;
	case 3:
		aTabName = "PETS";
		break;
	case 4:
		aTabName = "GAME MODES";
		break;
	case 5:
		aTabName = "VIRTUAL TANK";
		break;
	case 6:
		aTabName = "VIRTUAL TANK FISH";
		break;
	case 7:
		aTabName = "CONFIGURING VIRTUAL TANK";
		break;
	case 8:
		aTabName = "CREDITS";
		break;
	}

	if (mPageIdx != 0)
	{
		g->SetFont(FONT_CONTINUUMBOLD14);
		g->SetColor(Color::White);
		DrawStringWithOutline(g, aTabName, 320 - FONT_CONTINUUMBOLD14->StringWidth(aTabName) / 2, 60, FONT_CONTINUUMBOLD14OUTLINE, 0);

		g->SetColor(Color::White);
		SexyString aTabPage = StrFormat("Page %d of 8", mPageIdx);
		DrawStringWithOutline(g, aTabPage, 320 - FONT_CONTINUUMBOLD14->StringWidth(aTabPage) / 2, 80,
			FONT_CONTINUUMBOLD14OUTLINE, 0);
	}

	g->DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);
	g->DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);

	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(0xff, 200, 0, 0xff));
	SexyString aTitleString = "Instructions";
	if (mPageIdx != 0)
		aTitleString = "Insaniquarium Help";

	WriteCenteredLine(g, 25, aTitleString);
}

Rect Sexy::HelpScreen::GetVertTabRect(int theTabNum)
{
	return Rect(10, theTabNum*65 + 160, 620, 50);
}

Rect Sexy::HelpScreen::GetLeftRectVertTab(int theTabNum, int theYWidth)
{
	Rect aRec = GetVertTabRect(theTabNum);
	return Rect(22, aRec.mY + theYWidth, 75, aRec.mHeight + theYWidth *-2);
}

void Sexy::HelpScreen::CircleAroundCursor(int theUpdateCnt, int theX, int theY, int& theDestX, int& theDestY)
{
	double anAngleY = theUpdateCnt * 5.0 * PI / 180.0;
	float aSinVal = sinf(anAngleY) * 10.0;
	theDestY = theY + aSinVal;

	double anAngleX = theUpdateCnt * 2.0 * PI / 180.0;
	float aCosVal = cosf(anAngleX) * 10.0;
	theDestX = theX + aCosVal;
}

void Sexy::HelpScreen::DrawTabTitle(Graphics* g, SexyString theLine, SexyString theLine2)
{
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color(0xffffaa));
	WriteWordWrapped(g, Rect(10, 110, 620, 0), theLine, -1, 0);
	WriteWordWrapped(g, Rect(10, 125, 620, 0), theLine2, -1, 0);
}

void Sexy::HelpScreen::DrawVertTabText(Graphics* g, int theTabIdx,SexyString theLine, SexyString theLine2, int theHeightDrawOffset)
{
	Rect aTabRect = GetVertTabRect(theTabIdx);
	Rect aRect = GetLeftRectVertTab(theTabIdx, theHeightDrawOffset);
	g->SetColor(Color(0x60000000));
	g->FillRect(aRect);
	g->SetColor(Color(0x316584));
	g->DrawRect(aRect);
	g->SetColor(Color(0xffff88));
	g->SetFont(FONT_CONTINUUMBOLD14);
	
	// This is hidden for some reason, probably was debug thing
	DrawStringWithOutline(g, StrFormat("%d.", theTabIdx+1), -100, aTabRect.mY, FONT_CONTINUUMBOLD14OUTLINE, 0);

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	SetVaryingBlueColorGraphics(g, mUpdateCnt, aTabRect.mY + 20);
	g->DrawString(theLine, 120, aTabRect.mY + 20);
	SetVaryingBlueColorGraphics(g, mUpdateCnt, aTabRect.mY + 35);
	g->DrawString(theLine2, 120, aTabRect.mY + 35);
}

void Sexy::HelpScreen::DrawInstructionsPage(Graphics* g)
{
	DrawInstrLeftPart(g, Rect(30, 70, 185, 320));
	DrawInstrMiddlePart(g, Rect(227, 70, 185, 320));
	DrawInstrRightPart(g, Rect(424, 70, 185, 320));
	DrawHorzCrease(g, IMAGE_HORZCREASE, 13, 230, 23);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 209, 230, 24);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 406, 230, 24);
	DrawHorzCrease(g, IMAGE_HORZCREASE, 603, 230, 24);

	DrawVertCrease(g, IMAGE_VERTCREASE, 120, 33, 40);
	DrawVertCrease(g, IMAGE_VERTCREASE, 515, 33, 40);
	DrawVertCrease(g, IMAGE_VERTCREASE, 120, 386, 83);
	DrawVertCrease(g, IMAGE_VERTCREASE, 515, 386, 83);

	g->DrawImage(IMAGE_BOLT, 105, 53);
	g->DrawImage(IMAGE_BOLT, 135, 53);
	g->DrawImage(IMAGE_BOLT, 500, 53);
	g->DrawImage(IMAGE_BOLT, 530, 53);

	g->DrawImage(IMAGE_BOLT, 105, 406);
	g->DrawImage(IMAGE_BOLT, 135, 406);
	g->DrawImage(IMAGE_BOLT, 500, 406);
	g->DrawImage(IMAGE_BOLT, 530, 406);
}

void Sexy::HelpScreen::DrawInstrLeftPart(Graphics* g, Rect& theRect)
{
	g->DrawImageBox(theRect, IMAGE_FISHBOX);

	double aRadianVal = mUpdateCnt * 5 * PI / 180.0;
	float aVal = sinf(aRadianVal) * 3.0;
	int anObjY = theRect.mY + 120 + aVal;

	int anAnimTimer = mUpdateCnt % 115;

	bool doSwap = ((mUpdateCnt / 115) % 2);

	int aFoodX = InterpolateInt(theRect.mX + 50, theRect.mWidth + theRect.mX - 50, anAnimTimer, 30, doSwap);
	if (anAnimTimer > 39 && anAnimTimer < 85)
	{
		int aFoodY = InterpolateInt(theRect.mY + 30, anObjY, anAnimTimer - 40, 45, false);
		g->DrawImageCel(IMAGE_FOOD, aFoodX - 10, aFoodY - 20, (mUpdateCnt / 4) % 10, 0);
	}

	Rect anAnimSrcRect = Rect(((mUpdateCnt/2)%10) * 80, 0, 80, 80);
	Image* anImg = IMAGE_SMALLSWIM;
	bool doMirror = doSwap;
	if (anAnimTimer > 44)
	{
		if (anAnimTimer < 65)
		{
			anAnimSrcRect.mX = ((anAnimTimer - 45) / 2) * 80;
			anImg = IMAGE_SMALLTURN;
		}
		else 
		{
			doMirror = !doSwap;
			if (anAnimTimer > 74 && anAnimTimer < 95)
			{
				anAnimSrcRect.mX = ((anAnimTimer - 75) / 2) * 80;
				anImg = IMAGE_SMALLEAT;
			}
		}
	}
	int aFishX = InterpolateInt(theRect.mX + 50 + 15, theRect.mWidth + theRect.mX - 50 - 10, anAnimTimer - 45, 40, doSwap);
	g->DrawImageMirror(anImg, aFishX -37, anObjY-40, anAnimSrcRect, doMirror);

	if (anAnimTimer < 40 || (doMirror = true, anAnimTimer > 59))
		doMirror = false;

	DrawCursor(g, aFoodX, theRect.mY + 30, doMirror);

	SexyString aStrLines[3] = { "Click on the tank" , "to drop food" , "for your fish." };
	SexyString aStrTitle = "FEED ME";
	DrawInstrText(g, theRect, aStrTitle, aStrLines, 3);
}

void Sexy::HelpScreen::DrawInstrMiddlePart(Graphics* g, Rect& theRect)
{
	g->DrawImageBox(theRect, IMAGE_FISHBOX);
	int aCounter = mUpdateCnt % 40;
	aCounter -= 20;
	int anX = theRect.mX + theRect.mWidth / 2;
	int anY = theRect.mY + 80;

	double aRadianVal = mUpdateCnt * 3.0 * PI / 180.0;
	float aVal = sinf(aRadianVal) * 5.0;

	int aAroundCAlX = 0;
	int aAroundCAlY = 0;
	CircleAroundCursor(mUpdateCnt, anX, anY, aAroundCAlX, aAroundCAlY);
	int anOldClickX = 0;
	int anOldClickY = 0;
	CircleAroundCursor(mUpdateCnt - aCounter, anX, anY, anOldClickX, anOldClickY);

	int anXWOffset = anX - 80;

	Rect aSrcRect = Rect(((mUpdateCnt / 3) % 10) * 160, 0, 160, 160);
	g->DrawImage(IMAGE_SYLV, anXWOffset, theRect.mY + 20 + aVal, aSrcRect);

	if (aCounter >= 0)
	{
		if (aCounter < 10)
		{
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			g->SetColorizeImages(true);
			g->SetColor(Color(255, 255, 255, (10 - aCounter) * 10));
			g->DrawImage(IMAGE_SYLV, anXWOffset, theRect.mY + 20 + aVal, aSrcRect);
			g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
			g->SetColorizeImages(false);
		}
		if (aCounter < 20)
		{
			g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
			g->SetColor(Color::White);
			g->DrawLine(anOldClickX - 3, anOldClickY - 3, anOldClickX - 8, anOldClickY - 8);
			g->DrawLine(anOldClickX + 3, anOldClickY + 3, anOldClickX + 8, anOldClickY + 8);
			g->DrawLine(anOldClickX - 3, anOldClickY + 3, anOldClickX - 8, anOldClickY + 8);
			g->DrawLine(anOldClickX + 3, anOldClickY - 3, anOldClickX + 8, anOldClickY - 8);
			g->DrawLine(anOldClickX - 3, anOldClickY, anOldClickX - 8, anOldClickY);
			g->DrawLine(anOldClickX + 3, anOldClickY, anOldClickX + 8, anOldClickY);
			g->DrawLine(anOldClickX, anOldClickY + 3, anOldClickX, anOldClickY + 8);
			g->DrawLine(anOldClickX, anOldClickY - 3, anOldClickX, anOldClickY - 8);
		}
	}

	DrawCursor(g, aAroundCAlX, aAroundCAlY, false);

	SexyString aTitleStr = "FEAR ME";
	SexyString aLinesStr[2] = { "Use the mouse to" , "zap Aliens!" };
	DrawInstrText(g, theRect, aTitleStr, aLinesStr, 2);
}

void Sexy::HelpScreen::DrawInstrRightPart(Graphics* g, Rect& theRect)
{
	g->DrawImageBox(theRect, IMAGE_FISHBOX);
	g->DrawImage(IMAGE_EGGPIECES, (theRect.mWidth-IMAGE_EGGPIECES->mWidth) / 2 + theRect.mX, theRect.mY + 80);
	SexyString aTitleStr = "FIND ME";
	SexyString aLines[4] = { "Collect all 3 pieces" ,"of the egg to" ,"advance a level and" ,"gain a new pet!" };
	DrawInstrText(g, theRect, aTitleStr, aLines, 4);
}

void Sexy::HelpScreen::DrawCursor(Graphics* g, int theX, int theY, bool doClick)
{
	if (doClick)
	{
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color::White);
		g->DrawLine(theX - 3, theY - 3, theX - 8, theY - 8);
		g->DrawLine(theX + 3, theY + 3, theX + 8, theY + 8);
		g->DrawLine(theX - 3, theY + 3, theX - 8, theY + 8);
		g->DrawLine(theX + 3, theY - 3, theX + 8, theY - 8);
		g->DrawLine(theX - 3, theY, theX - 8, theY);
		g->DrawLine(theX + 3, theY, theX + 8, theY);
		g->DrawLine(theX, theY + 3, theX, theY + 8);
		g->DrawLine(theX, theY - 3, theX, theY - 8);
	}
	g->DrawImage(IMAGE_CURSOR_POINTER, theX - IMAGE_CURSOR_POINTER->mWidth / 2, theY - IMAGE_CURSOR_POINTER->mHeight / 2);
}

void Sexy::HelpScreen::DrawInstrText(Graphics* g, Rect& theRect, SexyString& theTitle, SexyString* theLines, int theNumOfLines)
{
	g->SetFont(FONT_CONTINUUMBOLD14);
	g->SetColor(Color(0xffff00));

	int aStrWdth = g->GetFont()->StringWidth(theTitle);
	DrawStringWithOutline(g, theTitle, (theRect.mWidth / 2 + theRect.mX) - aStrWdth / 2 - 5, theRect.mHeight - 100 + theRect.mY, FONT_CONTINUUMBOLD14OUTLINE, 0);

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);

	for (int i = 0; i < theNumOfLines; i++)
	{
		g->SetColor(Color::White);

		SexyString& aLine = theLines[i];
		int aLineWdth = g->GetFont()->StringWidth(aLine);
		g->DrawString(aLine, theRect.mWidth / 2 + theRect.mX - aLineWdth/2, theRect.mHeight - 100 + theRect.mY + 25 + i*15);
	}
}

void Sexy::HelpScreen::DrawTheBasicsPage(Graphics* g)
{
	DrawTabTitle(g, "Drop food for your fish to make them grow, then defend them from evil aliens!", 
		"Collect coins to buy upgrades and Egg pieces.  Complete an Egg to level up!");
	DrawVertTabText(g, 0, "Fish turn green when hungry!  Click on the tank to drop food.",
		"Well fed fish grow larger, but unfed fish may starve!");
	DrawVertTabText(g, 1, "Fish will drop coins for you.  Click on the coins to earn",
		"money!  Bigger fish will drop more valuable coins.");
	DrawVertTabText(g, 2, "Aliens will attack your fish!  Click on them to shoot.  Clicking",
		"on the right side of the alien will make it move left, and so on.");
	DrawVertTabText(g, 3, "Click buttons at the top to buy new upgrades.  Buy 3 Egg"
	, "Pieces to finish the level and earn a new pet!");

	Rect aRect = GetLeftRectVertTab(0, 4);
	g->DrawImageCel(IMAGE_HUNGRYSWIM, aRect.mWidth / 2 - 40 + aRect.mX, aRect.mHeight / 2 - 40 + aRect.mY, mUpdateCnt/3%10, 0);

	aRect = GetLeftRectVertTab(1, 4);
	g->DrawImageCel(IMAGE_MONEY, aRect.mWidth / 2 - 36 + aRect.mX, aRect.mHeight / 2 - 36 + aRect.mY, mUpdateCnt / 2 % 10, 1);

	aRect = GetLeftRectVertTab(2, 4);
	double aRadx = mUpdateCnt * 2.0 * PI / 180.0;
	double aRady = mUpdateCnt * 4.0 * PI / 180.0;

	float anXOffset = cosf((float)aRadx) * 2.0;
	float anYOffset = sinf((float)aRady) * 3.0;

	float aFinalX = aRect.mX + (aRect.mWidth - IMAGE_HELPALIEN->mWidth) / 2.0 + anXOffset;
	float aFinalY = aRect.mY + (aRect.mHeight - IMAGE_HELPALIEN->mHeight) / 2.0 + anYOffset;

	g->DrawImage(IMAGE_HELPALIEN, aFinalX +2, aFinalY +3);

	aRect = GetLeftRectVertTab(3, 4);
	g->DrawImageCel(IMAGE_EGGPIECES, aRect.mWidth / 2 - 23 + aRect.mX, aRect.mHeight / 2 - 18 + aRect.mY, mUpdateCnt / 36 % 3);
}

void Sexy::HelpScreen::DrawUpgradesPage(Graphics* g)
{
	DrawTabTitle(g, "There are several upgrades you can purchase if you have enough money.",
		"Click on the buttons at the top of the screen to buy the ones you want.");
	DrawVertTabText(g, 0, "You can buy new fish for your tank!  Click on the appropriate",
		"button to buy a new Guppy or Carnivore, if you can afford it.");
	DrawVertTabText(g, 1, "You can upgrade the quality of your food pellets.  Better food",
		"will feed fish for a longer time and make them grow faster.");
	DrawVertTabText(g, 2, "You can also increase the number of food pellets you can",
		"drop at once by upgrading your food quantity.");
	DrawVertTabText(g, 3, "You can also increase the power of your anti-alien weapon."
		, "More powerful weapons kill aliens in fewer clicks!");

	Rect aRect = GetLeftRectVertTab(0, 4);
	g->DrawImageCel(IMAGE_SMALLSWIM, aRect.mWidth / 2 - 40 + aRect.mX, aRect.mHeight / 2 - 40 + aRect.mY, mUpdateCnt / 3 % 10, 0);

	aRect = GetLeftRectVertTab(1, 4);
	double aRadY = mUpdateCnt * 5.0 * PI / 180.0;
	float anYOffset = sinf(aRadY) * 3.0;
	float aFinalY = aRect.mY + aRect.mHeight/2 - 20  + anYOffset;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth/2-40, aFinalY, mUpdateCnt / 3 % 10, 0);
	double anYDiff = 30.017454;
	double aRadY2 = aRadY + anYDiff;
	float anYOffset2 = sinf(aRadY2) * 3.0;
	float aFinalY2 = aRect.mY + aRect.mHeight / 2 - 20 + anYOffset2;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth / 2 - 20, aFinalY2, mUpdateCnt / 4 % 10, 1);
	double aRadY3 = aRadY + anYDiff*2;
	float anYOffset3 = sinf(aRadY3) * 3.0;
	float aFinalY3 = aRect.mY + aRect.mHeight / 2 - 20 + anYOffset3;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth / 2, aFinalY3, mUpdateCnt / 3 % 10, 2);

	aRect = GetLeftRectVertTab(2, 4);
	aRadY = mUpdateCnt * 5.0 * PI / 180.0 + 60.0;
	anYOffset = sinf(0) * 3.0; // Incorrect
	aFinalY = aRect.mY + aRect.mHeight / 2 - 20 + anYOffset;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth / 2 - 40, aFinalY, mUpdateCnt / 3 % 10, 0);
	aRadY2 = aRadY + 0.08726639;
	anYOffset = sinf(aRadY2) * 3.0;
	aFinalY = aRect.mY + aRect.mHeight / 2 - 20 + anYOffset;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth / 2 - 20, aFinalY, (mUpdateCnt / 4 + 3) % 10, 0);
	aRadY3 = aRadY + aRadY + 0.08726639;
	anYOffset = sinf(aRadY3) * 3.0;
	aFinalY = aRect.mY + aRect.mHeight / 2 - 20 + anYOffset;
	g->DrawImageCel(IMAGE_FOOD, aRect.mX + aRect.mWidth / 2, aFinalY, (mUpdateCnt / 3 + 6) % 10, 0);

	aRect = GetLeftRectVertTab(3, 4);
	g->DrawImageCel(IMAGE_LASERUPGRADES, aRect.mWidth / 2 - 23 + aRect.mX, aRect.mHeight / 2 - 18 + aRect.mY, mUpdateCnt / 36 % 3);
}

void Sexy::HelpScreen::DrawPetsPage(Graphics* g)
{
	DrawTabTitle(g, "You can finish a level by collecting all three pieces of an Egg.  Click on the",
		"Egg Piece button in the control bar to buy an Egg Piece.");
	DrawVertTabText(g, 0, "Advancing to the next level makes your egg hatch.  This gives you",
		"a new pet!  Each pet has different powers to help you out.");
	DrawVertTabText(g, 1, "ITCHY the Swordfish is one of the pets that you can get.",
		"He will attack aliens that infiltrate your tank!");
	DrawVertTabText(g, 2, "STINKY the Snail is another pet that you can get.",
		"He roams the ground picking up stray coins for you!");
	DrawVertTabText(g, 3, "You can only bring 3 pets into a level. If you have more than 3"
		, "pets, you\'ll have to choose which 3 you want to have in the tank.");

	Rect aRect = GetLeftRectVertTab(0, 4);
	g->DrawImageCel(IMAGE_EGGPIECES, aRect.mWidth / 2 - 23 + aRect.mX, aRect.mHeight / 2 -18 + aRect.mY, 2);
	
	aRect = GetLeftRectVertTab(1, 4);
	g->DrawImageCel(IMAGE_SCL_ITCHY, aRect.mWidth / 2 - 30 + aRect.mX, aRect.mHeight / 2 -30 + aRect.mY, mUpdateCnt/4 % 10);
	
	aRect = GetLeftRectVertTab(2, 4);
	g->DrawImageCel(IMAGE_SCL_STINKY, aRect.mWidth / 2 - 30 + aRect.mX, aRect.mHeight / 2 -30 + aRect.mY, mUpdateCnt/4 % 10);
	
	aRect = GetLeftRectVertTab(3, 4);
	g->DrawImage(IMAGE_HELPPETCHOOSE, (aRect.mWidth- IMAGE_HELPPETCHOOSE->mWidth) / 2 + aRect.mX, (aRect.mHeight- IMAGE_HELPPETCHOOSE->mHeight) / 2 + 2 + aRect.mY);
}

void Sexy::HelpScreen::DrawGameModesPage(Graphics* g)
{
	DrawTabTitle(g, "Insaniquarium has four different game modes that you can play!",
		"They are Adventure, Time Trial, Challenge, and Virtual Tank.");
	DrawVertTabText(g, 0, "Adventure is the main game mode.  In this mode, you",
		"progress through multiple tanks and accumulate pets.");
	DrawVertTabText(g, 1, "The goal of Time Trial mode is to collect as much",
		"money as you can before the time runs out.");
	DrawVertTabText(g, 2, "Challenge mode is for experts.  In this mode, you must",
		"deal with price inflation and increasingly difficult aliens.");
	DrawVertTabText(g, 3, "Virtual Tank is a Virtual Aquarium.  Use Shells earned in"
		, "other modes to purchase items and fish for your tank!");

	g->SetColor(Color(0xbbffbb));
	g->SetFont(FONT_CONTINUUMBOLD12);

	Rect aRect = GetLeftRectVertTab(0, 4);
	DrawGameModesPageText(g, "Adventure", (aRect.mWidth - g->GetFont()->StringWidth("Adventure")) / 2 + aRect.mX, aRect.mY + 30);

	aRect = GetLeftRectVertTab(1, 4);
	DrawGameModesPageText(g, "Time Trial", (aRect.mWidth - g->GetFont()->StringWidth("Time Trial")) / 2 + aRect.mX, aRect.mY + 30);

	aRect = GetLeftRectVertTab(2, 4);
	DrawGameModesPageText(g, "Challenge", (aRect.mWidth - g->GetFont()->StringWidth("Challenge")) / 2 + aRect.mX, aRect.mY + 30);

	aRect = GetLeftRectVertTab(3, 4);
	DrawGameModesPageText(g, "Virtual", (aRect.mWidth - g->GetFont()->StringWidth("Virtual")) / 2 + aRect.mX, aRect.mY + 18);
	DrawGameModesPageText(g, "Tank", (aRect.mWidth - g->GetFont()->StringWidth("Tank")) / 2 + aRect.mX, aRect.mY + 37);
}

void Sexy::HelpScreen::DrawGameModesPageText(Graphics* g, SexyString theString, int theX, int theY)
{
	int aVal = (mUpdateCnt + theY) % 200;
	if (aVal > 100)
		aVal = 200 - aVal;
	g->SetColor(Color(mApp->HSLToRGB(40, 200, (aVal * 70) / 100 + 180)));
	g->DrawString(theString, theX, theY);
}

void Sexy::HelpScreen::DrawVirtualTankPage(Graphics* g)
{
	DrawTabTitle(g, "Virtual Tank is a Virtual Aquarium.  In it, you can buy, name,",
		"take care of, and play with your own unique fish.");
	DrawVertTabText(g, 0, "Access the Virtual Tank store from within Virtual Tank.",
		"You must use Shells to buy items in the store.");
	DrawVertTabText(g, 1, "You can earn Shells at certain times in all four",
		"game modes.  Shells are the currency of Virtual Tank.");
	DrawVertTabText(g, 2, "Items in the Virtual Tank store change daily with the real",
		"world date so check back every day to see what\'s new!");
	DrawVertTabText(g, 3, "Some items in the store are quite rare so make sure to"
		, "check the store every day so you don\'t miss a rare item!");

	g->SetColor(Color(0xbbffbb));
	g->SetFont(FONT_CONTINUUMBOLD12);

	Rect aRect = GetLeftRectVertTab(0, 4);
	g->DrawImageCel(IMAGE_TROPHYBUTTONS, (aRect.mWidth - IMAGE_TROPHYBUTTONS->GetCelWidth()) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_TROPHYBUTTONS->GetHeight()) / 2 + aRect.mY, 2);

	aRect = GetLeftRectVertTab(1, 4);
	g->DrawImage(IMAGE_HELPSHELL, (aRect.mWidth - IMAGE_HELPSHELL->mWidth) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_HELPSHELL->mHeight) / 2 + aRect.mY);

	aRect = GetLeftRectVertTab(2, 4);
	Rect aSrcRect = Rect(mUpdateCnt / 2 % 10 * 80, 0, 80, 80);
	int anX = (aRect.mWidth - 80) / 2 + aRect.mX;
	int anY = (aRect.mHeight - 80) / 2 + aRect.mY;
	g->DrawImage(IMAGE_SIMSWIM1, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->SetColorizeImages(true);
	g->SetColor(Color(50, 100, 200, 255));
	g->DrawImage(IMAGE_SIMSWIM2, anX, anY, aSrcRect);
	g->SetColor(Color(255, 245, 65, 255));
	g->DrawImage(IMAGE_SIMSWIM3, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);

	aRect = GetLeftRectVertTab(3, 4);
	g->DrawImage(IMAGE_BALLS, (aRect.mWidth - 50) / 2 + aRect.mX, (aRect.mHeight - 50) / 2 + aRect.mY, Rect(mUpdateCnt/4%10*50,0,50,50));
}

void Sexy::HelpScreen::DrawVirtualTankFishPage(Graphics* g)
{
	DrawTabTitle(g, "Virtual Tank fish behave differently than fish in other game modes.",
		"Here is a list of differences.");
	DrawVertTabText(g, 0, "Virtual Tank fish want to eat three times a day.  They will",
		"become unhappy if you don\'t feed them three times every day.");
	DrawVertTabText(g, 1, "Fish in Virtual Tank will grow eventually.  The time",
		"to grow is about one real world week if fed every day.", 0);
	DrawVertTabText(g, 2, "To feed fish which don\'t eat the normal food that you drop by",
		"clicking, press the \"Feed\" button on the Virtual Tank menu.");
	DrawVertTabText(g, 3, "Virtual Tank fish can not die or be killed.  That\'s not"
		, "to say that you shouldn\'t take care of them, though!");

	g->SetColor(Color(0xbbffbb));
	g->SetFont(FONT_CONTINUUMBOLD12);

	Rect aRect = GetLeftRectVertTab(0, 4);

	int aFrameIdx = mUpdateCnt / 2 % 27;
	int aResId = IMAGE_SIMSWIM1_ID;
	if (aFrameIdx < 10)
		aResId = IMAGE_SIMEAT1_ID;
	else
		aFrameIdx += 3;

	Rect aSrcRect = Rect((aFrameIdx % 10) * 80, 0, 80, 80);

	int anX = (aRect.mWidth - 80) / 2 + aRect.mX;
	int anY = (aRect.mHeight - 80) / 2 + aRect.mY;

	Image* aToDrawImg = GetImageById(aResId);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->SetColorizeImages(true);
	g->SetColor(Color(35, 85, 130, 255)); // loc94

	aToDrawImg = GetImageById(aResId+1);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetColor(Color(180, 245, 180, 255)); // loc84

	aToDrawImg = GetImageById(aResId + 2);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);

	aRect = GetLeftRectVertTab(1, 4);

	aSrcRect = Rect(((mUpdateCnt / 2) % 10) * 80, 80, 80, 80);

	anX = (aRect.mWidth - 80) / 2 + aRect.mX;
	anY = (aRect.mHeight - 80) / 2 + aRect.mY;

	aToDrawImg = GetImageById(IMAGE_SIMSWIM1_ID);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->SetColorizeImages(true);
	g->SetColor(Color(35, 85, 130, 255)); // loc94

	aToDrawImg = GetImageById(IMAGE_SIMSWIM2_ID);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetColor(Color(180, 245, 180, 255)); // loc84

	aToDrawImg = GetImageById(IMAGE_SIMSWIM3_ID);
	g->DrawImage(aToDrawImg, anX, anY, aSrcRect);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);

	aRect = GetLeftRectVertTab(2, 4);

	g->DrawImageCel(IMAGE_TROPHYBUTTONS, (aRect.mWidth - IMAGE_TROPHYBUTTONS->GetCelWidth()) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_TROPHYBUTTONS->mHeight) / 2 + 2 + aRect.mY, 1);

	aRect = GetLeftRectVertTab(3, 4);
	
	g->DrawImage(IMAGE_SMALLDIE, (aRect.mWidth - 80) / 2 + aRect.mX, (aRect.mHeight - 80) / 2 + aRect.mY, Rect(720, 0, 80, 80));
	g->SetColor(Color(0xffff00));
	g->SetFont(FONT_CONTINUUMBOLD14);
	DrawStringWithOutline(g, "?", (aRect.mWidth - FONT_CONTINUUMBOLD14->StringWidth("?")) / 2 + aRect.mX, aRect.mY + 25, FONT_CONTINUUMBOLD14OUTLINE, 0);
}

void Sexy::HelpScreen::DrawConfigVirtualTankPage(Graphics* g)
{
	DrawTabTitle(g, "You can configure the Virtual Tank in several ways.",
		"Here is a list of the options.");
	DrawVertTabText(g, 0, "Press the \"Fish\" button to go to a screen where you can see",
		"information about your fish as well as hide, show, and sell them.");
	DrawVertTabText(g, 1, "Press the \"Pets\" button to select which pets you would like",
		"to display in your Virtual Tank.");
	DrawVertTabText(g, 2, "Press the \"Tank\" button to select which tank backdrop to display",
		"and whether or not to show various items in your tank.");
	DrawVertTabText(g, 3, "You can set Virtual Tank as your computer\'s Screensaver by"
		, "pressing the \"Screensaver\" button on the \"Tank\" screen.", 0);

	g->SetColor(Color(0xbbffbb));
	g->SetFont(FONT_CONTINUUMBOLD12);

	Rect aRect = GetLeftRectVertTab(0, 4);
	g->DrawImageCel(IMAGE_TROPHYBUTTONS, (aRect.mWidth - IMAGE_TROPHYBUTTONS->GetCelWidth()) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_TROPHYBUTTONS->GetHeight()) / 2 + 3 + aRect.mY, 3);
	aRect = GetLeftRectVertTab(1, 4);
	g->DrawImageCel(IMAGE_TROPHYBUTTONS, (aRect.mWidth - IMAGE_TROPHYBUTTONS->GetCelWidth()) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_TROPHYBUTTONS->GetHeight()) / 2 + aRect.mY, 4);
	aRect = GetLeftRectVertTab(2, 4);
	g->DrawImageCel(IMAGE_TROPHYBUTTONS, (aRect.mWidth - IMAGE_TROPHYBUTTONS->GetCelWidth()) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_TROPHYBUTTONS->GetHeight()) / 2 + aRect.mY, 5);
	aRect = GetLeftRectVertTab(3, 4);
	g->DrawImage(IMAGE_HELPSCREENSAVER, (aRect.mWidth - IMAGE_HELPSCREENSAVER->mWidth) / 2 + aRect.mX,
		(aRect.mHeight - IMAGE_HELPSCREENSAVER->mHeight) / 2 + 2 + aRect.mY);
}

void Sexy::HelpScreen::DrawCreditsPage(Graphics* g)
{
	bool is3DAccel = mApp->Is3DAccelerated();
	#define CREDITS_ROWS 11
	SexyString aCreditsMatrix[CREDITS_ROWS][2] = {
		{"Game Design", "George Fan"},
		{"Producer", "Jason Kapalka, Sukhbir Sidhu"},
		{"Programming", "George Fan, Thien Tran, Brian Rothstein"},
		{"Technical Assistance", "David Parton"},
		{"Art", "Josh Langley, Walter Wilson"},
		{"Character Design", "George Fan"},
		{"Music", "Jonne Valtonen, George Fan"},
		{"PopCap Framework", "Brian Fiete, David Parton"},
		{"Biz Dev","Don Walters"},
		{"QA", "Eric Harman, Shawn Conard,"},
		{"", "Brenna Flood, Chad Zoellner"}, };
	
	D3DInterface* anInterface = mApp->mDDInterface->mD3DInterface;
	int anY = 150;
	double aRad = 150;
	float anYOffset = 0;

	for (int i = 0; i < CREDITS_ROWS; i++)
	{
		if (is3DAccel)
		{
			aRad = ((aRad * 2) * PI) / 180.0 + (mUpdateCnt * 3.0 * PI) / 180.0;
			float aSinVal = sinf(aRad);
			anYOffset = aSinVal * 3;
			SexyTransform2D aTransform2D;
			aTransform2D.Translate(0, anYOffset);
			anInterface->PushTransform(aTransform2D);
		}
		g->SetColor(Color(0xbbffbb));
		g->SetFont(FONT_CONTINUUMBOLD14);

		if (aCreditsMatrix[i][0].size() == 0)
			anY -= 7;

		DrawCreditsPageLeftText(g, aCreditsMatrix[i][0], 30, anY);

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		SetVaryingBlueColorGraphics(g, mUpdateCnt,anY);
		g->DrawString(aCreditsMatrix[i][1], 300, anY);

		anY += 25;

		aRad = anY;
		if (is3DAccel)
			anInterface->PopTransform();
	}
}

void Sexy::HelpScreen::DrawCreditsPageLeftText(Graphics* g, SexyString theString, int theX, int theY)
{
	int aVal = (mUpdateCnt + theY) % 200;
	if (aVal > 100)
		aVal = 200 - aVal;
	g->SetColor(Color(mApp->HSLToRGB(40, 200, (aVal * 70) / 100 + 180)));
	g->SetFont(FONT_CONTINUUMBOLD14);
	DrawStringWithOutline(g, theString, theX, theY, FONT_CONTINUUMBOLD14OUTLINE, 0);
}

void Sexy::HelpScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::HelpScreen::ButtonDepress(int theId)
{
	if (theId == 1)
	{
		mApp->RemoveHelpScreen();
		if (!mApp->mBoard)
		{
			mApp->SwitchToGameSelector();
			return;
		}
	}
	else if(theId == 0)
	{
		mApp->StartGame();
		mApp->RemoveHelpScreen();
		return;
	}
	else if (theId == 2)
	{
		mPageIdx++;
		if (mPageIdx > 8)
			mPageIdx = 1;
		return;
	}
	else if (theId == 3)
	{
		mPageIdx--;
		if (mPageIdx < 1)
			mPageIdx = 8;
		return;
	}
}

