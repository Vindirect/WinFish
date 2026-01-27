#include "VirtualDialog.h"
#include "Res.h"
#include "WinFishApp.h"
#include "SexyAppFramework/DialogButton.h"

using namespace Sexy;

Sexy::VirtualDialog::VirtualDialog(WinFishApp* theApp)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_VIRTUAL, true, "WELCOME TO VIRTUAL TANK!", "", "OK", BUTTONS_FOOTER)
{
	mFishBoxRect = Rect(30, 85, 490, 220);
	mWidth = 550;
	mHeight = 400;
	mBubbleMgr = new BubbleMgr();
	mBubbleMgr->mBubbleBounds = Rect(mFishBoxRect.mX + 20, mFishBoxRect.mY,
		mFishBoxRect.mWidth - 40, mFishBoxRect.mHeight - 10);
	mBubbleMgr->SetBubbleConfig(10, 3);
	mBubbleMgr->UpdateALot();
}

Sexy::VirtualDialog::~VirtualDialog()
{
	if (mBubbleMgr)
		delete mBubbleMgr;
}

void Sexy::VirtualDialog::Update()
{
	mBubbleMgr->Update();
	MarkDirty();
}

void Sexy::VirtualDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);

	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	g->SetColor(Color(0xffff88));

	Rect aStrRect = Rect(30, 55, mWidth - 60, 0);

	WriteWordWrapped(g, aStrRect, "Customize your very own virtual aquarium!", -1, 0);

	g->DrawImageBox(mFishBoxRect, IMAGE_FISHBOX);
	mBubbleMgr->Draw(g);
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color::White);

	g->DrawImageCel(IMAGE_TROPHYBUTTONS, 60, mFishBoxRect.mY + 23, 2);

	aStrRect.mX = mFishBoxRect.mX + 100;
	aStrRect.mY = mFishBoxRect.mY + 35;
	aStrRect.mWidth = mFishBoxRect.mWidth - 120;
	aStrRect.mHeight = mFishBoxRect.mHeight;
	WriteWordWrapped(g, aStrRect, "Buy all sorts of fish in the Store.  Virtual fish never die, but try to take good care of them.", -1, -1);

	aStrRect.mY += 60;
	g->DrawImage(IMAGE_HELPSHELL, 72, aStrRect.mY);

	WriteWordWrapped(g, aStrRect, "Use shells to buy fish -- you can earn shells in every game mode!", -1, -1);

	g->DrawImageCel(IMAGE_TROPHYBUTTONS, 60, aStrRect.mY + 48, 5);
	aStrRect.mY += 60;

	WriteWordWrapped(g, aStrRect, "Make Virtual Tank your screensaver!  Click on the Tank button to set it up.", -1, -1);

	g->SetFont(FONT_TINY);
	g->SetColor(Color(0xaaffaa));

	g->DrawString("STORE", 67, aStrRect.mY - 80);

	g->DrawString("SHELLS", 64, aStrRect.mY - 28);

	g->DrawString("TANK", 68, aStrRect.mY + 35);
}

void Sexy::VirtualDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	DialogButton* aBtn = mYesButton;
	aBtn->Resize(mX + (mWidth - 200) / 2, aBtn->mY, 200, aBtn->mHeight);
}
