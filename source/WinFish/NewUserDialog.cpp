#include "NewUserDialog.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Res.h"

using namespace Sexy;

Sexy::NewUserDialog::NewUserDialog(WinFishApp* theApp, bool theRename)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, theRename * 2 + WinFishApp::DIALOG_NEW_USER, true, theRename ? "RENAME USER" : "NEW USER",
		"\nPlease enter your name.", "", BUTTONS_OK_CANCEL)
{
	SetButtonFont(FONT_JUNGLEFEVER12OUTLINE);
	SetHeaderFont(FONT_JUNGLEFEVER15OUTLINE);
	SetLinesFont(FONT_JUNGLEFEVER10OUTLINE);
	mContentInsets = Insets(36, 15, 36, 36);
	SetColor(COLOR_HEADER, Color(0xff, 200, 0));
	SetColor(COLOR_LINES, Color::White);
	SetColor(COLOR_BUTTON_TEXT, Color::White);
	mApp2 = theApp;
	mEditWidget = MakeEditWidget(0, this);
	mEditWidget->mMaxChars = 12;
	mEditWidget->AddWidthCheckFont(FONT_JUNGLEFEVER10OUTLINE, 150);
	mEditWidget->AddWidthCheckFont(FONT_BLAMBOTPRO15, 220);
}

Sexy::NewUserDialog::~NewUserDialog()
{
	if (mEditWidget)
		delete mEditWidget;
}

void Sexy::NewUserDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	DrawEditWidgetBox(g, mEditWidget);
}

void Sexy::NewUserDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mEditWidget);
	theWidgetManager->SetFocus(mEditWidget);
}

void Sexy::NewUserDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mEditWidget);
}

int Sexy::NewUserDialog::GetPreferredHeight(int theWidth)
{
	return MoneyDialog::GetPreferredHeight(theWidth) + 40;
}

void Sexy::NewUserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	int anEditWidgetX = mX + 48;
	int anEditWidgetWidth = mWidth - 96;

	int anEditWidgetY = (mY + mHeight) - 110;
	int anEditWidgetHeight = 24;

	mEditWidget->Resize(anEditWidgetX, anEditWidgetY, anEditWidgetWidth, anEditWidgetHeight);
}

void Sexy::NewUserDialog::EditWidgetText(int theId, const SexyString& theString)
{
	mApp->ButtonDepress(mId + 2000);
}

bool Sexy::NewUserDialog::AllowChar(int theId, SexyChar theChar)
{
	if (isalnum(theChar))
		return true;
	if (theChar == ' ')
		return true;
	return false;
}

SexyString Sexy::NewUserDialog::CleanString()
{
	SexyString& anOriginalStr = mEditWidget->mString;

	SexyString aNewStr = "";

	char aLastChar = ' ';

	for (size_t i = 0; i < anOriginalStr.length(); ++i)
	{
		char aCurrentChar = anOriginalStr[i];

		if (aCurrentChar == ' ')
		{
			if (aLastChar != ' ')
				aNewStr.append(1, ' ');
		}
		else
			aNewStr.append(1, aCurrentChar);
		aLastChar = aCurrentChar;
	}

	if (!aNewStr.empty() && aNewStr.back() == ' ')
		aNewStr.pop_back();

	return aNewStr;
}
