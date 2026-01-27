#include "PetButtonWidget.h"
#include "WinFishApp.h"
#include "Res.h"
#include "PetsScreen.h"

using namespace Sexy;

Sexy::PetButtonWidget::PetButtonWidget(int theImageId, int theId, ButtonListener* theListener)
	: ButtonWidget(theId, theListener)
{
	mRect = Rect(0,0,0,0);
	mApp = (WinFishApp*) gSexyApp;
	m0x138 = theImageId;
	mDoFinger = true;
	mButtonImage = IMAGE_PETBUTTON;

	mNormalRect.mX = IMAGE_PETBUTTON->mWidth/4;
	mNormalRect.mY = 0;
	mNormalRect.mWidth = IMAGE_PETBUTTON->mWidth/4;
	mNormalRect.mHeight = IMAGE_PETBUTTON->mHeight;

	mOverRect.mX = IMAGE_PETBUTTON->mWidth/4 * 3;
	mOverRect.mY = 0;
	mOverRect.mWidth = IMAGE_PETBUTTON->mWidth/4;
	mOverRect.mHeight = IMAGE_PETBUTTON->mHeight;

	mDownRect.mX = IMAGE_PETBUTTON->mWidth/4 * 2;
	mDownRect.mY = 0;
	mDownRect.mWidth = IMAGE_PETBUTTON->mWidth/4;
	mDownRect.mHeight = IMAGE_PETBUTTON->mHeight;

	mDisabledRect.mX = 0;
	mDisabledRect.mY = 0;
	mDisabledRect.mWidth = IMAGE_PETBUTTON->mWidth/4;
	mDisabledRect.mHeight = IMAGE_PETBUTTON->mHeight;

	m0x130 = false;
}

Sexy::PetButtonWidget::~PetButtonWidget()
{
}

void Sexy::PetButtonWidget::Draw(Graphics* g)
{
	ButtonWidget::Draw(g);
	if (!mDisabled)
	{
		Image* anImg = GetImageById(m0x138);
		Rect aRect = anImg->GetAnimCelRect(mUpdateCnt);
		int aX = 14;
		int aY = 10;
		if (anImg == IMAGE_SCL_AMP)
		{
			aX = 15;
			aY = 24;
		}
		g->DrawImageAnim(anImg, aX, aY, mUpdateCnt);
	}
}

void Sexy::PetButtonWidget::MouseDown(int x, int y, int theClickCount)
{
	Widget::MouseDown(x, y, theClickCount);
}

void Sexy::PetButtonWidget::MouseUp(int x, int y, int theClickCount)
{
	if (mIsOver && !mDisabled)
	{
		if (m0x130 && mApp->mPetsScreen != nullptr)
		{
			Setup(false);
			mApp->mPetsScreen->m0x124--;
		}
		else if (mApp->mPetsScreen != nullptr && mApp->mPetsScreen->m0x124 < mApp->mPetsScreen->m0x130)
		{
			Setup(true);
			mApp->mPetsScreen->m0x124++;
		}
	}
}

void Sexy::PetButtonWidget::Setup(bool flag)
{
	if (flag == m0x130)
		return;

	Rect aTemp1 = mDownRect;
	mDownRect = mNormalRect;
	mNormalRect = aTemp1;

	aTemp1 = mOverRect;
	mOverRect = mRect;
	mRect = aTemp1;

	m0x130 = flag;

	MarkDirty();
}
