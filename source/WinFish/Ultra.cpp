#include "Ultra.h"
#include "WinFishApp.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Board.h"
#include "Res.h"

Sexy::Ultra::Ultra()
{
	mType = TYPE_ULTRA;
}

Sexy::Ultra::Ultra(int theX, int theY) : Fish(theX, theY)
{
	Ultra::Init();
}

Sexy::Ultra::Ultra(int theX, int theY, bool velocityRight) : Fish(theX, theY)
{
	Ultra::Init();
	if (velocityRight)
	{
		mVX = 1.0;
		mPrevVX = 1.0;
	}
	else
	{
		mVX = -1.0;
		mPrevVX = -1.0;
	}
}

Sexy::Ultra::~Ultra()
{
}

void Sexy::Ultra::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[3]++;
}

void Sexy::Ultra::OnFoodAte(GameObject* obj)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	if (mVirtualTankId > -1)
	{
		UpdateMentalState();
		if (mPreNamedTypeId == KILGORE && mSongId != -1)
			mApp->mBoard->StopFishSong(mSongId);
	}
	mApp->mBoard->PlayChompSound(mVoracious);
	mHunger += 900;
	if (mHunger > 1300)
		mHunger = 1300;
	if (mApp->m0x882 && obj->mType != TYPE_FOOD)
	{
		for (int i = mApp->mSeed->Next() % 3 + 2; i > 0; i--)
		{
			mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 20 + 15 + obj->mX, mApp->mSeed->Next() % 20 + 15 + obj->mY, 1);
		}
	}
	UpdateHungerStateIfWasHungry(hungry);
}

void Sexy::Ultra::DrawStoreAnimation(Graphics* g, int justification)
{
	int anX = 0, anY = 0;
	switch (justification)
	{
	case 0:
		anX = 13;
		anY = -22;
		break;
	case 1:
		anX = 3;
		anY = -22;
		break;
	case 2:
		anX = 3;
		anY = -42;
		break;
	case 3:
		anX = 10;
		anY = -40;
		break;
	case 4:
		anX = -70;
		anY = -45;
		break;
	}
	g->Translate(anX, anY);
	Color aCol = Color::White;
	if (mPreNamedTypeId == KILGORE)
		aCol = Color(0xffaaff);
	SetStoreColor(g, aCol);
	g->DrawImageCel(IMAGE_ULTRA, 0, 0, mStoreAnimationIndex, 0);
	g->SetColorizeImages(false);
	g->Translate(-anX, -anY);
}

void Sexy::Ultra::DropCoin()
{
	mCoinDropTimer++;
	if (CoinDropTimerPassed(mCoinDropTimer, mCoinDropT))
	{
		mCoinDropTimer = 0;
		if (RelaxModeCanDrop())
			mApp->mBoard->DropCoin(mX + 40, mY + 90, 7, nullptr, -1.0, 0);
	}
}

bool Sexy::Ultra::Hungry()
{
	UpdateHungerAnimCounter();
	if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
		UpdateHungerCounter();
	if (ShouldDie())
		Die(true);
	else
	{
		if (mHunger < 500)
		{
			if (mPreNamedTypeId == KILGORE)
			{
				if (mHunger == 290 && m0x118 % 5 == 0 && mSongId == -1)
				{
					m0x114 = 5;
					return HungryBehavior();
				}
				if (mHunger >= 290 || ((m0x114 != 0 || mSongId != -1) && mApp->mBoard->mFishSongMgr->Unk01(mSongId) < 35001))
					return false;
			}
			return HungryBehavior();
		}
	}
	return false;
}

void Sexy::Ultra::DrawFish(Graphics* g, bool mirror)
{
	if (gZombieMode)
	{
		bool aHungr = IsHungryVisible();
		if (aHungr)
		{
			g->SetColorizeImages(true);
			g->SetColor(Color(0xfad75f));
		}
		g->DrawImageMirror(IMAGE_ULTRA, 0, 0, Rect(1440, 480, 160, 160), mirror);
		g->SetColorizeImages(false);
		if (mMisslePtr)
			DrawCrosshair(g, 40, 40);
		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 40, 0, 2);
		return;
	}

	Rect aSrcRect(mAnimationFrameIndexFish * 160, 0, 160, 160);
	if(mTurnAnimationTimer == 0)
		if (mEatingAnimationTimer > 0)
		{
			aSrcRect.mY = 160;
		}
		else
		{
			aSrcRect.mY = 0;
			if(mVoraciousScreamCounter > 100)
				aSrcRect.mY = 160;
		}
	if (mTurnAnimationTimer != 0)
		aSrcRect.mY = 320;

	if (mInvisible)
		if (DrawInvisibleEffect(g, IMAGE_ULTRA, aSrcRect, mirror))
			return;

	bool hungry = IsHungryVisible();
	if (hungry)
	{
		SetColorHelper(g, Color(0xfa, 0xd7, 0x5f));
		g->DrawImageMirror(IMAGE_ULTRA, 0, 0, aSrcRect, mirror);
	}
	else
	{
		if (mPreNamedTypeId == KILGORE)
			SetColorHelper(g, Color(0xffaaff));
		else
			SetColorHelper(g, Color::White);
		g->DrawImageMirror(IMAGE_ULTRA, 0, 0, aSrcRect, mirror);
		if (mHungerAnimationTimer != 0)
		{
			SetColorHelper(g, Color(0xfa, 0xd7, 0x5f, mHungerAnimationTimer * 255 / 5));
			g->DrawImageMirror(IMAGE_ULTRA, 0, 0, aSrcRect, mirror);
		}
	}
	g->SetColorizeImages(false);
	if (mMisslePtr)
		DrawCrosshair(g, 40, 40);
	if (IsHungryBlipPointer(500))
		g->DrawImageCel(IMAGE_MISCITEMS, 40, 0, 2);
}

bool Sexy::Ultra::HungryBehavior()
{
	GameObject* aFood = FindNearestFood();
	int aX = mXD + 80.0;
	int aY = mYD + 100.0;
	if (mExoticDietFoodType != 0)
		aY += 20;

	if (mSpecialMovementStateChangeTimer > 2)
	{
		if (aFood == nullptr)
			return false;
		mSpecialMovementStateChangeTimer = 0;
		int aFX = aFood->mX;
		int aFY = aFood->mY;
		if (mHunger <= 300)
		{
			if (aFX + 44 < aX)
			{
				if (mVX > -5.0)
					mVX -= 1.5;
			}
			else if (aFX + 36 > aX)
			{
				if (mVX < 5.0)
					mVX += 1.5;
			}
			else if (aFX + 42 < aX)
			{
				if (mVX > -5.0)
					mVX -= 0.2;
			}
			else if (aFX + 38 > aX)
			{
				if (mVX < 5.0)
					mVX += 0.2;
			}
			else if (aFX + 40 < aX)
			{
				if (mVX > -5.0)
					mVX -= 0.05;
			}
			else if (aFX + 40 > aX)
			{
				if (mVX < 5.0)
					mVX += 0.05;
			}

			if (aFY + 40 < aY)
			{
				if (mVY > -3.0)
					mVY -= 1.3;
			}
			else if (aFY + 40 > aY)
			{
				if (mVY < 4.0)
					mVY += 1.3;
			}
		}
		else
		{
			if (aFX + 44 < aX)
			{
				if (mVX > -4.0)
					mVX -= 1.5;
			}
			else if (aFX + 36 > aX)
			{
				if (mVX < 4.0)
					mVX += 1.5;
			}
			else if (aFX + 42 < aX)
			{
				if (mVX > -4.0)
					mVX -= 0.2;
			}
			else if (aFX + 38 > aX)
			{
				if (mVX < 4.0)
					mVX += 0.2;
			}
			else if (aFX + 40 < aX)
			{
				if (mVX > -4.0)
					mVX -= 0.1;
			}
			else if (aFX + 40 > aX)
			{
				if (mVX < 4.0)
					mVX += 0.1;
			}

			if (aFY + 43 < aY)
			{
				if (mVY > -3.0)
					mVY -= 0.8;
			}
			else if (aFY + 37 > aY)
			{
				if (mVY < 4.0)
					mVY += 1.3;
			}
			else if (aFY + 40 < aY)
			{
				if (mVY > -3.0)
					mVY -= 0.3;
			}
			else if (aFY + 40 > aY)
			{
				if (mVY < 4.0)
					mVY += 0.5;
			}
		}
		if (mVXAbs < 5)
			mVXAbs++;
	}
	if (aFood != nullptr)
		CollideWithFood();
	return aFood != nullptr;
}

Sexy::GameObject* Sexy::Ultra::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(mX + mWidth / 2, mY + mHeight / 2 + 20);

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	Oscar* aRet = nullptr;
	for (int i = 0; i < aBoard->mOscarList->size(); i++)
	{
		Oscar* aFish = aBoard->mOscarList->at(i);

		if (aFish->mVirtualTankId < 0 && aFish->mCanBeEatenDelay < 1)
		{
			int ax = (mXD + 80) - (aFish->mX + 40);
			int ay = (mYD + 80) - (aFish->mY + 40);
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = aFish;
				aDist = aNewDist;
			}
		}
	}
	if (aDist < 40000)
	{
		VoraciousScream(175);
	}
	if (aDist < 10000)
	{
		mSpeedySpeedState = 100;
	}
	return aRet;
}

void Sexy::Ultra::CollideWithFood()
{
	Board* aBoard = mApp->mBoard;
	if (mExoticDietFoodType == 0)
	{
		for (int i = 0; i < aBoard->mOscarList->size(); i++)
		{
			Oscar* aFish = aBoard->mOscarList->at(i);

			int aFX = aFish->mX;
			int aFY = aFish->mY;
			if (aFish->mVirtualTankId < 0 && aFish->mCanBeEatenDelay < 1)
			{
				if (mXD + 80.0 < aFX + 120 && mXD + 80.0 > aFX - 40 &&
					mYD + 90.0 < aFY + 80 && mYD + 90.0 > aFY)
				{
					OnFoodAte(aFish);
					aFish->RemoveFromGame(true);
					if (mEatingAnimationTimer == 0)
					{
						ShowInvisibility();
						mEatingAnimationTimer = 8;
					}
					return;
				}
				else if (mEatingAnimationTimer == 0 && mXD + 80.0 < aFX + 150 && mXD + 80.0 > aFX - 70
					&& mYD + 90.0 < aFY + 100 && mYD + 90.0 > aFY - 20)
				{
					mEatingAnimationTimer = 20;
					ShowInvisibility();
				}
			}
		}
	}
	else
	{
		int aVal = ExoticFoodCollision(mX + mWidth / 2, mY + mHeight / 2 + 20);
		if (mEatingAnimationTimer == 0)
		{
			if (aVal == 1)
			{
				mEatingAnimationTimer = 8;
				ShowInvisibility();
			}
			else if (aVal == 2)
			{
				mEatingAnimationTimer = 20;
				ShowInvisibility();
			}
		}
	}
}

void Sexy::Ultra::RemoveFromGame(bool aRemoveShadow)
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (aRemoveShadow && mShadowPtr)
		mShadowPtr->Remove();
	mApp->mBoard->m0x474++;
}

void Sexy::Ultra::FishUpdateAnimation()
{
	if (mInvisible)
		UpdateInvisible();

	if (mPrevVX < 0.0 && mVX > 0.0)
		mTurnAnimationTimer = -20;
	else if (mPrevVX > 0.0 && mVX < 0.0)
		mTurnAnimationTimer = 20;

	if (mTurnAnimationTimer > 0)
		mTurnAnimationTimer--;
	else if (mTurnAnimationTimer < 0)
		mTurnAnimationTimer++;

	if (mEatingAnimationTimer > 0)
	{
		mEatingAnimationTimer--;
		mAnimationFrameIndexFish = 9 - mEatingAnimationTimer / 2;
	}

	if (mTurnAnimationTimer == 0)
	{
		if (mEatingAnimationTimer < 1)
		{
			if (mVXAbs < 2)
				mSwimFrameCounter++;
			else
				mSwimFrameCounter += 2;
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
		}
	}
	else
	{
		mEatingAnimationTimer = 0;
		if (mTurnAnimationTimer < 0)
			mAnimationFrameIndexFish = mTurnAnimationTimer / 2 + 9;
		else if(mTurnAnimationTimer > 0)
			mAnimationFrameIndexFish = 9 - mTurnAnimationTimer / 2;
	}

	if (mPrevVX != mVX && mPrevVX != 0 && mVX != 0)
		mPrevVX = mVX;

	if (mVoraciousScreamCounter > 100 && mTurnAnimationTimer == 0)
		mAnimationFrameIndexFish = 4;

	if (mMisslePtr)
		UpdateCrosshairAnimation();
}

void Sexy::Ultra::Init()
{
	mType = TYPE_ULTRA;
	mSize = TYPE_ULTRA;
	mWidth = 160;
	mHeight = 160;
	mIsGuppy = false;
	mYMax = 310;
	mYMin = 75;
	mXMin = 0;
	mXMax = 480;
	mHunger = mApp->mSeed->Next() % 200 + 600;
	mMouseVisible = gUnkBool06;
	mCoinDropT = DetermineCoinDropT(mApp->mSeed->Next() % 250 + 200);
}
