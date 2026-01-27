#include "MenuButtonWidget.h"
#include "ToolTipWidget.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::MenuButtonWidget::MenuButtonWidget(WidgetManager* theWidgetManager, int theId, ButtonListener* 
	theButtonListener, SexyString theTooltipText)
	: ButtonWidget(theId, theButtonListener)
{
	mManager = theWidgetManager;
	mDoFinger = true;
	mClip = false;
	mToolTipXOffset = 15;
	mToolTipYOffset = 60;
	mSlotImage = 0;
	m0x13c = 0;
	m0x138 = 0;
	mToolTipWidget = new ToolTipWidget(theTooltipText);
	mPriceTextColor = Color(0x6e, 0xfa, 0x6e);
}

Sexy::MenuButtonWidget::~MenuButtonWidget()
{
	if (mToolTipWidget)
		delete mToolTipWidget;
}

void Sexy::MenuButtonWidget::RemovedFromManager(WidgetManager* theWidgetManager)
{
	ButtonWidget::RemovedFromManager(theWidgetManager);
	if (mToolTipWidget)
		mManager->RemoveWidget(mToolTipWidget);
}

void Sexy::MenuButtonWidget::Update()
{
	WidgetContainer::Update();
	WinFishApp* anApp = (WinFishApp*)gSexyApp;
	if (!anApp->mBoard || !anApp->mBoard->mPause)
	{
		m0x138++;
		if (m0x13c > 0)
			m0x13c--;
	}
}

void Sexy::MenuButtonWidget::Draw(Graphics* g)
{
	WinFishApp* anApp = (WinFishApp*)gSexyApp;
	bool loc11 = false;
	if (mMouseVisible && m0x13c < 7)
	{
		ButtonWidget::Draw(g);

		int aY = 0;
		if (anApp->mBoard)
			aY = anApp->mBoard->mGameUpdateCnt;

		if (mSlotImage != 0)
		{
			int aVal = mSlotImageCol;
			if (aVal < 0)
				aVal = (aY / 2) % mSlotImage->mNumCols;
			g->DrawImageCel(mSlotImage, mSlotImageX, mSlotImageY, aVal, mSlotImageRow);
		}

		g->SetFont(FONT_CONTINUUMBOLD12OUTLINE);
		g->SetColor(Color(0x6e, 0xfa, 0x6e));
		g->DrawString(m0x15c, (mWidth - g->GetFont()->StringWidth(m0x15c)) / 2, g->GetFont()->GetAscent() + 9);
		loc11 = true;
	}

	g->SetFont(FONT_TINY);
	g->SetColor(mPriceTextColor);

	g->DrawString(mPriceText, (mWidth - g->GetFont()->StringWidth(mPriceText)) / 2, g->GetFont()->GetAscent() + 45); // May be incorrect

	if (m0x13c != 0 && m0x13c < 7)
	{
		g->DrawImageCel(IMAGE_HATCHANIMATION, 0, 0, (6 - m0x13c) / 2);
	}
	if (loc11)
	{
		g->DrawImage(IMAGE_MENUBUTTONREFLECTION, 0,0);
		return;
	}

	Graphics gClip(*g);

	gClip.ClipRect(0, 40, mWidth, mHeight - 40);
	gClip.DrawImage(IMAGE_MENUBUTTONREFLECTION, 0, 0);
}

void Sexy::MenuButtonWidget::MouseEnter()
{
	ButtonWidget::MouseEnter();
	mToolTipWidget->Resize(mToolTipXOffset + mX, mToolTipYOffset + mY, mToolTipWidget->mWidth, mToolTipWidget->mHeight);
	mManager->AddWidget(mToolTipWidget);
}

void Sexy::MenuButtonWidget::MouseLeave()
{
	ButtonWidget::MouseLeave();
	mManager->RemoveWidget(mToolTipWidget);
}

void Sexy::MenuButtonWidget::MouseDown(int theX, int theY, int theClickCount)
{
	Widget::MouseDown(theX, theY, theClickCount);
	mManager->RemoveWidget(mToolTipWidget);
}

void Sexy::MenuButtonWidget::Configure(Image* theImage, int theSlotImageX, int theSlotImageY, int theSlotImageRow, int theSlotImageCol)
{
	mSlotImage = theImage;
	mSlotImageX = theSlotImageX;
	mSlotImageY = theSlotImageY;
	mSlotImageRow = theSlotImageRow;
	mSlotImageCol = theSlotImageCol;
}

void Sexy::MenuButtonWidget::SetSlotPrice(int thePrice)
{
	if (!mMouseVisible)
		return;

	if (thePrice < 1)
		mPriceText = "";
	else
		mPriceText = StrFormat("$%d", thePrice);
}

void Sexy::MenuButtonWidget::SetMaxedOut()
{
	mDoFinger = false;
	mMouseVisible = false;
	ShowFinger(false);
	m0x15c = "";
	mPriceText = "MAX";
}
