#include "ContinueDialog.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/DialogButton.h"
#include "Res.h"

using namespace Sexy;

Sexy::ContinueDialog::ContinueDialog(WinFishApp* theApp)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_CONTINUE_GAME, true, "CONTINUE GAME?", "", "CANCEL", Dialog::BUTTONS_FOOTER)
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE)
	{
		mDialogLines = "Do you want to continue your current game or restart the level?";
		mContinueButton = MakeDialogButton(0, this, "Continue", NULL);
		mRestartButton = MakeDialogButton(1, this, "Restart Level", NULL);
	}
	else
	{
		mDialogLines = "Do you want to continue your current game or start a new game?";
		mContinueButton = MakeDialogButton(0, this, "Continue", NULL);
		mRestartButton = MakeDialogButton(1, this, "New Game", NULL);
	}
}

Sexy::ContinueDialog::~ContinueDialog()
{
	if(mContinueButton)
		delete mContinueButton;
	if (mRestartButton)
		delete mRestartButton;
}

void Sexy::ContinueDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mContinueButton);
	theWidgetManager->AddWidget(mRestartButton);
}

void Sexy::ContinueDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mContinueButton);
	theWidgetManager->RemoveWidget(mRestartButton);
}

void Sexy::ContinueDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	int aContinueButtonX = mX + mBackgroundInsets.mLeft + mContentInsets.mLeft + mButtonSidePadding;

	int anAvailableWidth = mWidth - mBackgroundInsets.mLeft - mBackgroundInsets.mRight - 
		mContentInsets.mLeft - mContentInsets.mRight - (mButtonSidePadding * 2) - mButtonHorzSpacing;

	int aButtonWidth = anAvailableWidth / 2;

	int aButtonsY = mYesButton->mY - mContinueButton->mComponentImage->GetHeight() - 4;

	int aButtonHeight = mContinueButton->mComponentImage->GetHeight();

	mContinueButton->Resize(aContinueButtonX, aButtonsY, aButtonWidth, aButtonHeight);
	mRestartButton->Resize(aContinueButtonX + aButtonWidth + mButtonHorzSpacing, mContinueButton->mY, aButtonWidth, aButtonHeight);
}

int Sexy::ContinueDialog::GetPreferredHeight(int theWidth)
{
	return MoneyDialog::GetPreferredHeight(theWidth) + 40;
}

void Sexy::ContinueDialog::ButtonPress(int theId)
{
	MoneyDialog::ButtonPress(theId);
}

void Sexy::ContinueDialog::ButtonDepress(int theId)
{
	if (theId == 0)
	{
		mApp->EraseFile(mApp->mCurrentProfile->GetSaveGameFilePath(mApp->mGameMode, mApp->mCurrentProfile->mId));
		mApp->KillDialog(mId);
	}
	else if (theId == 1)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE)
		{
			Dialog* aDia = mApp->DoDialog(WinFishApp::DIALOG_RESTART_GAME, true, "Restart Level?", "Are you sure that you want to restart the level?", "", BUTTONS_OK_CANCEL);
			aDia->mYesButton->mLabel = "Restart";
		}
		else
		{
			Dialog* aDia = mApp->DoDialog(WinFishApp::DIALOG_RESTART_GAME, true, "New Game?", "Are you sure that you want to start a new game?", "", BUTTONS_OK_CANCEL);
			aDia->mYesButton->mLabel = "New Game";
		}
	}
	else
	{
		mApp->KillDialog(mId);
		mApp->SwitchToGameSelector();
	}
}
