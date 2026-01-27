#include "FishNamingDialog.h"
#include "WinFishApp.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "WinFishAppCommon.h"
#include "Res.h"

Sexy::FishNamingDialog::FishNamingDialog(WinFishApp* theApp, const SexyString& theLine)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_FISH_NAMING, true, "Name Your Fish", theLine, "", BUTTONS_OK_CANCEL)
{
	mApp2 = theApp;
	m0x164 = 12345;

	SetButtonFont(FONT_JUNGLEFEVER12OUTLINE);
	SetHeaderFont(FONT_JUNGLEFEVER15OUTLINE);
	SetLinesFont(FONT_JUNGLEFEVER10OUTLINE);
	mContentInsets = Insets(36,15,36,36);
	SetColor(COLOR_HEADER, Color(255, 200, 0));
	SetColor(COLOR_LINES, Color::White);
	SetColor(COLOR_BUTTON_TEXT, Color::White);
	mEditWidget = MakeEditWidget(0, this);
	mEditWidget->mMaxChars = 12;
	mEditWidget->mMaxPixels = 170;
	mEditWidget->AddWidthCheckFont(FONT_JUNGLEFEVER15OUTLINE, -1);
}

Sexy::FishNamingDialog::~FishNamingDialog()
{
	if (mEditWidget)
		mApp2->mBoard->mWidgetManager->RemoveWidget(mEditWidget);
	if (mEditWidget)
		delete mEditWidget;
}

void Sexy::FishNamingDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mEditWidget);
	theWidgetManager->SetFocus(mEditWidget);
}

void Sexy::FishNamingDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mEditWidget);
}

void Sexy::FishNamingDialog::Draw(Graphics* g)
{
	Dialog::Draw(g);
	DrawEditWidgetBox(g, mEditWidget);
	if (m0x168)
	{
		int availableWidth = mWidth
			- mContentInsets.mRight
			- mBackgroundInsets.mLeft
			- mBackgroundInsets.mRight
			- mContentInsets.mLeft
			- 4;
		int startX = mBackgroundInsets.mLeft + mContentInsets.mLeft + 2;

		Rect aWrappedRect = Rect(startX, 120, availableWidth, 0);
		SexyString aStr = "Note: All fish come with a one day\nmoney back guarantee.";
		
		WriteWordWrapped(g, aWrappedRect, aStr, mLinesFont->GetLineSpacing() + mLineSpacingOffset, mTextAlign);
	}
}

void Sexy::FishNamingDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	mEditWidget->Resize(mX + 48, mY + 80, mWidth - 96, 24);
}

int Sexy::FishNamingDialog::GetPreferredHeight(int theWidth)
{
	int aHght = MoneyDialog::GetPreferredHeight(theWidth);
	int aPrefHght = aHght + 56;
	if (m0x168)
		aPrefHght = aHght + 76;
	return aPrefHght;
}

void Sexy::FishNamingDialog::EditWidgetText(int theId, const SexyString& theString)
{
	mApp->ButtonDepress(mId + 2000);
}

bool Sexy::FishNamingDialog::AllowChar(int theId, SexyChar theChar)
{
	int isPrint = isprint(theChar);
	if (isPrint == 0 && theChar != ' ')
		return false;
	return true;
}
