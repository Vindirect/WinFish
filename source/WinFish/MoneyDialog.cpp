#include "MoneyDialog.h"
#include "Res.h"
#include "WinFishAppCommon.h"
#include "WinFishApp.h"

#include "SexyAppFramework/DialogButton.h"

using namespace Sexy;

Sexy::MoneyDialog::MoneyDialog(WinFishApp* theApp, Image* theComponentImage, Image* theButtonComponentImage, 
	int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, 
	const SexyString& theDialogFooter, int theButtonMode) :
	Dialog(theComponentImage, theButtonComponentImage, theId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode)
{
	mApp = theApp;
	mEnableButtonsTimer = -1;
	DefaultDialogSettings(this);
}

Sexy::MoneyDialog::~MoneyDialog()
{
}

void Sexy::MoneyDialog::Update()
{
	Widget::Update();
	if (mUpdateCnt == mEnableButtonsTimer)
	{
		if (mYesButton)
			mYesButton->SetDisabled(false);
		if (mNoButton)
			mNoButton->SetDisabled(false);
	}
}

void Sexy::MoneyDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::MoneyDialog::ButtonDepress(int theId)
{
	if (mEnableButtonsTimer < mUpdateCnt)
		Dialog::ButtonDepress(theId);
}

void Sexy::MoneyDialog::DisableButtons(int theEnableButtonsTimer)
{
	mEnableButtonsTimer = theEnableButtonsTimer;
	if (mYesButton)
		mYesButton->SetDisabled(true);
	if (mNoButton)
		mNoButton->SetDisabled(true);
}

void Sexy::MoneyDialog::WidgetClicked(int theId, bool theFlag)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::MoneyDialog::DrawEditWidgetBox(Graphics* g, EditWidget* theWidget)
{
	g->DrawImageBox(Rect((theWidget->mX - 8) - g->mTransX, (theWidget->mY - 4) - g->mTransY,
		theWidget->mWidth + 16, theWidget->mHeight + 8), IMAGE_EDITBOX);
}
