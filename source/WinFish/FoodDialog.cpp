#include "FoodDialog.h"
#include "MoneyDialog.h"
#include "WinFishApp.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"
#include "Board.h"
#include "Res.h"

Sexy::FoodDialog::FoodDialog(WinFishApp* theApp)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_FOOD, false, "FOOD SELECTOR", "", "CLOSE", BUTTONS_FOOTER)
{
	for (int i = 0;i < 8;i++)
		mButtons[i] = {};

	mContentInsets = Insets(30, 0, 30, 15);
	Resize(0, 65, 640, 50);
	m0x158 = -1;
	m0x15c = -1;

	int aReqs[8] = { 1,1,1,1,1,1,1,1 };
	int aInTank[8] = { 0 };
	if (mApp->mBoard != nullptr)
	{
		mApp->mBoard->GetExoticFoodsRequiredInTank(aReqs);
		mApp->mBoard->GetExoticFoodsInTank(aInTank);
	}

	int ivar5 = 0;
	for (int i = 0; i < 8;i++)
	{
		if (mApp->mBoard == nullptr)
		{
			mButtons[i].mShown = true;
			ivar5++;
		}
		else
		{
			mButtons[i].mShown = aReqs[i] > 0;
			if(aReqs[i] > 0)
				ivar5++;
		}
	}

	int anXPos = 300 - (ivar5 * 56) / 2;
	for (int i = 0; i < 8;i++)
	{
		mButtons[i].mButtonRect = Rect(anXPos, 2, 50, 50);
		mButtons[i].mId = i;
		if (mButtons[i].mShown)
			anXPos += 56;
	}
}

Sexy::FoodDialog::~FoodDialog()
{
}

void Sexy::FoodDialog::Draw(Graphics* g)
{
	g->SetColor(Color(0xb0000000));
	g->FillRect(0, 0, mWidth, mHeight);
	g->SetColor(Color(0xffffff));
	g->DrawRect(0, 0, mWidth - 1, mHeight - 1);
	g->SetColor(Color(0xffff00));
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	int anAscent = g->GetFont()->GetAscent();
	g->DrawString("SELECT", 10, anAscent + 5);
	int aHeight = g->GetFont()->GetHeight();
	g->DrawString("FOOD", 10, aHeight - 5 + anAscent + 5);
	for (int i = 0; i < 8; i++)
	{
		if (!mButtons[i].mShown)
			continue;

		int anImgId = IMAGE_MENUBTNU_ID;
		int anX = mButtons[i].mButtonRect.mX;
		int anY = mButtons[i].mButtonRect.mY;
		if (i == m0x15c)
		{
			anImgId = IMAGE_MENUBTND2_ID;
			anX++;
			anY++;
		}
		else
		{
			if (i == m0x158)
				anImgId = IMAGE_MENUBTNO_ID;
		}

		g->DrawImage(GetImageById(anImgId), mButtons[i].mButtonRect.mX, mButtons[i].mButtonRect.mY);

		switch (i)
		{
		case 0:
			g->DrawImageCel(IMAGE_SMALLSWIM, anX - 10, anY - 17, 0, 0);
			break;
		case 1:
			g->DrawImageCel(IMAGE_MONEY, anX - 8, anY - 12, 0, 2);
			break;
		case 2:
			g->DrawImageCel(IMAGE_MONEY, anX - 8, anY - 15, 0, 5);
			break;
		case 3:
			g->DrawImageCel(IMAGE_SCL_OSCAR, anX + 8, anY + 3, 0, 0);
			break;
		case 4:
			g->DrawImageCel(IMAGE_SCL_ULTRA, anX + 8, anY + 3, 0, 0);
			break;
		case 5:
			g->DrawImage(IMAGE_CHICKEN, anX + 8, anY + 2);
			break;
		case 6:
			g->DrawImage(IMAGE_PIZZA, anX + 8, anY + 2);
			break;
		case 7:
			g->DrawImage(IMAGE_ICECREAM, anX + 13, anY);
			break;
		}
		g->DrawImage(IMAGE_MENUBUTTONREFLECTION, mButtons[i].mButtonRect.mX, mButtons[i].mButtonRect.mY, Rect(0, 0, IMAGE_MENUBUTTONREFLECTION->mWidth, 40));
	}
}

void Sexy::FoodDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);
	mYesButton->Resize(530, theY + 10, 100, mYesButton->mHeight);
}

void Sexy::FoodDialog::MouseLeave()
{
	if (m0x15c == -1 && m0x158 == -1)
		return;

	m0x15c = -1;
	m0x158 = -1;
	MarkDirty();
}

void Sexy::FoodDialog::MouseMove(int x, int y)
{
	int aHandle = MouseAtHandle(x, y);
	if (aHandle != m0x158)
	{
		m0x158 = aHandle;
		MarkDirty();
	}
}

void Sexy::FoodDialog::MouseDown(int x, int y, int theClickCount)
{
	int aHandle = MouseAtHandle(x, y);
	m0x15c = aHandle;
	m0x158 = aHandle;
	MarkDirty();
}

void Sexy::FoodDialog::MouseUp(int x, int y, int theClickCount)
{
	int aHandle = MouseAtHandle(x, y);
	m0x158 = aHandle;
	int aReqs[8] = { 0 };
	int aInTank[8] = { 0 };
	if (aHandle == m0x15c && m0x15c != -1 && mApp->mBoard != nullptr)
	{
		mApp->mBoard->GetExoticFoodsRequiredInTank(aReqs);
		mApp->mBoard->GetExoticFoodsInTank(aInTank);
		if (aInTank[m0x15c] < aReqs[m0x15c])
			mApp->mBoard->SpawnVirtualTankFood(m0x15c);
		else
			mApp->PlaySample(SOUND_BUTTONCLICK);
	}

	m0x15c = -1;
	MarkDirty();
}

int Sexy::FoodDialog::MouseAtHandle(int x, int y)
{
	for (int i = 0;i < 8;i++)
	{
		FishFeedButton* aBtn = &mButtons[i];
		if (aBtn->mShown && aBtn->mButtonRect.mX <= x && aBtn->mButtonRect.mX + aBtn->mButtonRect.mWidth > x &&
			aBtn->mButtonRect.mY <= y && aBtn->mButtonRect.mY + aBtn->mButtonRect.mHeight > y)
		{
			return i;
		}
	}
	return -1;
}

