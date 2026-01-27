#include "BilaterusBone.h"
#include "Bilaterus.h"
#include "BilaterusHead.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

Sexy::BilaterusBone::BilaterusBone(Bilaterus* theBilaterus, int theX, int theY, int theBoneId)
{
	mXD = theX;
	mBilaterus = theBilaterus;
	mYD = theY;
	mX = theX;
	mY = theY;
	mBoneId = theBoneId;
	m0x170 = 0;
	m0x178 = -0.5;
	mWidth = 80;
	mHeight = 80;
	m0x198 = 1.0;
	if (mApp->mSeed->Next() % 2 == 0)
	{
		m0x170 = -0.1;
		m0x198 = -1.0;
	}
	m0x1a0 = 370;
	m0x180 = 0;
	m0x1a8 = 95;
	m0x188 = 0;
	m0x1a4 = 10;
	m0x1ac = 540;
	m0x190 = 0.8;
	m0x1c0 = 100;
	m0x1b0 = 0;
	m0x1b4 = 0;
	m0x1c8 = 0;
	m0x1b8 = 0;
	m0x1d0 = 0;
	mMouseVisible = false;
	m0x1d8 = 0;
	m0x1bc = 0;
	m0x1e0 = 0;
	m0x1e8 = 12;
}

Sexy::BilaterusBone::~BilaterusBone()
{
}

void Sexy::BilaterusBone::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	bool mirror = false;
	if (m0x170 >= 0.0)
	{
		if ((int)m0x170 != 0)
			mirror = true;
		else if (m0x198 > 0.0)
			mirror = true;
		else if (m0x170 > 0.0 && m0x198 >= 0.0)
			mirror = true;
	}
		
	DrawBone(g, mirror);
}

void Sexy::BilaterusBone::OnFoodAte(GameObject* obj)
{
	gUnkInt03 = mApp->mUpdateCount;
	mApp->mBoard->PlayChompSound(false);
	if (mApp->m0x882)
	{
		for (int i = mApp->mSeed->Next() % 3 + 2; i > 0; i--)
		{
			if (obj->mType == TYPE_ULTRA)
			{
				mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 20 + 55 + obj->mX, mApp->mSeed->Next() % 20 + 55 + obj->mY, 1);
			}
			else
				mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 20 + 15 + obj->mX, mApp->mSeed->Next() % 20 + 15 + obj->mY, 1);
		}
	}
}

void Sexy::BilaterusBone::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncLong(mBoneId);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(m0x170);
	theSync->SyncDouble(m0x178);
	theSync->SyncDouble(m0x180);
	theSync->SyncDouble(m0x188);
	theSync->SyncDouble(m0x190);
	theSync->SyncDouble(m0x198);
	theSync->SyncLong(m0x1a0);
	theSync->SyncLong(m0x1a4);
	theSync->SyncLong(m0x1a8);
	theSync->SyncLong(m0x1ac);
	theSync->SyncLong(m0x1b0);
	theSync->SyncLong(m0x1b4);
	theSync->SyncLong(m0x1b8);
	theSync->SyncLong(m0x1bc);
	theSync->SyncLong(m0x1c0);
	theSync->SyncDouble(m0x1c8);
	theSync->SyncDouble(m0x1d0);
	theSync->SyncDouble(m0x1d8);
	theSync->SyncDouble(m0x1e0);
	theSync->SyncLong(m0x1e8);
}

void Sexy::BilaterusBone::UpdateBone()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	if (mBilaterus == nullptr || mBilaterus->mActiveHead == nullptr)
		return;

	m0x1e8++;
	if (m0x1e8 >= 12)
	{
		m0x1e8 = 0;
		if (mBoneId == 0)
		{
			m0x1d8 = mBilaterus->mActiveHead->mVX;
			m0x1e0 = mBilaterus->mActiveHead->mVY;
		}
		else
		{
			BilaterusBone* aPrevBone = mBilaterus->mBoneList->at(mBoneId - 1);
			m0x1d8 = aPrevBone->m0x170;
			m0x1e0 = aPrevBone->m0x178;
		}
	}

	if (mBoneId == 0)
	{
		m0x1c8 = mBilaterus->mActiveHead->mXD;
		m0x1d0 = mBilaterus->mActiveHead->mYD;
	}
	else
	{
		BilaterusBone* aPrevBone = mBilaterus->mBoneList->at(mBoneId - 1);
		m0x1c8 = aPrevBone->mXD;
		m0x1d0 = aPrevBone->mYD;
	}

	if (m0x170 < m0x1d8)
		m0x170 += 0.02;
	if (m0x170 > m0x1d8)
		m0x170 -= 0.02;
	if (m0x178 < m0x1e0)
		m0x178 += 0.02;
	if (m0x178 > m0x1e0)
		m0x178 -= 0.02;

	if (m0x170 != m0x198 && m0x170 != 0.0 && m0x198 != 0.0)
		m0x198 = m0x170;
	double aMaxXDist = 30.0;
	if (mBoneId == 0)
		aMaxXDist = 40.0;

	if (mXD < m0x1c8 - aMaxXDist)
		mXD = m0x1c8 - aMaxXDist;
	if (mXD > m0x1c8 + aMaxXDist)
		mXD = m0x1c8 + aMaxXDist;
	if (mYD > m0x1d0 + 20.0)
		mYD = m0x1d0 + 20.0;
	if (mYD < m0x1d0 - 20.0)
		mYD = m0x1d0 - 20.0;

	if (mXD < m0x1a4)
		mXD = m0x1a4;
	if (mXD > m0x1ac)
		mXD = m0x1ac;
	if (mYD > m0x1a0)
		mYD = m0x1a0;
	if (mYD < m0x1a8)
		mYD = m0x1a8;

	mXD += m0x170 / m0x190;
	mYD += m0x178 / m0x190;

	if (m0x1bc > 0)
		m0x1bc--;
	if (m0x1c0 > 0)
		m0x1c0--;

	Move(mXD, mYD);
	if (m0x1c0 < 1)
		CheckCollision();
}

void Sexy::BilaterusBone::UpdateFollowPosAndVel()
{
	Bilaterus* aBil = mBilaterus;
	if (mBoneId == 0)
	{
		m0x1d8 = aBil->mActiveHead->mVX;
		m0x1e0 = aBil->mActiveHead->mVY;
		m0x1c8 = aBil->mActiveHead->mXD;
		m0x1d0 = aBil->mActiveHead->mYD;
	}
	else
	{
		int aPrevId = mBoneId - 1;
		m0x1d8 = aBil->mBoneList->at(aPrevId)->m0x170;
		m0x1e0 = aBil->mBoneList->at(aPrevId)->m0x178;
		m0x1c8 = aBil->mBoneList->at(aPrevId)->mXD;
		m0x1d0 = aBil->mBoneList->at(aPrevId)->mYD;
	}
}

void Sexy::BilaterusBone::CheckCollision()
{
	if (!mApp->CanAlienChaseAnyFish())
		return;

	int aCX = mX + mWidth / 2;
	int aCY = mY + mHeight / 2;

	for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mVirtualTankId > -1 || anObj->mCanBeEatenDelay > 0) continue;

		int aXEatDistObj = 30;
		int aYEatDistObj = 30;
		bool canBeEaten = false;
		switch (anObj->mType)
		{
		case TYPE_GUPPY:
			canBeEaten = !((Fish*)anObj)->WadsworthActive();
			break;
		case TYPE_ULTRA:
			aXEatDistObj = 70;
			aYEatDistObj = 70;
		case TYPE_OSCAR:
		case TYPE_GEKKO:
		case TYPE_PENTA:
		case TYPE_GRUBBER:
		case TYPE_BREEDER:
			canBeEaten = true;
			break;
		case TYPE_OTHER_TYPE_PET:
		case TYPE_FISH_TYPE_PET:
			canBeEaten = (mApp->mBoard->mCyraxPtr != nullptr);
			break;
		default:
			continue;
		}

		if (canBeEaten)
		{
			int ax = aCX - (anObj->mWidth / 2 + anObj->mX);
			int ay = aCY - (anObj->mHeight / 2 + anObj->mY);
			if (ax > -aXEatDistObj && ax < aXEatDistObj && ay > -aYEatDistObj && ay < aYEatDistObj)
			{
				OnFoodAte(anObj);
				anObj->Remove();
				return;
			}
		}
	}
}

void Sexy::BilaterusBone::DrawBone(Graphics* g, bool mirror)
{
	if (mBilaterus->m0x16c != 0)
		return;

	int isEven = mBoneId % 2;
	double aDeltaX = 0;
	if (mBoneId == 0)
		aDeltaX = mXD - mBilaterus->mActiveHead->mXD;
	else
	{
		BilaterusBone* aPrevBone = mBilaterus->mBoneList->at(mBoneId-1);
		aDeltaX = mXD - aPrevBone->mXD;
	}

	int aFrame = 0;
	if (aDeltaX < 36.0 && aDeltaX > -30.0)
	{
		int aAngleVal = (int)(aDeltaX / 6.0);
		aFrame = 5 - aAngleVal;
	}

	int aSrcXOffset = aFrame * -80;

	int aSrcYOffset = (-6 - isEven) * 80;

	DrawBoneHelper(g, IMAGE_BILATERUS, aSrcXOffset, aSrcYOffset, mirror);
}

void Sexy::BilaterusBone::DrawBoneHelper(Graphics* g, Image* theImage, int theSrcX, int theSrcY, bool mirror)
{
	g->DrawImage(theImage, theSrcX, theSrcY);
	if (m0x1bc > 0)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, m0x1bc * 25));
		g->DrawImage(theImage, theSrcX, theSrcY);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		g->SetColorizeImages(false);
	}
}
