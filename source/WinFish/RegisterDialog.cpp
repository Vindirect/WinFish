#include "RegisterDialog.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"

#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::RegisterDialog::RegisterDialog(WinFishApp* theApp) :
	MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_REGISTER, true, "Register Insaniquarium",
		"You must obtain a registration code online and enter it below.", "", BUTTONS_OK_CANCEL)
{
	mApp = theApp;
	mYesButton->mLabel = "Register";
	mNoButton->mLabel = "Cancel";

	mGetCodeButton = MakeDialogButton(0, this, "CLICK HERE TO GET REGISTRATION CODE", nullptr);
	mNameEditWidget = MakeEditWidget(0, this);
	mNameEditWidget->SetColor(EditWidget::COLOR_TEXT, Color::White);

	mCodeEditWidget = MakeEditWidget(1, this);
	mCodeEditWidget->SetColor(EditWidget::COLOR_TEXT, Color::White);

	mNameEditWidget->mTabPrev = mCodeEditWidget;
	mNameEditWidget->mTabNext = mNameEditWidget->mTabPrev;

	mCodeEditWidget->mTabPrev = mNameEditWidget;
	mCodeEditWidget->mTabNext = mCodeEditWidget->mTabPrev;
}

Sexy::RegisterDialog::~RegisterDialog()
{
	if (mNameEditWidget)
		delete mNameEditWidget;
	if (mCodeEditWidget)
		delete mCodeEditWidget;
	if (mGetCodeButton)
		delete mGetCodeButton;
}

void Sexy::RegisterDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mGetCodeButton);
	theWidgetManager->AddWidget(mNameEditWidget);
	theWidgetManager->AddWidget(mCodeEditWidget);
	theWidgetManager->SetFocus(mNameEditWidget);
}

void Sexy::RegisterDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mGetCodeButton);
	theWidgetManager->RemoveWidget(mCodeEditWidget);
	theWidgetManager->RemoveWidget(mNameEditWidget);
}

void Sexy::RegisterDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color(0xffff00));
	g->DrawString("YOUR NAME", mNameEditWidget->mX - mX, mNameEditWidget->mY - mY - 10);
	g->DrawString("YOUR REGISTRATION CODE", mCodeEditWidget->mX - mX, mCodeEditWidget->mY - mY - 10);
	DrawEditWidgetBox(g, mNameEditWidget);
	DrawEditWidgetBox(g, mCodeEditWidget);
}

void Sexy::RegisterDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	mGetCodeButton->Resize(theX + 36, theY + 90, theWidth - 75, 33);
	int anY = theY + 135 + mGetCodeButton->mHeight;
	mNameEditWidget->Resize(theX + 52, anY, theWidth - 107, 24);
	mCodeEditWidget->Resize(theX + 52, anY + mNameEditWidget->mHeight + 40, theWidth - 107, 24);
}

int Sexy::RegisterDialog::GetPreferredHeight(int theWidth)
{
	return MoneyDialog::GetPreferredHeight(theWidth) + 190;
}

void Sexy::RegisterDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::RegisterDialog::ButtonDepress(int theId)
{
	if (theId == 0)
		mApp->OpenRegisterPage();
	else
		MoneyDialog::ButtonDepress(theId);
}

void Sexy::RegisterDialog::EditWidgetText(int theId, const SexyString& theString)
{
	if (theId == 0)
		mWidgetManager->SetFocus(mCodeEditWidget);
	else if (theId == 1)
		ButtonDepress(1000);
}
