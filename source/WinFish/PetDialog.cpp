#include "PetDialog.h"
#include "Res.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::PetDialog::PetDialog(WinFishApp* theApp)
	: MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, WinFishApp::DIALOG_PET_DIALOG, false, "PRESTO CHANGE-O", "", "CANCEL", Dialog::BUTTONS_FOOTER)
{
	for (int i = 0; i < GameObject::PET_END; i++)
		mRects[i] = Rect(0, 0, 0, 0);

	mContentInsets.mLeft = 100;
	mContentInsets.mRight = 100;
	mContentInsets.mBottom = 10;
	Resize(0, 0, 640, 480);
	m0x158 = -1;
	m0x15c = -1;

	int j = 0;
	int aY = 120;
	for (; aY < 470; aY += 50)
	{
		mRects[j] = Rect(10, aY, 50, 50);
		j++;
	}
	aY -= 50;
	int aX = 62;
	for (; aX < 634; aX += 52)
	{
		mRects[j] = Rect(aX, aY, 50, 50);
		j++;
	}
	aX -= 52;
	for (; j < GameObject::PET_END; j++)
	{
		aY -= 50;
		mRects[j] = Rect(aX, aY, 50, 50);
	}

	for (int i = 0; i < GameObject::PET_END; i++)
		mPetsUnlocked[i] = mApp->mCurrentProfile->IsPetUnlocked(i);
}

Sexy::PetDialog::~PetDialog()
{
}

void Sexy::PetDialog::Draw(Graphics* g)
{
	g->SetColor(Color(0xb0000000));
	g->FillRect(0, 0, 70, 480);
	g->FillRect(570, 0, 70, 480);
	g->FillRect(70, 0, 500, 70);
	g->FillRect(70, 410, 500, 70);

	g->SetColor(Color::White);
	g->DrawRect(0, 0, 639, 479);
	g->DrawRect(70, 70, 500, 340);

	g->SetColor(Color(0xffff00));
	g->SetFont(FONT_JUNGLEFEVER12OUTLINE);

	int anAscent = g->GetFont()->GetAscent();
	WriteCenteredLine(g, anAscent + 5, "PRESTO CHANGE-O");

	for (int i = 0; i < GameObject::PET_END; i++)
	{
		if (mPetsUnlocked[i])
		{
			int anImgId = IMAGE_PETBTNU_ID;
			if(i == m0x15c || i == m0x158)
				anImgId = IMAGE_PETBTNO_ID;

			Image* anImg = GetImageById(anImgId);
			g->DrawImage(anImg, mRects[i].mX, mRects[i].mY);
			g->DrawImage((Image*) mApp->mPrestoMenuData->mPetImages[i], mRects[i].mX + 5, mRects[i].mY + 5);
		}
	}
}

void Sexy::PetDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	mYesButton->Resize(270, 30, 100, mYesButton->mHeight);
}

void Sexy::PetDialog::MouseLeave()
{
	if (m0x15c == -1 && m0x158 == -1)
		return;

	m0x15c = -1;
	m0x158 = -1;

	MarkDirty();
}

void Sexy::PetDialog::MouseMove(int x, int y)
{
	int anId = MouseAtHandle(x, y);
	if (anId != m0x158)
	{
		m0x158 = anId;
		MarkDirty();
	}
}

void Sexy::PetDialog::MouseDown(int x, int y, int theClickCount)
{
	int anId = MouseAtHandle(x, y);
	m0x15c = anId;
	m0x158 = anId;
	MarkDirty();
}

void Sexy::PetDialog::MouseUp(int x, int y, int theClickCount)
{
	int anId = MouseAtHandle(x, y);
	int prevId = m0x15c;
	m0x15c = anId;
	if (prevId == m0x15c && prevId != -1)
		mApp->ApplyPrestoMorph(prevId);
	m0x15c = -1;
	MarkDirty();
}

bool Sexy::PetDialog::IsPointVisible(int x, int y)
{
	if (y < 0)
		return false;
	if (x > 70 && x < 570 && y > 70 && y < 410)
		return false;
	return true;
}

int Sexy::PetDialog::MouseAtHandle(int x, int y)
{
	int theId = 0;
	while (x < mRects[theId].mX || x >= (mRects[theId].mX + mRects[theId].mWidth) ||
		y < mRects[theId].mY || y >= (mRects[theId].mY + mRects[theId].mHeight))
	{
		theId++;
		if (theId >= GameObject::PET_END)
			return -1;
	}
	if (!mPetsUnlocked[theId])
		return -1;
	return theId;
}

void Sexy::PetDialog::ButtonPress(int theId)
{
	MoneyDialog::ButtonPress(theId);
}

void Sexy::PetDialog::ButtonDepress(int theId)
{
	MoneyDialog::ButtonDepress(theId);
}