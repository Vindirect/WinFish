#include "FishButtonWidget.h"
#include "WinFishApp.h"
#include "GameObject.h"
#include "Res.h"

Sexy::FishButtonWidget::FishButtonWidget(GameObject* theObject, int theId, ButtonListener* theButtonListener)
	: ButtonWidget(theId, theButtonListener)
{
	mObject = theObject;
	mApp = (WinFishApp*)gSexyApp;
	mDoFinger = true;
	mButtonImage = IMAGE_PETBUTTON;
	int aBtnWdth = IMAGE_PETBUTTON->mWidth / 4;
	int aBtnHght = IMAGE_PETBUTTON->mHeight;
	mOverRect = Rect(aBtnWdth * 3, 0, aBtnWdth, aBtnHght);
	mDownRect = Rect(aBtnWdth * 2, 0, aBtnWdth, aBtnHght);
	mNormalRect = Rect(aBtnWdth, 0, aBtnWdth, aBtnHght);
	m0x130 = Rect(aBtnWdth * 2, 0, aBtnWdth, aBtnHght);
	mDisabledRect = Rect(0, 0, aBtnWdth, aBtnHght);

	m0x150 = false;
	if (theObject == nullptr)
		mDisabled = true;
}

Sexy::FishButtonWidget::~FishButtonWidget()
{
}

void Sexy::FishButtonWidget::Update()
{
	if (mObject)
		mObject->UpdateStoreAnimation();
}

void Sexy::FishButtonWidget::Draw(Graphics* g)
{
	ButtonWidget::Draw(g);
	if (mObject)
		mObject->DrawStoreAnimation(g, 3);
}

void Sexy::FishButtonWidget::SwitchDrawRects(bool toggle)
{
	if (toggle != m0x150)
	{
		m0x150 = toggle;
		Rect aTempRect = mNormalRect;
		mNormalRect = m0x130;
		m0x130 = aTempRect;
		aTempRect = mOverRect;
		mOverRect = m0x120;
		m0x120 = aTempRect;
		aTempRect = mDownRect;
		mDownRect = m0x140;
		m0x140 = aTempRect;
		MarkDirty();
	}
}
