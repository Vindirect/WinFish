#include "DeadFish.h"
#include "WinFishApp.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Res.h"

using namespace Sexy;

Sexy::DeadFish::DeadFish()
{
	mType = TYPE_DEAD_FISH;
}

Sexy::DeadFish::DeadFish(int theX, int theY, double theVX, double theVY, double theSpeedMod, int theObjType, bool facingRight) 
	: GameObject()
{
	mType = TYPE_DEAD_FISH;
	mX = theX;
	mY = theY;
	mXD = theX;
	mYD = theY;
	mObjType = theObjType;
	mVX = theVX;

	if (theX < 115 || theVY < -3.0)
		mVY = theVY - 1.0;
	else
		mVY = theVY - 2.0;

	mSpeedMod = theSpeedMod;
	mWidth = 80;
	mHeight = 80;
	if (theObjType == TYPE_ULTRA)
	{
		mWidth = 160;
		mHeight = 160;
	}

	m0x1a0 = 125;
	mMouseVisible = false;
	mFlipped = facingRight;
	m0x18c = 0;
	m0x190 = 100;
	m0x188 = false;
	mApp->mBoard->mWidgetManager->AddWidget(this); // Weird, being called 2 times for the same object
	m0x198 = 1.0;
}

void Sexy::DeadFish::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();

	if (m0x190 >= 0 && m0x190 <= 10)
	{
		m0x190--;
		m0x18c = m0x190;
		if (m0x190 == 10)
			m0x190 = 10; // ??? WHAT
	}
	else
	{
		if (mObjType == 9 || mObjType == 8)
		{
			if (m0x1a0 > 105)
				m0x18c = 9 - (m0x1a0 - 106) / 2;
			else
				m0x18c = 9;
		}
		else
		{
			if (m0x1a0 < 106)
			{
				if (m0x1a0 == 104 || m0x1a0 == 103)
					m0x18c = 8;
				else if (m0x1a0 == 102 || m0x1a0 == 101)
					m0x18c = 7;
				else
					m0x18c = 9 + (m0x1a0 <= 100 ? -3 : 0);
			}
			else
				m0x18c = 9 - (m0x1a0 - 106) / 2;
		}
	}

	if (m0x1a0 < 105)
	{
		m0x198 -= 0.02;
		if (m0x198 < 0.0)
			m0x198 = 0;
		if (mShadowPtr)
			mShadowPtr->m0x168 = m0x198;
	}
	if (m0x1a0 <= 0)
	{
		RemoveDeadFish();
		return;
	}

	if (m0x190 < 1) // Ressurection (Angel Pet)
	{
		mApp->mBoard->PlaySample(SOUND_HEAL_ID, 3, 1.0);
		RemoveDeadFish();
		if (mObjType == TYPE_GEKKO)
			mApp->mBoard->RessurectGekko(mX, mY, mFlipped);
		else if (mObjType == TYPE_OSCAR)
			mApp->mBoard->RessurectOscar(mX, mY, mFlipped);
		else if (mObjType == TYPE_GRUBBER)
			mApp->mBoard->RessurectGrubber(mX);
		else if (mObjType == TYPE_PENTA)
			mApp->mBoard->RessurectPenta(mX);
		else if (mObjType >= TYPE_BREEDER && mObjType <= TYPE_BREEDER + 2)
			mApp->mBoard->RessurectBreeder(mX, mY, mObjType - TYPE_BREEDER, mFlipped);
		else if (mObjType == TYPE_ULTRA)
			mApp->mBoard->RessurectUltra(mX, mY, mFlipped);
		else
			mApp->mBoard->RessurectFish(mX, mY, mObjType, mFlipped);

		return;
	}

	if ((mObjType == TYPE_ULTRA && mYD > 300.0) ||
		((mObjType == TYPE_GRUBBER || mObjType == TYPE_PENTA) && mYD > 365.0) ||
		(mYD > 370.0 || m0x1a0 > 105))
	{
		m0x1a0--;
	}

	if (mVX < 0.0)
	{
		mVX += 0.03;
		if (mVX > 0.0)
			mVX = 0;
	}
	else if (mVX > 0.0)
	{
		mVX -= 0.03;
		if (mVX < 0.0)
			mVX = 0;
	}

	if (!m0x188)
	{
		if (mVY < 2.0)
			mVY += 0.05;
	}
	else
	{
		if (mVY < -1.5)
			mVY += 0.05;
		else if (mVY > -1.5)
			mVY -= 0.05;
	}

	mXD += mVX / mSpeedMod;
	mYD += mVY / mSpeedMod;
	if (mXD > 540.0)
		mXD = 540.0;
	if (mXD < 10.0)
		mXD = 10.0;

	if (mObjType == TYPE_PENTA || mObjType == TYPE_GRUBBER)
	{
		if(mYD > 370.0)
			mYD = 370.0;
	}
	else if (mObjType == TYPE_ULTRA)
	{
		if (mYD > 310.0)
			mYD = 310.0;
	}
	else
	{
		if (mYD > 380.0)
			mYD = 380.0;
	}

	if (mYD < 85.0)
		mYD = 85.0;

	Move(mXD, mYD);
}

void Sexy::DeadFish::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	g->SetColorizeImages(true);
	g->SetColor(Color(255, 255, 255, m0x198 * 255));
	
	int anYVal = 0;
	if (m0x1a0 < 90)
		anYVal = (90 - m0x1a0) / 2;
	Image* aDrawImage = nullptr;
	bool flag = mFlipped;
	Rect theSrcRect = Rect(0,0,0,0);
	if (mObjType == TYPE_GEKKO)
	{
		aDrawImage = IMAGE_GEKKO;
		theSrcRect = Rect(m0x18c * 80, 400, 80, 80);
	}
	else if (mObjType == TYPE_ULTRA)
	{
		if (m0x1a0 >= 90 && m0x18c < 5) // 97
		{
			double aRed = (m0x18c * 5 + 1250) / 5;
			double aGreen = (m0x18c * 40 + 1075) / 5;
			double aBlue = (m0x18c * 160 + 475) / 5;
			g->SetColor(Color(aRed, aGreen, aBlue, 255));
		}

		theSrcRect = Rect(m0x18c * 160, 480, 160, 160);
		aDrawImage = IMAGE_ULTRA;
	}
	else if (mObjType == TYPE_GRUBBER)
	{
		flag = false;
		if (m0x1a0 >= 90 && m0x18c < 5)
		{
			double aRed = (m0x18c * 5 + 1250) / 5;
			double aGreen = (m0x18c * 40 + 1075) / 5;
			double aBlue = (m0x18c * 160 + 475) / 5;
			g->SetColor(Color(aRed, aGreen, aBlue, 255));
		}
		aDrawImage = IMAGE_GRUBBER;
		theSrcRect = Rect(m0x18c * 80, 240, 80, 80);
	}
	else if (mObjType == TYPE_PENTA)
	{
		flag = false;
		theSrcRect = Rect(m0x18c * 80, 160, 80, 80);
		aDrawImage = IMAGE_STARCATCHER;
	}
	else if (mObjType == TYPE_OSCAR)
	{
		theSrcRect = Rect(m0x18c * 80, 320, 80, 80);
		aDrawImage = IMAGE_SMALLDIE;
	}
	else if (mObjType < TYPE_OSCAR)
	{
		int aRow = mObjType;
		if (mObjType > TYPE_BIG_GUPPY) 
			aRow--;
		theSrcRect = Rect(m0x18c * 80, aRow * 80, 80, 80);
		aDrawImage = IMAGE_SMALLDIE;
	}
	else if (mObjType >= TYPE_BREEDER && mObjType <= TYPE_BIG_BREEDER)
	{
		theSrcRect = Rect(m0x18c * 80, (mObjType - TYPE_BREEDER) * 240, 80, 80);
		aDrawImage = IMAGE_HUNGRYBREEDER;
	}
	if(aDrawImage)
		g->DrawImageMirror(aDrawImage, 0, anYVal, theSrcRect, flag);
	g->SetColorizeImages(false);
}

void Sexy::DeadFish::Remove()
{
	RemoveDeadFish();
}

void Sexy::DeadFish::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncBool(mFlipped);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncDouble(mSpeedMod);
	theSync->SyncBool(m0x188);
	theSync->SyncLong(m0x18c);
	theSync->SyncLong(m0x190);
	theSync->SyncLong(m0x194);
	theSync->SyncDouble(m0x198);
	theSync->SyncLong(m0x1a0);
	theSync->SyncLong(mObjType);
}

void Sexy::DeadFish::RemoveDeadFish()
{
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();	
}
