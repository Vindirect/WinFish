#include "MessageWidget.h"
#include "WinFishApp.h"
#include "Res.h"
#include "Board.h"

#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::MessageWidget::MessageWidget(WinFishApp* theApp, SexyString theMessage)
{
	m0xc8 = 0;
	mColor1 = Color(0xb4, 0xfa, 0x5a, 0xff);
	mColor2 = Color(0, 0x4b, 0, 0xff);
	mApp = theApp;
	mFont = FONT_CONTINUUMBOLD14OUTLINE;
	mMessage = theMessage;
	mX = 90;
	mY = 445;
	mWidth = 460;
	mHeight = mFont->GetHeight() + 6;
	m0xd0 = 185;
	mMouseVisible = false;
	m0xd8 = -1;
	m0xd4 = false;
}

Sexy::MessageWidget::~MessageWidget()
{
}

void Sexy::MessageWidget::Update()
{
	Board* aBoard = mApp->mBoard;
	if (aBoard && !aBoard->mPause && m0xd0 > 0)
		m0xd0--;
}

void Sexy::MessageWidget::Draw(Graphics* g)
{
	if (m0xd0 <= 0)
		return;

	int aFontHeight = mFont->GetHeight();
	int aHeight = mHeight;
	int aFontAscent = mFont->GetAscent();

	int aY = aFontAscent + (aHeight - aFontHeight) / 2;

	int aStrWidth = mFont->StringWidth(mMessage);
	int aX = (mWidth - aStrWidth) / 2 + 2;

	int aTimer = m0xd0 % 32;

	if (aTimer < 27 || m0xd4 == 0)
	{
		g->SetColor(mColor2);
		g->SetFont(mFont);
		g->DrawString(mMessage, aX, aY);
		g->SetColor(mColor1);
		g->SetFont(FONT_CONTINUUMBOLD14);
		g->DrawString(mMessage, aX, aY);
	}
}

void Sexy::MessageWidget::Sync(DataSync& theSync)
{
	theSync.SyncString(mMessage);
	theSync.SyncColor(mColor1);
	theSync.SyncColor(mColor2);
	theSync.SyncLong(m0xc8);
	theSync.SyncLong(m0xd0);
	theSync.SyncBool(m0xd4);
	theSync.SyncLong(m0xd8);
}
