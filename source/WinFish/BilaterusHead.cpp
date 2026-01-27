#include "BilaterusHead.h"
#include "BilaterusBone.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

using namespace Sexy;
Sexy::BilaterusHead::BilaterusHead(Bilaterus* theBilaterus,int theX, int theY, bool active) 
	: GameObject()
{
	mXD = theX;
	mClip = false;
	mYD = theY;
	mBilaterus = theBilaterus;
	mHeadActive = active;
	mX = theX;
	mY = theY;
	mVX = 0;
	mVY = -0.5;
	mWidth = 80;
	mHeight = 80;
	m0x1a0 = 1.0;
	if (mApp->mSeed->Next() % 2 == 0)
	{
		mVX = -0.1;
		m0x1a0 = -1.0;
	}
	m0x188 = 0;
	m0x1a8 = 370;
	m0x190 = 0;
	m0x1b0 = 95;
	m0x1ac = 10;
	m0x198 = 0.8;
	m0x1b4 = 540;
	m0x1d8 = 100.0;
	m0x1e0 = 0;
	m0x1e8 = 0;
	m0x1f0 = 0;
	m0x1f8 = 0;
	m0x1bc = 40;
	m0x1c0 = 0;
	m0x1c4 = 0;
	m0x1c8 = 0;
	m0x1cc = 0;
	mMouseVisible = false;
	m0x200 = 12;
	m0x1d4 = 100;
	m0x1d0 = 0;
	m0x15c = false;
	m0x15d = false;
	m0x1b8 = mApp->mSeed->Next() % 10;
	m0x160 = !mHeadActive;
}

Sexy::BilaterusHead::~BilaterusHead()
{
}

void Sexy::BilaterusHead::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	bool mirror = false;
	if (mVX >= 0.0)
	{
		if ((int)mVX != 0)
			mirror = true;
		else if (m0x1a0 < 0.0)
			mirror = true;
		else if (mVX > 0.0)
			mirror = true;
	}
	DrawHead(g, mirror);

	if (mHeadActive)
	{
		if (mApp->mBoard->mPetsInTank[PET_BLIP] != 0 && mApp->mBoard->mTank != 5)
		{
			Image* aBarImg = IMAGE_HEALTHBAR;
			int aBarWidth = (int)((double)aBarImg->mWidth * m0x1d8 / 100.0);
			int aY = mHeight - 3;
			if (aBarWidth < 0) aBarWidth = 0;
			if (aBarWidth > aBarImg->mWidth)aBarWidth = aBarImg->mWidth;
			if (m0x160 == 4)
				aBarWidth = aBarImg->mWidth - aBarWidth;
			Rect aSrcRect(0, 0, aBarWidth, aBarImg->mHeight);
			g->DrawImage(aBarImg, -20, aY, aSrcRect);
			g->DrawImage(IMAGE_HEALTHBARTUBE, -20, aY);
		}
	}
}

void Sexy::BilaterusHead::Remove()
{
	Die(true);
}

void Sexy::BilaterusHead::OnFoodAte(GameObject* obj)
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

void Sexy::BilaterusHead::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncBool(m0x15c);
	theSync->SyncBool(m0x15d);
	theSync->SyncBool(mHeadActive);
	theSync->SyncLong(m0x160);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncDouble(m0x188);
	theSync->SyncDouble(m0x190);
	theSync->SyncDouble(m0x198);
	theSync->SyncDouble(m0x1a0);
	theSync->SyncLong(m0x1a8);
	theSync->SyncLong(m0x1ac);
	theSync->SyncLong(m0x1b0);
	theSync->SyncLong(m0x1b4);
	theSync->SyncLong(m0x1b8);
	theSync->SyncLong(m0x1bc);
	theSync->SyncLong(m0x1c0);
	theSync->SyncLong(m0x1c4);
	theSync->SyncLong(m0x1c8);
	theSync->SyncLong(m0x1cc);
	theSync->SyncLong(m0x1d0);
	theSync->SyncLong(m0x1d4);
	theSync->SyncDouble(m0x1d8);
	theSync->SyncDouble(m0x1e0);
	theSync->SyncDouble(m0x1e8);
	theSync->SyncDouble(m0x1f0);
	theSync->SyncDouble(m0x1f8);
	theSync->SyncLong(m0x200);
}

void Sexy::BilaterusHead::UpdateHead()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	if (!mHeadActive) // Passive head follow
	{
		if (mBilaterus == nullptr)
			return;

		m0x200++;
		BilaterusBone* aBone = mBilaterus->mBoneList->at(5);
		if (m0x200 >= 12)
		{
			m0x200 = 0;
			m0x1f0 = aBone->m0x170;
			m0x1f8 = aBone->m0x178;
		}

		m0x1e0 = aBone->mXD;
		m0x1e8 = aBone->mYD;

		if (mVX < m0x1f0)
			mVX += 0.02;
		if (mVX > m0x1f0)
			mVX -= 0.02;
		if (mVY < m0x1f8)
			mVY += 0.02;
		if (mVY > m0x1f8)
			mVY -= 0.02;

		if (mXD < m0x1e0 - 40.0)
			mXD = m0x1e0 - 40.0;
		if (mXD > m0x1e0 + 40.0)
			mXD = m0x1e0 + 40.0;
		if (mYD > m0x1e8 + 20.0)
			mYD = m0x1e8 + 20.0;
		if (mYD < m0x1e8 - 20.0)
			mYD = m0x1e8 - 20.0;
	}
	else // Active head update
	{
		if (!ShouldChaseTarget())
		{
			if (m0x1b8 == 0)
				m0x188 = -1.5;
			else if (m0x1b8 == 1)
				m0x188 = 1.5;
			else if (m0x1b8 == 2)
				m0x190 = -1.5;
			else if (m0x1b8 == 3)
				m0x190 = 1.5;

			if (mVX > m0x188)
				mVX -= 0.1;
			if (mVX < m0x188)
				mVX += 0.1;
			if (mVY > m0x190)
				mVY -= 0.1;
			if (mVY < m0x190)
				mVY += 0.1;

			m0x1bc++;
			m0x1c0++;
			if (m0x1c0 > 20)
			{
				m0x1c0 = 0;
				if (mApp->mSeed->Next() % 5 == 0)
					m0x1b8 = mApp->mSeed->Next() % 4;
			}
		}
	}

	UpdateAnimation();
	if (mXD < m0x1ac)
	{
		mXD = m0x1ac;
		m0x188 = 1.5;
		mVX = 0.0;
	}
	if (mXD > m0x1b4)
	{
		mXD = m0x1b4;
		m0x188 = -1.5;
		mVX = 0.0;
	}
	if (mYD > m0x1a8)
	{
		mYD = m0x1a8;
		mVY = 0.0;
	}
	if (mYD < m0x1b0)
	{
		mYD = m0x1b0;
		mVY = 0.0;
	}

	mXD += mVX / m0x198;
	mYD += mVY / m0x198;
	if (m0x1d0 > 0)
		m0x1d0--;
	if (m0x1d4 > 0)
		m0x1d4--;

	Move(mXD, mYD);

	if (mHeadActive && mBilaterus)
		mBilaterus->Move(mXD, mYD);
	if (m0x15d)
		m0x15d = false;
}

void Sexy::BilaterusHead::DrawHead(Graphics* g, bool mirror)
{
	int ivar3 = 3;
	int ivar1 = 4;
	int ivar4 = 5;
	if (m0x160 == 1)
	{
		ivar3 = 0;
		ivar1 = 1;
		ivar4 = 2;
	}
	Rect aSrcRect(0,0,0,0);
	if (!mHeadActive)
	{
		if (mBilaterus == nullptr)
			return;

		double aDeltaX = mXD - mBilaterus->mBoneList->at(0)->mXD;
		if (aDeltaX < 36.0 && aDeltaX > -30.0)
		{
			aSrcRect = Rect((5 - (int)(aDeltaX / 6.0)) * 80, ivar3 * 80, 80, 80);
			DrawHeadHelper(g, IMAGE_BILATERUS, aSrcRect, false);
			if (aDeltaX >= 0.0)
				m0x15c = false;
			else
				m0x15c = true;
			return;
		}
		aSrcRect = Rect(320, ivar4 * 80, 80, 80);
		mirror = !m0x15c;
	}
	else if (m0x1cc == 0)
	{
		aSrcRect = Rect(m0x1c8 * 80, ivar4 * 80, 80, 80);
	}
	else
	{
		aSrcRect = Rect(m0x1c8 * 80, ivar1 * 80, 80, 80);
		mirror = m0x1cc > 0;
	}
	DrawHeadHelper(g, IMAGE_BILATERUS, aSrcRect, mirror);
}

void Sexy::BilaterusHead::DrawHeadHelper(Graphics* g, Image* theImage, Rect& theSrcRect, bool mirror)
{
	if (mBilaterus == nullptr || mBilaterus->m0x16c == 0)
	{
		g->DrawImageMirror(theImage, 0, 0, theSrcRect, mirror);
		if (m0x1d0 > 0)
		{
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			g->SetColorizeImages(true);
			g->SetColor(Color(255, 255, 255, m0x1d0 * 25));
			g->DrawImageMirror(theImage, 0, 0, theSrcRect, mirror);
			g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
			g->SetColorizeImages(false);
		}
	}
	else if(mBilaterus->m0x16c < 7)
	{
		float aRatio = mBilaterus->m0x16c / 7.0;

		if (aRatio <= 0.0)
			return;

		if (mHeadActive)
		{
			aRatio = (aRatio * 0.8f);
			aRatio *= aRatio;
		}

		int aWidth = theSrcRect.mWidth;
		int aHeight = theSrcRect.mHeight;

		int aInsetX = (int)((float)aWidth * aRatio);
		int aInsetY = (int)((float)aHeight * aRatio);

		Rect aDestRect(aInsetX/2, aInsetY/2, theSrcRect.mWidth - aInsetX, theSrcRect.mHeight - aInsetY);
		g->SetFastStretch(!mApp->Is3DAccelerated());
		g->DrawImageMirror(IMAGE_BILATERUS, aDestRect, theSrcRect, mirror);
	}
}

void Sexy::BilaterusHead::UpdateAnimation()
{
	if (m0x1a0 < 0.0 && mVX > 0.0 && m0x1cc == 0 && !m0x15d)
		m0x1cc = -20;
	else if (m0x1a0 > 0.0 && mVX < 0.0 && m0x1cc == 0 && !m0x15d)
		m0x1cc = 20;

	if (m0x1cc > 0)
		m0x1cc--;
	else if (m0x1cc < 0)
		m0x1cc++;

	if (m0x1cc == 0)
	{
		m0x1c4++;
		if (m0x1c4 > 19)
			m0x1c4 = 0;
		if (m0x160 == 0)
			m0x1c8 = m0x1c4 / 2;
		else
		{
			m0x1c8 = m0x1c4 - 10;
			if (m0x1c8 < 0)
				m0x1c8 = m0x1c4 - 9;
			m0x1c8 = abs(m0x1c8);
		}
	}
	else
	{
		if (m0x1cc > 0)
			m0x1c8 = 9 - m0x1cc / 2;
		else if (m0x1cc < 0)
			m0x1c8 = m0x1cc / 2 + 9;
	}

	if (mVX != m0x1a0 && mVX != 0.0 && m0x1a0 != 0.0)
		m0x1a0 = mVX;
}

bool Sexy::BilaterusHead::TakeHit(int theX, int theY)
{
	if (theX > mXD && theX < mXD + 80.0 && theY > mYD && theY < mYD + 80.0 && m0x1d0 < 1)
	{
		if (theX < mXD + 30.0 && theY < mYD + 30.0)
		{
			double aSpeedMod = m0x198 * 3.5;
			mVX = aSpeedMod;
			mVY = aSpeedMod;
		}
		else if (theX < mXD + 30.0 && theY < mYD + 50.0)
			mVX = m0x198 * 4.0;
		else if (theX < mXD + 30.0)
		{
			mVX = m0x198 * 3.5;
			mVY = m0x198 * -3.5;
		}
		else if (theX < mXD + 50.0 && theY < mYD + 30.0)
			mVY = m0x198 * 4.0;
		else if (theX > mXD + 50.0 && theY > mYD + 50.0)
		{
			mVX = m0x198 * -3.5;
			mVY = m0x198 * -3.5;
		}
		else if (theX > mXD + 50.0 && theY < mYD + 30.0)
		{
			mVX = m0x198 * -3.5;
			mVY = m0x198 * 3.5;
		}
		else if (theX > mXD + 50.0)
			mVX = m0x198 * -4.0;
		else if (theY > mYD + 50.0)
			mVY = m0x198 * -4.0;

		m0x1d8 -= mApp->mBoard->m0x3e4 * 2 + 2;
		mApp->mBoard->PlaySample(SOUND_HIT_ID, 3, 1.0);
		m0x1d0 = 10;
		if (m0x1d8 <= 0.0)
			Die(true);
		return true;
	}
	return false;
}

bool Sexy::BilaterusHead::ShouldChaseTarget()
{
	if (mApp->CanAlienChaseAnyFish())
	{
		if (mApp->mBoard->HasAnyFish() || mApp->mBoard->mCyraxPtr != nullptr)
		{
			if (FindNearestTarget())
			{
				ChaseTarget();
				return true;
			}
		}
	}
	return false;
}

Sexy::GameObject* Sexy::BilaterusHead::FindNearestTarget()
{
	if (!mApp->CanAlienChaseAnyFish())
		return nullptr;
	int aDist = 100000000;
	int aCenterX = mWidth / 2 + mX;
	int aCenterY = mHeight / 2 + mY;
	GameObject* f = nullptr;
	Board* aBoard = mApp->mBoard;
	for (GameObjectSet::iterator it = aBoard->mGameObjectSet.begin(); it != aBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mVirtualTankId > -1 || anObj->mCanBeEatenDelay > 0) continue;
		bool chase = false;

		switch (anObj->mType)
		{
		case GameObject::TYPE_GUPPY:
			chase = !((Fish*)anObj)->WadsworthActive();
			break;
		case GameObject::TYPE_OSCAR:
		case GameObject::TYPE_ULTRA:
		case GameObject::TYPE_GEKKO:
		case GameObject::TYPE_PENTA:
		case GameObject::TYPE_GRUBBER:
		case GameObject::TYPE_BREEDER:
			chase = true;
			break;
		case GameObject::TYPE_OTHER_TYPE_PET:
		case GameObject::TYPE_FISH_TYPE_PET:
			if (aBoard->mCyraxPtr) chase = true;
			break;
		default: 
			continue;
		}
		if (chase)
		{
			int ax = (aCenterX - anObj->mWidth / 2) - anObj->mX;
			int ay = (aCenterY - anObj->mHeight / 2) - anObj->mY;
			int aNewDist = ax * ax + ay * ay;
			if (aNewDist < aDist)
			{
				f = anObj;
				aDist = aNewDist;
			}
		}
	}
	return f;
}

void Sexy::BilaterusHead::ChaseTarget()
{
	GameObject* aTarget = FindNearestTarget();
	if (aTarget)
	{
		if (mXD + 40.0 < aTarget->mX + 40 && mVX < 1.8)
			mVX += 0.1;
		else if (mXD + 40.0 > aTarget->mX + 40 && mVX > -1.8)
			mVX -= 0.1;

		if (mYD + 40.0 < aTarget->mY + 40 && mVY < 1.8)
			mVY += 0.1;
		else if (mYD + 40.0 > aTarget->mY + 40 && mVY > -1.8)
			mVY -= 0.1;
	}

	if (m0x1d4 < 1 && aTarget)
		CheckCollision();
}

void Sexy::BilaterusHead::Die(bool flag)
{
	if (!mBilaterus->m0x164)
	{
		int aMissleType = 5;
		if (m0x160 == 0)
			aMissleType = 4;

		mApp->mBoard->SpawnMissle(mX, mY, nullptr, aMissleType);
		mBilaterus->ChangeHead();
		mWidgetManager->RemoveWidget(this);
		mApp->SafeDeleteWidget(this);
		mBilaterus->m0x164 = true;
		mBilaterus->mPassiveHead = nullptr;
		if (mShadowPtr)
			mShadowPtr->Remove();
		mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
		mApp->mBoard->PlaySample(SOUND_EXPLOSION1_ID, 3, 1.0);
	}
	else
		mBilaterus->Remove(flag);
}

void Sexy::BilaterusHead::BHUnk01()
{
	if (!m0x15c)
		mVX = 1.5;
	else
		mVX = -1.5;

	m0x1f0 = mBilaterus->mBoneList->at(5)->m0x170;
	m0x1f8 = mBilaterus->mBoneList->at(5)->m0x178;
	m0x1e0 = mBilaterus->mBoneList->at(5)->mXD;
	m0x1e8 = mBilaterus->mBoneList->at(5)->mYD;

	if (mXD - m0x1e0 < 0.0)
		m0x15c = true;
	else
		m0x15c = false;
	m0x15d = true;
}

void Sexy::BilaterusHead::CheckCollision()
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
