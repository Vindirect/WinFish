#include "UpdateCheckDialog.h"
#include "WinFishApp.h"
#include "InternetManager.h"

using namespace Sexy;

Sexy::UpdateCheckDialog::UpdateCheckDialog(Image* theComponentImage, Image* theButtonComponentImage, Image* theWaitImage, int theId) :
	Dialog(theComponentImage, theButtonComponentImage, theId, true, gSexyApp->GetString("UPDATE_CHECK_TITLE"),
		gSexyApp->GetString("UPDATE_CHECK_BODY"), gSexyApp->GetString("DIALOG_BUTTON_CANCEL"), BUTTONS_FOOTER)
{
	mWaitBarImage = theWaitImage;
	mCheckFinished = 0;
	mWaitBarXOffset = 0;
	mYOffset = 66;
}

Sexy::UpdateCheckDialog::~UpdateCheckDialog()
{
}

void Sexy::UpdateCheckDialog::Update()
{
	Dialog::Update();

	if (!mCheckFinished && mUpdateCnt % 3 == 0)
	{
		mWaitBarXOffset = (mWaitBarXOffset + 1) % 32;
		MarkDirty();
	}

	int aResCode = gSexyApp->mInternetManager->GetUpdateResultCode();
	if (mCheckFinished)
		return;

	if (aResCode != HTTPTransfer::RESULT_NOT_FOUND && aResCode != HTTPTransfer::RESULT_NOT_COMPLETED)
		return;

	mCheckFinished = true;
	if (aResCode == HTTPTransfer::RESULT_NOT_COMPLETED)
		gSexyApp->mInternetManager->StartAdLoading();

	Dialog* aDia = NULL;
	if (aResCode == HTTPTransfer::RESULT_NOT_FOUND || gSexyApp->mInternetManager->IsUpToDate())
		aDia = ((WinFishApp*)gSexyApp)->DoDialog(mId + 20000, true,gSexyApp->GetString("UP_TO_DATE_TITLE"), 
			gSexyApp->GetString("UP_TO_DATE_BODY"), gSexyApp->GetString("DIALOG_BUTTON_OK"), BUTTONS_FOOTER);
	else
		aDia = ((WinFishApp*)gSexyApp)->DoDialog(mId + 10000, true, gSexyApp->GetString("NEW_VERSION_TITLE"),
			gSexyApp->GetString("NEW_VERSION_BODY"), "", BUTTONS_YES_NO);

	aDia->Move(mX + 32, mY - 32);
}

void Sexy::UpdateCheckDialog::Draw(Graphics* g)
{
	Dialog::Draw(g);

	Rect aClipRect = Rect(mContentInsets.mLeft + 24, mHeight - mYOffset - mContentInsets.mBottom,
		mWidth - mContentInsets.mRight - mContentInsets.mLeft - 48, 16);

	Graphics g2(*g);

	g2.ClipRect(aClipRect);

	int aNumTiles = (aClipRect.mWidth / mWaitBarImage->GetWidth()) + 2;
	int aX = 0;
	for (int i = 0; i < aNumTiles; i++)
	{
		g2.DrawImage(mWaitBarImage, aX - mWaitBarXOffset + aClipRect.mX,aClipRect.mY);
		aX += mWaitBarImage->GetWidth();
	}

	g->SetColor(Color::Black);
	g->DrawRect(aClipRect.mX - 1, aClipRect.mY - 1, aClipRect.mWidth + 1, aClipRect.mHeight + 1);
}

int Sexy::UpdateCheckDialog::GetPreferredHeight(int theWidth)
{
	return Dialog::GetPreferredHeight(theWidth) + 32;
}

