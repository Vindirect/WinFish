#include "OptionsDialog.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "Board.h"
#include "SexyAppFramework/Slider.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/DialogButton.h"

using namespace Sexy;

Sexy::OptionsDialog::OptionsDialog(WinFishApp* theApp, bool theFlag)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_OPTIONS, true, "Options",
		"", "OK", BUTTONS_FOOTER)
{
	mApp2 = theApp;
	SetColor(3, Color(0xff, 0xff, 100));
	mFlag = theFlag;
	mRegisterButton = MakeDialogButton(0, this, "Register", NULL);
	mHelpButton = MakeDialogButton(1, this, "Help", NULL);
	SexyString aWebLinkStr = mApp->GetString("weblinktext", "");
	mWebLinkButton = MakeDialogButton(2, this, aWebLinkStr, NULL);

	mCheckUpdatesButton = MakeDialogButton(4, this, "Check Updates", NULL);
	mBackButton = MakeDialogButton(3, this, "Back to Main Menu", NULL);

	mMusicSlider = new Slider(IMAGE_SLIDERTRACK, IMAGE_SLIDERTHUMB, 5, this);
	double aMusicValue = mApp->GetMusicVolume();
	if (aMusicValue > 1.0) aMusicValue = 1.0; if (aMusicValue < 0) aMusicValue = 0;
	mMusicSlider->SetValue(aMusicValue);

	mSFXSlider = new Slider(IMAGE_SLIDERTRACK, IMAGE_SLIDERTHUMB, 6, this);
	double aSfxValue = mApp->GetSfxVolume();
	if (aSfxValue > 1.0) aSfxValue = 1.0; if (aSfxValue < 0) aSfxValue = 0;
	mSFXSlider->SetValue(aSfxValue);

	mFullscreenCB = MakeCheckbox(7, this, !mApp->mIsWindowed);
	mCustomCursorsCB = MakeCheckbox(8, this, mApp->mCustomCursorsEnabled);
	m3DCB = MakeCheckbox(9, this, mApp->Is3DAccelerated());

	if (theFlag)
		mBackButton->SetVisible(false);
	if (mApp->mDontUpdate)
		mCheckUpdatesButton->SetVisible(false);
	if(mApp->mBuildUnlocked)
		mRegisterButton->SetVisible(false);
	mWebLinkButton->SetVisible(mWebLinkButton->mLabel.size() != 0);
}

Sexy::OptionsDialog::~OptionsDialog()
{
	if (mRegisterButton)
		delete mRegisterButton;
	if (mCheckUpdatesButton)
		delete mCheckUpdatesButton;
	if (mWebLinkButton)
		delete mWebLinkButton;
	if (mMusicSlider)
		delete mMusicSlider;
	if (mSFXSlider)
		delete mSFXSlider;
	if (mFullscreenCB)
		delete mFullscreenCB;
	if (mCustomCursorsCB)
		delete mCustomCursorsCB;
	if (m3DCB)
		delete m3DCB;
	if (mHelpButton)
		delete mHelpButton;
	if (mBackButton)
		delete mBackButton;
}

void Sexy::OptionsDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mRegisterButton);
	theWidgetManager->AddWidget(mCheckUpdatesButton);
	theWidgetManager->AddWidget(mHelpButton);
	theWidgetManager->AddWidget(mMusicSlider);
	theWidgetManager->AddWidget(mSFXSlider);
	theWidgetManager->AddWidget(mCustomCursorsCB);
	theWidgetManager->AddWidget(m3DCB);
	theWidgetManager->AddWidget(mFullscreenCB);
	theWidgetManager->AddWidget(mWebLinkButton);
	theWidgetManager->AddWidget(mBackButton);
}

void Sexy::OptionsDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mRegisterButton);
	theWidgetManager->RemoveWidget(mCheckUpdatesButton);
	theWidgetManager->RemoveWidget(mHelpButton);
	theWidgetManager->RemoveWidget(mMusicSlider);
	theWidgetManager->RemoveWidget(mSFXSlider);
	theWidgetManager->RemoveWidget(mCustomCursorsCB);
	theWidgetManager->RemoveWidget(m3DCB);
	theWidgetManager->RemoveWidget(mFullscreenCB);
	theWidgetManager->RemoveWidget(mWebLinkButton);
	theWidgetManager->RemoveWidget(mBackButton);
}

void Sexy::OptionsDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColorizeImages(true);
	g->SetColor(mColors[4]);
	g->DrawString("Music", 48, 78);
	g->DrawString("Sound Fx", 48, 110);
	g->DrawString("Fullscreen", mFullscreenCB->mX - mX + 43, mFullscreenCB->mY - mY + 24);
	g->DrawString("Custom Cursors", mCustomCursorsCB->mX - mX + 43, mCustomCursorsCB->mY - mY + 24);
	g->DrawString("Hardware Acceleration", m3DCB->mX - mX + 43, m3DCB->mY - mY + 24);
	g->SetColorizeImages(false);
}

// AI Generated
void Sexy::OptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	int aWidgetY = mYesButton->mY - 34;

	if (mBackButton->mVisible)
	{
		int aButtonWidth = mWidth - mContentInsets.mLeft - mContentInsets.mRight - mButtonSidePadding * 2;
		int aButtonX = mX + mContentInsets.mLeft;

		mBackButton->Resize(aButtonX, aWidgetY, aButtonWidth, 33);
		aWidgetY -= 34;
	}

	int aHalfWidth = (mWidth - mContentInsets.mLeft - mContentInsets.mRight - 6) / 2;
	if (mWebLinkButton->mVisible)
	{
		int aButtonX = mX + mContentInsets.mLeft;
		mWebLinkButton->Resize(aButtonX, aWidgetY, aHalfWidth, 33);
		aWidgetY -= 34;
	}

	std::vector<ButtonWidget*> aBottomButtons;
	if (mHelpButton->mVisible) aBottomButtons.push_back(mHelpButton);
	if (mRegisterButton->mVisible) aBottomButtons.push_back(mRegisterButton);
	if (mCheckUpdatesButton->mVisible) aBottomButtons.push_back(mCheckUpdatesButton);

	if (aBottomButtons.size() == 1 || aBottomButtons.size() == 3)
	{
		int aButtonX = mX + mContentInsets.mLeft;
		int aButtonWidth = mWidth - mContentInsets.mLeft - mContentInsets.mRight;
		aBottomButtons[0]->Resize(aButtonX, aWidgetY, aButtonWidth, 33);
		aWidgetY -= 34;
	}
	if (aBottomButtons.size() > 1)
	{
		int aButtonX = mX + mContentInsets.mLeft;
		int aButtonWidth = (mWidth - mContentInsets.mLeft - mContentInsets.mRight - 3 * (aBottomButtons.size() - 1)) / aBottomButtons.size();

		for (size_t i = 0; i < aBottomButtons.size(); ++i)
		{
			aBottomButtons[i]->Resize(aButtonX, aWidgetY, aButtonWidth, 33);
			aButtonX += aButtonWidth + 3;
		}
	}

	int aSliderWidth = mWidth - 192;

	mMusicSlider->Resize(mX + 152, mY + 58, aSliderWidth, 33);
	mSFXSlider->Resize(mX + 152, mY + 90, aSliderWidth, 33);

	mFullscreenCB->Resize(mX + 33, mY + 128, 45, 46);
	mCustomCursorsCB->Resize(mX + 161, mY + 128, 45, 46);
	m3DCB->Resize(mX + 33, mY + 172, 45, 46);
}

int Sexy::OptionsDialog::GetPreferredHeight(int theWidth)
{
	int aPrefHght = 362;
	if (!mBackButton->mVisible)
		aPrefHght = 328;
	if (mRegisterButton->mVisible && mCheckUpdatesButton->mVisible)
		aPrefHght += 34;
	if (mWebLinkButton->mVisible)
		aPrefHght += 34;
	return aPrefHght;
}

void Sexy::OptionsDialog::WidgetClicked(int theId, bool theFlag)
{
	MoneyDialog::WidgetClicked(theId, theFlag);
	if (theId == 7)
	{
		if (!theFlag && mApp->mForceFullscreen)
		{
			mApp->DoDialog(8, true, "No Windowed Mode", "Windowed mode is only available if your desktop was running in either\n16 bit or 32 bit color mode when you started the game.\n\nIf you\'d like to run in Windowed mode then you need to quit the game and switch your desktop to 16 or 32 bit color mode.",
				"OK", BUTTONS_FOOTER);
			mFullscreenCB->SetChecked(true);
		}
	}
	else if (theId == 9 && theFlag)
	{
		if (!mApp->Is3DAccelerationSupported())
			mApp->DoDialog(14, true, "Not Supported", 
				"Hardware Acceleration cannot be enabled on this computer.\n\nYour video card does not\nmeet the minimum requirements\nfor this game.",
				"OK", BUTTONS_FOOTER);

		if (!mApp->Is3DAccelerationRecommended())
			mApp->DoDialog(14, true, "Warning",
				"Your video card may not fully support this feature.\n\nIf you experience slower performance, please disable Hardware Acceleration.",
				"OK", BUTTONS_FOOTER);
	}
}

void Sexy::OptionsDialog::CheckboxChecked(int theId, bool checked)
{
	WidgetClicked(theId, checked);
}

void Sexy::OptionsDialog::SliderVal(int theId, double theVal)
{
	if (theId == 5)
		mApp->SetMusicVolume(theVal);
	else if (theId == 6)
	{
		mApp->SetSfxVolume(theVal);
		if (!mSFXSlider->mDragging)
			mApp->PlaySample(SOUND_BUTTONCLICK);
	}
}

void Sexy::OptionsDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::OptionsDialog::ButtonDepress(int theId)
{
	MoneyDialog::ButtonDepress(theId);
	switch (theId)
	{
	case 0:
		mApp->DoRegisterDialog();
		return;
	case 1:
		mApp->SwitchToHelpScreen(false);
		return;
	case 2:
		mApp->OpenURL(mApp->GetString("weblink", "http://www.popcap.com"));
		return;
	case 3:
		break;
	case 4:
		mApp->DoUpdateCheckDialog();
		return;
	default:
		return;
	}

	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK && mApp->mBoard &&
		mApp->mBoard->mShouldSave && mApp->mGameMode != WinFishApp::GAMEMODE_SANDBOX)
	{
		mApp->DoLeaveGameDialog();
		return;
	}
	mApp->LeaveGameBoard();
}
