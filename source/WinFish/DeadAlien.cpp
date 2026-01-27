#include "DeadAlien.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

Sexy::DeadAlien::DeadAlien()
{
	mType = TYPE_DEAD_ALIEN;
}

Sexy::DeadAlien::DeadAlien(int theX, int theY, int theAlienType, int theAnimIndex, bool facingRight)
{
	mType = TYPE_DEAD_ALIEN;
	mX = theX;
	mY = theY;
	mXD = theX;
	mYD = theY;
	mVX = 0;
	mVY = 0;
	mAlienType = theAlienType;
	mWidth = 160;
	mHeight = 160;
	mTimer = 125;
	m0x19c = 5;
	mMouseVisible = false;
	mFacingRight = facingRight;
	mDeathAnimationIdx = theAnimIndex;
	mApp->mBoard->mWidgetManager->AddWidget(this);
	mOpacity = 1.0;
}

void Sexy::DeadAlien::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (mTimer < 1)
	{
		RemoveDeadAlien();
		return;
	}

	mTimer--;
	if (mTimer < 105)
	{
		mOpacity -= 0.02;
		if (mOpacity < 0.0)
			mOpacity = 0.0;
	}

	if (mTimer % 5 == 0 && mTimer > 50 && DeadAlienUnk01())
	{
		int anInt = InterpolateInt(25, 100, mTimer - 50, 75, false);
		if (anInt < 25)
			anInt = 25;
		else if (anInt > 100)
			anInt = 100;
		mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, anInt / 100.0);
	}

	if (mTimer % 7 == 0 && mTimer > 50)
	{
		mApp->mBoard->SpawnBubble(mApp->mSeed->Next() % 70 + 30 + mX, mApp->mSeed->Next() % 70 + 30 + mY);
		mApp->mBoard->SpawnBubble(mApp->mSeed->Next() % 50 + 50 + mX, mApp->mSeed->Next() % 50 + 50 + mY);

		mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 50 + 20 + mX, mApp->mSeed->Next() % 50 + 20 + mY, mApp->mSeed->Next() % 3 + 3);
		mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 30 + 40 + mX, mApp->mSeed->Next() % 30 + 40 + mY, mApp->mSeed->Next() % 3 + 3);

		mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 50 + 20 + mX, mApp->mSeed->Next() % 50 + 20 + mY, (mApp->mSeed->Next() & 1) + 6);
		mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 30 + 40 + mX, mApp->mSeed->Next() % 30 + 40 + mY, (mApp->mSeed->Next() & 1) + 6);
	}

	if (mVX <= 0.0)
		mVX += 0.03;
	else
		mVX -= 0.03;

	if (mVY < 2.0)
		mVY += 0.05;
	if (mXD > 490.0)
		mXD = 490.0;
	if (mXD < -10.0)
		mXD = -10.0;

	if ((mAlienType == Alien::ALIEN_DESTRUCTOR || mAlienType == Alien::ALIEN_ULYSEES) && mYD > 280.0)
		mYD = 280.0;

	mXD += mVX;
	mYD += mVY;
	Move(mXD, mYD);
}

void Sexy::DeadAlien::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	g->SetColorizeImages(true);
	g->SetColor(Color(255, 255, 255, mOpacity * 255));
	int anImgId = 0;
	if (mAlienType == Alien::ALIEN_WEAK_SYLV || mAlienType == Alien::ALIEN_STRONG_SYLV)
		anImgId = IMAGE_SYLV_ID;
	else if(mAlienType == Alien::ALIEN_BALROG)
		anImgId = IMAGE_BALROG_ID;
	else if(mAlienType == Alien::ALIEN_DESTRUCTOR)
		anImgId = IMAGE_DESTRUCTOR_ID;
	else if(mAlienType == Alien::ALIEN_ULYSEES)
		anImgId = IMAGE_ULYSSES_ID;
	else if(mAlienType == Alien::ALIEN_PSYCHOSQUID)
		anImgId = IMAGE_PSYCHOSQUID_ID;
	else if(mAlienType == Alien::ALIEN_CYRAX)
		anImgId = IMAGE_BOSS_ID;

	Rect aSrcRect(mDeathAnimationIdx * 160, 0, 160, 160);
	Image* anAlienImg = GetImageById(anImgId);
	g->DrawImageMirror(anAlienImg, 0, 0, aSrcRect, mFacingRight);
	if (mTimer > 115)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColor(Color(255, 255, 255, (mTimer - 115) * 20));
		for (int i = 0; i < 5; i++)
		{
			g->DrawImageMirror(anAlienImg, 0, 0, aSrcRect, mFacingRight);
		}
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}
	g->SetColorizeImages(false);
}

void Sexy::DeadAlien::Remove()
{
	RemoveDeadAlien();
}

void Sexy::DeadAlien::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncBool(mFacingRight);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncDouble(m0x180);
	theSync->SyncLong(mDeathAnimationIdx);
	theSync->SyncDouble(mOpacity);
	theSync->SyncLong(mTimer);
	theSync->SyncLong(m0x19c);
	theSync->SyncLong(mAlienType);
}

void Sexy::DeadAlien::RemoveDeadAlien()
{
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
}

bool Sexy::DeadAlien::DeadAlienUnk01()
{
	int aMaxNum = 0;
	for (int i = 0; i < mApp->mBoard->mDeadAlienList->size(); i++)
	{
		DeadAlien* anAlien = mApp->mBoard->mDeadAlienList->at(i);
		if (aMaxNum < anAlien->mTimer)
			aMaxNum = anAlien->mTimer;
	}
	return aMaxNum <= mTimer;
}
