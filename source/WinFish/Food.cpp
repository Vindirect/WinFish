#include "Food.h"
#include "Board.h"
#include "Res.h"
#include "WinFishApp.h"
#include "SexyAppFramework/WidgetManager.h"

using namespace Sexy;

Sexy::Food::Food()
{
	mClip = false;
	mType = TYPE_FOOD;
}

Sexy::Food::Food(int theX, int theY, int unk1, bool unk2, int theExoFoodType)
{
	mXD = theX;
	mClip = false;
	mYD = theY;
	mType = TYPE_FOOD;
	mExoticFoodType = theExoFoodType;
	m0x190 = unk2;
	m0x184 = false;
	mFoodType = 0;
	mX = theX;
	mY = theY;
	mVY = -2;
	mWidth = 40;
	mHeight = 40;
	if (unk1 == 2)
		mVX = 3.0;
	else
		mVX = -3.0;

	if (theExoFoodType == 2)
	{
		mMouseVisible = true;
		mDoFinger = true;
	}
	else
		mMouseVisible = false;

	m0x178 = 0;
	m0x180 = 0;
	m0x17c = mApp->mSeed->Next() % 2 + 3;
	mPickedUp = false;
	mCantEatTimer = 20;
}

Sexy::Food::~Food()
{
}

void Sexy::Food::Update()
{
	if (!mApp->mBoard || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (mCantEatTimer != 0)
		mCantEatTimer--;
	m0x178++;
	if (m0x178 > (m0x17c * 10 - 1))
		m0x178 = 0;
	if (mFoodType == 3)
		mMouseVisible = true;

	if (m0x180 != 0)
	{
		m0x180--;
		if(m0x180 != 0)
			return;
		Remove();
		return;
	}

	if (!m0x190 || m0x184)
	{
		if (!mPickedUp)
			mYD += 1.5;
		else
		{
			if (mXD > 550.0)
				mXD = mXD - (mXD - 550.0) / 7.0;
			else if (mXD < 550.0)
				mXD = mXD + (550.0 - mXD) / 7.0;

			if (mYD > 30.0)
				mYD = mYD - (mYD - 30.0) / 7.0;
			else if (mYD < 30.0)
				mYD = mYD + (30.0 - mYD) / 7.0;

			if (mY < 40)
			{
				Remove();
				mApp->mBoard->Unk07(1);
				return;
			}
			Move(mXD, mYD);
		}
	}
	else if (mYD < 120.0)
		m0x184 = true;
	else
	{
		if (mYD < 150.0)
			mYD -= 2.5;
		else if (mYD < 135.0)
			mYD -= 1.5;
		else if (mYD < 130.0)
			mYD -= 1.0;
		else if (mYD < 125.0)
			mYD -= 0.5;
		else if (mYD >= 150.0)
			mYD -= 8.0;
	}

	if (m0x18c != 0)
	{
		if (mVY < 0.0)
		{
			mVY += 0.5;
			mYD += mVY;
		}
		if (m0x18c == 2)
		{
			if (mVX > 0.0)
			{
				mVX -= 0.05;
				mXD += mVX;
			}
		}
		else if (m0x18c == 1 && mVX < 0.0)
		{
			mVX += 0.05;
			mXD += mVX;
		}

		if (mXD > 550.0)
			mXD = 550.0;
		else if (mXD < 20.0)
			mXD = 20.0;
	}

	if (mYD > 410.0 || (mFoodType == 3 && mYD > 400.0))
	{
		if (mFoodType == 3)
		{
			mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
			int aNumOfSmokes = mApp->mSeed->Next() % 3 + 2;
			for (int i = 0; i < aNumOfSmokes; i++)
			{
				int aType = mApp->mSeed->Next() % 3 + 3;
				int aX = mApp->mSeed->Next() % 30 - 10 + mX;
				int aY = mApp->mSeed->Next() % 30 - 10 + mY;
				mApp->mBoard->SpawnShot(aX, aY, aType);
			}
			Remove();
			return;
		}
		if (mExoticFoodType == 2)
		{
			Remove();
			return;
		}
		if (m0x180 < 1)
			m0x180 = 15;
	}
	Move(mXD, mYD);
}

void Sexy::Food::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	if (m0x180 != 0)
	{
		g->SetColor(Color(255, 255, 255, m0x180 * 255 / 15));
		g->SetColorizeImages(true);
	}

	if (mExoticFoodType == EXO_FOOD_SNOT)
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, 200));
		g->DrawImageCel(IMAGE_FOOD, 0, 0, m0x178 / m0x17c, 4);
		g->SetColorizeImages(false);
	}
	else if (mExoticFoodType == EXO_FOOD_PIZZA)
	{
		g->DrawImage(IMAGE_PIZZA, 0, 0);
		g->SetColorizeImages(false);
	}
	else if (mExoticFoodType == EXO_FOOD_ICE_CREAM)
	{
		g->DrawImage(IMAGE_ICECREAM, 0, 0);
		g->SetColorizeImages(false);
	}
	else if (mExoticFoodType == EXO_FOOD_CHICKEN)
	{
		g->DrawImage(IMAGE_CHICKEN, 0, 0);
		g->SetColorizeImages(false);
	}
	else
	{
		int aX = -5;
		int aY = -4;
		if (mFoodType == 3)
		{
			aX = 0;
			aY = -3;
		}
		if (mFoodType == 0 || mFoodType == 1 || mFoodType == 2 || mFoodType == 3)
			g->DrawImageCel(IMAGE_FOOD, aX, aY, m0x178 / m0x17c, mFoodType);
		g->SetColorizeImages(false);
	}
}

void Sexy::Food::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount > -1 && mExoticFoodType == 2)
	{
		PickUp();
		return;
	}
	mApp->mBoard->CheckMouseDown(mX + x, mY + y);
}

void Sexy::Food::Remove()
{
	mApp->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
}

void Sexy::Food::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncLong(m0x178);
	theSync->SyncLong(m0x17c);
	theSync->SyncLong(m0x180);
	theSync->SyncBool(m0x184);
	theSync->SyncBool(mPickedUp);
	theSync->SyncLong(mFoodType);
	theSync->SyncLong(m0x18c);
	theSync->SyncBool(m0x190);
	theSync->SyncLong(mExoticFoodType);
	theSync->SyncLong(mCantEatTimer);
}

void Sexy::Food::PickUp()
{
	if (!mPickedUp)
	{
		ShowFinger(false);
		mDoFinger = false;
		mMouseVisible = false;
		mPickedUp = true;
		mApp->mBoard->PlayPointsSound();
	}
}
