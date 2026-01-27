#include "Warp.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

Sexy::Warp::Warp()
{
	mType = TYPE_WARP;
}

Sexy::Warp::Warp(int theX, int theY)
{
	mType = TYPE_WARP;
	mX = theX;
	mY = theY;
	mWidth = 100;
	mHeight = 220;
	mExistenceCounter = 36;
	mMouseVisible = false;
}

void Sexy::Warp::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (mExistenceCounter < 1)
	{
		mApp->mBoard->mWidgetManager->RemoveWidget(this);
		mApp->SafeDeleteWidget(this);
		mApp->mBoard->RemoveGameObjectFromLists(this, false);
	}
	mExistenceCounter--;
}

void Sexy::Warp::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	g->DrawImageCel(IMAGE_WARPHOLE, 20, 0, 17 - mExistenceCounter / 2);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->DrawImageCel(IMAGE_WARPGLOW, 0, 0, 17 - mExistenceCounter / 2);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Sexy::Warp::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncLong(mExistenceCounter);
}
