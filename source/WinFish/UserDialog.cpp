#include "UserDialog.h"
#include "Res.h"
#include "SexyAppFramework/ScrollbarWidget.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/WidgetManager.h"
#include "WinFishAppCommon.h"

using namespace Sexy;

Sexy::UserDialog::UserDialog(WinFishApp* theApp, bool transferShells)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, transferShells*16 + 24, true, transferShells ? "TRANSFER SHELLS" : "WHO ARE YOU?",
		"", "", BUTTONS_OK_CANCEL)
{
	mTransferShells = transferShells;
	mListWidget = new ListWidget(0, FONT_LIDDIE12, this);
	int anListWidgetColors[][3] =
	{ {0x35, 0x2b, 0x1d},
	{0, 0, 0},
	{255, 0xe1, 0x49},
	{255, 255, 255},
	{0xbc, 0x16, 0x16} };
	mListWidget->SetColors(anListWidgetColors,5);
	mListWidget->mDrawOutline = true;
	mListWidget->mJustify = 1;
	mListWidget->mItemHeight = 15;

	if (!mListWidget)
		mScrollbarWidget = new ScrollbarWidget(0, NULL);
	else
		mScrollbarWidget = new ScrollbarWidget(0, mListWidget);

	mRenameButton = MakeDialogButton(0, this, "Rename", FONT_JUNGLEFEVER12OUTLINE);
	mDeleteButton = MakeDialogButton(1, this, "Delete", FONT_JUNGLEFEVER12OUTLINE);
	mEditWidget = MakeEditWidget(0, this);
	mEditWidget->SetText("", true);
	mEditWidget->mCursorPos = mEditWidget->mString.size();
	mListWidget->mScrollbar = mScrollbarWidget;

	if (!mTransferShells)
		mListWidget->AddLine("(Create a New User)", false);

	if (mApp->mCurrentProfile && !mTransferShells)
		mListWidget->SetSelect(mListWidget->AddLine(mApp->mCurrentProfile->mUserName, false));


	for (UserProfilesMap::iterator it = mApp->mProfileMgr->mProfilesMap->begin();
		it != mApp->mProfileMgr->mProfilesMap->end(); ++it)
	{
		UserProfile* aProf = &it->second;
		if(aProf != mApp->mCurrentProfile)
			mListWidget->AddLine(aProf->mUserName, false);
	}

	if (mApp->mProfileMgr->mProfilesMap->size() < 8)
		mScrollbarWidget->SetVisible(false);
	if (!mTransferShells)
		mEditWidget->SetVisible(false);
	else
	{
		mRenameButton->SetVisible(false);
		mDeleteButton->SetVisible(false);
	}
}

Sexy::UserDialog::~UserDialog()
{
	delete mListWidget;
	delete mScrollbarWidget;
	delete mRenameButton;
	delete mDeleteButton;
	delete mEditWidget;
}

void Sexy::UserDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	if (!mTransferShells)
		return;

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color::White);

	int aXPos = mContentInsets.mLeft + mBackgroundInsets.mLeft;
	int aYPos = mContentInsets.mTop + mBackgroundInsets.mTop + mY + 54;
	g->DrawString("Transfer Shells to User", aXPos + 70, aYPos);
	g->DrawString("Transfer Amount", aXPos, mEditWidget->mY - mY + 17);

	g->DrawImage(IMAGE_HELPSHELL, mEditWidget->mX - mX - 35, mEditWidget->mY - mY + 3);
	DrawEditWidgetBox(g, mEditWidget);
}

void Sexy::UserDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mListWidget);
	theWidgetManager->AddWidget(mScrollbarWidget);
	theWidgetManager->AddWidget(mDeleteButton);
	theWidgetManager->AddWidget(mRenameButton);
	theWidgetManager->AddWidget(mEditWidget);
	if (mTransferShells)
		theWidgetManager->SetFocus(mEditWidget);
}

void Sexy::UserDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mListWidget);
	theWidgetManager->RemoveWidget(mScrollbarWidget);
	theWidgetManager->RemoveWidget(mDeleteButton);
	theWidgetManager->RemoveWidget(mRenameButton);
	theWidgetManager->RemoveWidget(mEditWidget);
}

void Sexy::UserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);

	int aXPos = mContentInsets.mLeft + mBackgroundInsets.mLeft + mX;
	int aYPos = mContentInsets.mTop + mBackgroundInsets.mTop + mY + 54;
	int aWidth = mWidth - mContentInsets.mRight - mContentInsets.mLeft - mBackgroundInsets.mRight - mBackgroundInsets.mLeft;
	theHeight = 16;
	if (!mScrollbarWidget->mVisible)
		theHeight = 0;

	int aWidthCorrect = aWidth - 20 - theHeight;
	mListWidget->Resize(aXPos + 10, aYPos, aWidthCorrect, 130);
	mScrollbarWidget->ResizeScrollbar(aWidthCorrect + aXPos + 10, aYPos, theHeight, 130);
	mRenameButton;
	mRenameButton->Layout(4355, mYesButton);
	mDeleteButton->Layout(4355, mNoButton);
	mEditWidget->Layout(464, mYesButton, aXPos + 180, -10, aWidth - 190, 24);
}
int Sexy::UserDialog::GetPreferredHeight(int theWidth)
{
	return MoneyDialog::GetPreferredHeight(theWidth) + 190;
}

void Sexy::UserDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::UserDialog::ButtonDepress(int theId)
{
	MoneyDialog::ButtonDepress(theId);

	SexyString aStr = GetSelectedUserName();

	if (aStr.size() > 0)
	{
		if (theId == 0)
			mApp->DoRenameDialog(aStr);
		else if (theId == 1)
			mApp->DoDeleteWarningDialog(aStr);
	}
}

void Sexy::UserDialog::ListClicked(int theId, int theIdx, int theClickCount)
{
	if (mTransferShells)
	{
		mListWidget->SetSelect(theIdx);
		return;
	}
	if (theIdx == 0)
	{
		mApp->DoNewUserDialog();
		return;
	}
	mListWidget->SetSelect(theIdx);
	if (theClickCount == 2)
		mApp->UserDialogOkPressed(true);
}

SexyString Sexy::UserDialog::GetSelectedUserName()
{
	SexyString aStr = "";
	if (!mTransferShells && mListWidget->mSelectIdx < 1)
	{
		return aStr;
	}
	if (mListWidget->mSelectIdx > -1)
	{
		if (mListWidget->mSelectIdx < mListWidget->GetLineCount())
		{
			aStr.assign(mListWidget->GetStringAt(mListWidget->mSelectIdx));
			return aStr;
		}
	}
	aStr.assign("");
	return aStr;
}

int Sexy::UserDialog::GetShellsValue()
{
	int aVal = atoi(mEditWidget->mString.c_str());
	return aVal < 0 ? 0 : aVal;
}

