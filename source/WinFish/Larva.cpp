#include "Larva.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::Larva::Larva()
{
	mType = TYPE_LARVA;
}

Sexy::Larva::Larva(int theX, int theY)
{
	mType = TYPE_LARVA;
	mXD = theX;
	mYD = theY;
	mX = theX;
	mY = theY;
	mWidth = 72;
	mHeight = 72;
	mMouseVisible = false;
	mDoFinger = false;
	mApp->mSeed->Next();
	mVY = 1.0;
	mAnimFrame = 0;
	mPickedUp = false;
	m0x175 = false;
}

void Sexy::Larva::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (!mMouseVisible && mYD < 320.0 && !m0x175)
	{
		mMouseVisible = true;
		mDoFinger = true;
	}
	if (!mPickedUp)
	{
		mYD -= mVY;
		if (mYD < 64.0)
		{
			Remove();
			return;
		}
		if (mXD > 530.0)
			mXD = 530.0;
	}
	else
	{
		if (mXD < 550.0)
			mXD += (550.0 - mXD) / 7.0;
		else if (mXD > 550.0)
			mXD -= (mXD - 550.0) / 7.0;

		if (mYD < 30.0)
			mYD += (30.0 - mYD) / 7.0;
		else if (mYD > 30.0)
			mYD -= (mYD - 30.0) / 7.0;

		if (mY < 40)
		{
			Remove();
			mApp->mBoard->m0x4a8++;
			mApp->mBoard->Unk07(GetValue());
			return;
		}
	}
	Move(mXD, mYD);
	mAnimFrame = (mAnimFrame + 1) % 10;
}

void Sexy::Larva::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	g->DrawImageCel(IMAGE_MONEY, 0, 0, mAnimFrame, 5);
}

void Sexy::Larva::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount > -1)
	{
		mPickedUp = true;
		mApp->mBoard->PlayPointsSound();
	}
	else
	{
		mApp->mBoard->CheckMouseDown(mX + x, mY + y);
	}
}

void Sexy::Larva::Remove()
{
	mApp->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
}

void Sexy::Larva::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVY);
	theSync->SyncLong(mAnimFrame);
	theSync->SyncBool(mPickedUp);
	theSync->SyncBool(m0x175);
}

int Sexy::Larva::GetValue()
{
	return mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK ? 150 : 7;	
}
