#include "ScreenSaverDialog.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Res.h"

Sexy::ScreenSaverDialog::ScreenSaverDialog(WinFishApp* theApp)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_SCREENSAVER, true, "Screensaver", "", "", BUTTONS_OK_CANCEL)
{
	mSSEnabledCB = MakeCheckbox(0, this, mApp->mScreenSaverEnabled);
	mSSSoundCB = MakeCheckbox(1, this, mApp->mScreenSaverSound);
	mSSRotateBackdropsCB = MakeCheckbox(2, this, mApp->mScreenSaverRotateBackdrops);
	mSSPeriodicDimCB = MakeCheckbox(3, this, mApp->mScreenSaverPeriodicDim);
	mSSShowMoneyCB = MakeCheckbox(4, this, mApp->mScreenSaverShowMoney);
	mSSPowerSaveCB = MakeCheckbox(5, this, mApp->mScreenSaverPowerSave);
}

Sexy::ScreenSaverDialog::~ScreenSaverDialog()
{
	if (mSSEnabledCB)
		delete mSSEnabledCB;
	if (mSSSoundCB)
		delete mSSSoundCB;
	if (mSSRotateBackdropsCB)
		delete mSSRotateBackdropsCB;
	if (mSSPeriodicDimCB)
		delete mSSPeriodicDimCB;
	if (mSSShowMoneyCB)
		delete mSSShowMoneyCB;
	if (mSSPowerSaveCB)
		delete mSSPowerSaveCB;
}

void Sexy::ScreenSaverDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mSSEnabledCB);
	theWidgetManager->AddWidget(mSSSoundCB);
	theWidgetManager->AddWidget(mSSRotateBackdropsCB);
	theWidgetManager->AddWidget(mSSPeriodicDimCB);
	theWidgetManager->AddWidget(mSSShowMoneyCB);
	theWidgetManager->AddWidget(mSSPowerSaveCB);
}

void Sexy::ScreenSaverDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mSSEnabledCB);
	theWidgetManager->RemoveWidget(mSSSoundCB);
	theWidgetManager->RemoveWidget(mSSRotateBackdropsCB);
	theWidgetManager->RemoveWidget(mSSPeriodicDimCB);
	theWidgetManager->RemoveWidget(mSSShowMoneyCB);
	theWidgetManager->RemoveWidget(mSSPowerSaveCB);
}

void Sexy::ScreenSaverDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	DrawCheckboxString(g, "Enable Screensaver", mSSEnabledCB, nullptr);
	DrawCheckboxString(g, "Allow Monitor Powersave Mode", mSSPowerSaveCB, nullptr);
	DrawCheckboxString(g, "Enable Sound", mSSSoundCB, nullptr);
	DrawCheckboxString(g, "Rotate Backdrops", mSSRotateBackdropsCB, nullptr);
	DrawCheckboxString(g, "Periodically Darken Screen", mSSPeriodicDimCB, nullptr);
	DrawCheckboxString(g, "Show Shells Collected", mSSShowMoneyCB, nullptr);

	SexyString aStr = "";
	if (mSSEnabledCB->mIsOver)
		aStr = "Check here to set your Virtual Tank as your Screensaver.";
	else if (mSSPowerSaveCB->mIsOver)
		aStr = "Check here to allow your monitor to go into powersave mode while running the Insaniquarium Screensaver.";
	else if (mSSSoundCB->mIsOver)
		aStr = "This option controls whether or not to play sounds in the Screensaver.";
	else if (mSSRotateBackdropsCB->mIsOver)
		aStr = "Setting this option will cause the Screensaver to periodically rotate among your purchased tank backdrops.";
	else if (mSSPeriodicDimCB->mIsOver)
		aStr = "This option will make the Screensaver periodically darken the screen.";
	else if (mSSShowMoneyCB->mIsOver)
		aStr = "This option will have the Screensaver show you how many Shells you have collected while it has been running.";
	else
		aStr = "Insaniquarium Screensaver Settings";

	if (aStr.length() > 0)
	{
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		g->SetColor(Color(0xffffaa));
		int aHght = GetWordWrappedHeight(g, mWidth - 80, aStr, -1);
		WriteWordWrapped(g, Rect(40, 70 - aHght / 2, mWidth - 80, 0), aStr, -1, 0);
	}
}

void Sexy::ScreenSaverDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	mSSEnabledCB->Resize(mX + 33, mY + 100, 46, 45);
	mSSPowerSaveCB->Layout(0x1203, mSSEnabledCB);
	mSSSoundCB->Layout(0x1203, mSSPowerSaveCB);
	mSSRotateBackdropsCB->Layout(0x1203, mSSSoundCB);
	mSSPeriodicDimCB->Layout(0x1203, mSSRotateBackdropsCB);
	mSSShowMoneyCB->Layout(0x1203, mSSPeriodicDimCB);
}

int Sexy::ScreenSaverDialog::GetPreferredHeight(int theWidth)
{
	return 440;
}

void Sexy::ScreenSaverDialog::WidgetClicked(int theId, bool theFlag)
{
	MoneyDialog::WidgetClicked(theId, theFlag);
}

void Sexy::ScreenSaverDialog::CheckboxChecked(int theId, bool checked)
{
	WidgetClicked(theId, checked);
}
