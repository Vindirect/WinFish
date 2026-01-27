#include "StoreButtonWidget.h"
#include "WinFishApp.h"
#include "SexyAppFramework/Font.h"
#include "StoreScreen.h"
#include "Res.h"
#include "GameObject.h"

Sexy::StoreButtonWidget::StoreButtonWidget(int theId, ButtonListener* theButtonListener, Image* theButtonImage)
	: ButtonWidget(theId, theButtonListener)
{
	mApp = (WinFishApp*)gSexyApp;
	mClip = false;
	m0x148 = false;
	mDoFinger = true;
	mButtonImage = theButtonImage;
	mOverRect.mX = mButtonImage->mWidth / 2;
	mDownRect.mX = mButtonImage->mWidth / 2;
	mNormalRect.mX = 0;
	mNormalRect.mY = 0;
	mOverRect.mY = 0;
	mDownRect.mY = 0;
	mNormalRect.mWidth = mButtonImage->mWidth / 2;
	mOverRect.mWidth = mButtonImage->mWidth / 2;
	mDownRect.mWidth = mButtonImage->mWidth / 2;
	mProduct = nullptr;
	m0x14c = 0;
	m0x158 = 0;
	mNormalRect.mHeight = mButtonImage->mHeight;
	mOverRect.mHeight = mButtonImage->mHeight;
	mDownRect.mHeight = mButtonImage->mHeight;
	m0x128 = 100;
	m0x150 = 13;
	m0x154 = 108;
}

Sexy::StoreButtonWidget::~StoreButtonWidget()
{
	if (mProduct)
		delete mProduct;
}

void Sexy::StoreButtonWidget::Update()
{
	WidgetContainer::Update();
	if (mProduct)
		mProduct->UpdateStoreAnimation();
}

void Sexy::StoreButtonWidget::Draw(Graphics* g)
{
	int localc = 0;
	int anXTrans = 0;
	int anYTrans = 0;
	if (mButtonImage == IMAGE_STORELEFTBUTTON)
	{
		anXTrans = 15;
		anYTrans = 4;
	}
	else if (mButtonImage == IMAGE_STORETOPBUTTON)
		anYTrans = 5;
	if (mId > 2)
		localc = (mId > 6) + 1;

	g->Translate(anXTrans, anYTrans);
	ButtonWidget::Draw(g);
	g->Translate(-anXTrans, -anYTrans);

	if (mMouseVisible)
	{
		switch (m0x14c) 
		{
		case 1:
			if (mProduct)
				mProduct->DrawStoreAnimation(g, localc);
			break;
		case 2:
			g->DrawImageCel(IMAGE_BUBBULATOR_STORE, 35, 23, 0);
			break;	
		case 3:
			g->DrawImageCel(IMAGE_STOREBACKDROPS, 0, 0, m0x158);
			if (mIsOver || mIsDown || m0x148)
			{
				g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
				g->SetColorizeImages(true);
				g->SetColor(Color(50, 50, 50));
				g->DrawImageCel(IMAGE_STOREBACKDROPS, 0, 0, m0x158);
				g->SetColorizeImages(false);
				g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
			}
			break;
		case 4:
			g->DrawImage(IMAGE_AA_STORE, 30, 20);
			break;
		case 5:
			g->DrawImageCel(IMAGE_FOOD, 40, 40, (mUpdateCnt/4) % 10, 2);
			break;
		}
	}
}

void Sexy::StoreButtonWidget::DrawOverlay(Graphics* g)
{
	g->SetFont(FONT_CONTINUUMBOLD12OUTLINE);
	g->SetColor(Color(110, 250, 110));
	int aX = m0x150 - g->GetFont()->StringWidth(m0x12c)/2;
	int aStrX = aX + 45;
	if (!mMouseVisible)
	{
		g->SetColor(Color(0xcccccc));
		aStrX = aX + 40;
	}
	else
		g->DrawImage(IMAGE_STORESHELL, aX + 25, m0x154 + 3);

	int anAsc = g->GetFont()->GetAscent();
	g->DrawString(m0x12c, aStrX, anAsc + m0x154);
	if (mIsOver || mIsDown || m0x148)
	{
		g->SetColor(Color(240, 255, 193));
		g->SetColorizeImages(true);
		if (mButtonImage == IMAGE_STORELEFTBUTTON)
			g->DrawImage(IMAGE_STORELEFTHILIGHT, 4, 4);
		else if (mButtonImage == IMAGE_STORETOPBUTTON)
			g->DrawImage(IMAGE_STORETOPHILIGHT, -7, -2);
		else
			g->DrawImage(IMAGE_STORERIGHTHILIGHT, -4, -3);
		g->SetColorizeImages(false);
	}
}

void Sexy::StoreButtonWidget::MouseEnter()
{
	ButtonWidget::MouseEnter();
	if (mApp->mStoreScreen)
	{
		mApp->mStoreScreen->mOverButtonId = mId;
		mApp->mStoreScreen->mStoreScreenUpdateCnt = 0;
	}
}

void Sexy::StoreButtonWidget::MouseLeave()
{
	ButtonWidget::MouseLeave();
	if (mApp->mStoreScreen)
		mApp->mStoreScreen->mOverButtonId = -1;
}

void Sexy::StoreButtonWidget::HandleMouseEvent(bool inside)
{
	if (inside != m0x148)
	{
		m0x148 = inside;
		Rect aTempRect = mNormalRect;
		mNormalRect = mOverRect;
		mOverRect = aTempRect;
	}
}

Sexy::GameObject* Sexy::StoreButtonWidget::GetProduct(bool removeProduct)
{
	GameObject* aRet = mProduct;
	if (removeProduct)
		mProduct = nullptr;
	return aRet;
}

void Sexy::StoreButtonWidget::SetUpButton(GameObject* theProduct, int theValue)
{
	if (mProduct)
		delete mProduct;
	mProduct = theProduct;
	if (theProduct)
	{
		mMouseVisible = true;
		theProduct->mShellPrice = theValue;
		m0x14c = 1;
	}
	else
		m0x14c = 0;
	SetPrice(theValue);
}

void Sexy::StoreButtonWidget::SetPrice(int theValue)
{
	m0x128 = theValue;
	m0x12c = StrFormat("%d", theValue);
}

void Sexy::StoreButtonWidget::Bought()
{
	SetUpButton(nullptr, 0);
	HandleMouseEvent(false);
	mMouseVisible = false;
	m0x12c = "SOLD";
}

void Sexy::StoreButtonWidget::SetProductType(int theType, int theImageCel, int thePrice)
{
	if (theType != 1)
	{
		if (mProduct)
			delete mProduct;
		mProduct = nullptr;
	}
	m0x14c = theType;
	m0x158 = theImageCel;
	SetPrice(thePrice);
	mMouseVisible = true;
}

