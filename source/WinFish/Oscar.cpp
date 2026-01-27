#include "Oscar.h"
#include "WinFishApp.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Board.h"
#include "Res.h"

using namespace Sexy;

Sexy::Oscar::Oscar() : Fish()
{
	mType = TYPE_OSCAR;
}

Sexy::Oscar::Oscar(int theX, int theY) : Fish(theX, theY)
{
	Init(theX, theY);
}

Sexy::Oscar::Oscar(int theX, int theY, bool velocityRight) : Fish(theX, theY)
{
	Init(theX, theY);
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

void Sexy::Oscar::Init(int theX, int theY)
{
	mType = TYPE_OSCAR;
	mSize = TYPE_OSCAR;
	mIsGuppy = false;
	mYMax = 360;
	mHunger = mApp->mSeed->Next() % 200 + 600;
	mMouseVisible = gUnkBool06;
	if (mApp->mRelaxMode)
	{
		SetFishColors(Rand() % 12 + 30, false);
		mHasSpecialColors = true;
	}
}

void Sexy::Oscar::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[0]++;
}

void Sexy::Oscar::OnFoodAte(GameObject* obj)
{
	mApp->mBoard->PlayChompSound(mVoracious);
	bool wasHungry = IsHungryVisible();
	GameObject::Unk02(false);
	if (mVirtualTankId >= 0)
		UpdateMentalState();
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

	UpdateHungerStateIfWasHungry(wasHungry);
	if (mPreNamedTypeId == JOHNNYV)
	{
		int aRandVal = Rand();
		if (aRandVal % 3 == 0)
		{
			mApp->mBoard->MakeNote(mX - 40 + mWidth / 2, mY - 10, 2, "This is my last pizza,");
			mApp->mBoard->MakeNote(mX + mWidth / 2, mY + 5, 2, "I swear!");
		}
	}
}

void Sexy::Oscar::DrawStoreAnimation(Graphics* g, int unk)
{
	int anXTrans = 0, anYTrans = 0;
	switch (unk)
	{
	case 0:
		anXTrans = 17;
		anYTrans = 17;
		break;
	case 1:
		anXTrans = 7;
		anYTrans = 17;
		break;
	case 2:
		anXTrans = 17;
		break;
	case 3:
		anXTrans = 8;
		anYTrans = 2;
		break;
	case 4:
		anXTrans = -40;
		break;
	}
	g->Translate(anXTrans, anYTrans);
	if (!mHasSpecialColors)
	{
		SetStoreColor(g, Color::White);
		g->DrawImage(IMAGE_SMALLSWIM, 0, 0, Rect(mStoreAnimationIndex * 80, 320, 80, 80));
	}
	else
	{
		Rect aSrcRect(mStoreAnimationIndex * 80, 240, 80, 80);
		SetStoreColor(g, Color::White);
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect, false);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);

		SetStoreColor(g, mColors[1]);
		aSrcRect.mY = 80;
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect, false);

		SetStoreColor(g, mColors[0]);
		aSrcRect.mY = 0;
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect, false);

		SetStoreColor(g, mColors[2]);
		aSrcRect.mY = 160;
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect, false);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}
	g->SetColorizeImages(false);
	g->Translate(-anXTrans, -anYTrans);
}

void Sexy::Oscar::DropCoin()
{
	mCoinDropTimer++;
	if (CoinDropTimerPassed(mCoinDropTimer, mCoinDropT))
	{
		mCoinDropTimer = 0;
		if (RelaxModeCanDrop())
			mApp->mBoard->DropCoin(mX + 5, mY + 10, Coin::COIN_DIAMOND, nullptr, -1.0, 0);
	}
}

bool Sexy::Oscar::Hungry()
{
	UpdateHungerAnimCounter();
	if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
	{
		UpdateHungerCounter();
	}

	if (ShouldDie())
		Die(true);
	else
	{
		if (mHunger < 500)
			return HungryBehavior();
	}
	return false;
}

void Sexy::Oscar::DrawFish(Graphics* g, bool mirror)
{
	if (gZombieMode)
	{
		bool aHungr = IsHungryVisible();
		g->DrawImageMirror(IMAGE_SMALLDIE, 0, 0, Rect(aHungr ? 480 : 720, 320, 80, 80), mirror);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);
		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, -10, 2);
		return;
	}

	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, 320, 80, 80);
	bool aHungr = IsHungryVisible();

	if (mInvisible)
	{
		Image* anImg = GetImageToDraw(aHungr);
		if (DrawInvisibleEffect(g, anImg, aSrcRect, mirror))
			return;
	}

	if (!mHasSpecialColors || aHungr)
	{
		SetColorHelper(g, Color::White);
		g->DrawImageMirror(GetImageToDraw(aHungr), 0, 0, aSrcRect, mirror);
	}
	else
	{
		SetColorHelper(g, Color::White);

		int aRow = 0;
		if (mTurnAnimationTimer == 0)
		{
			if (mEatingAnimationTimer > 0 || mVoraciousScreamCounter > 100)
				aRow = 8;
		}
		else
			aRow = 4;

		Rect aSrcRect2 = Rect(mAnimationFrameIndexFish * 80, (aRow * 5 + 15) * 16, 80, 80);
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect2, mirror);

		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		SetColorHelper(g, mColors[1]);
		aSrcRect2 = Rect(mAnimationFrameIndexFish * 80, (aRow * 80) + 80, 80, 80);
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect2, mirror);
		SetColorHelper(g, mColors[0]);

		aSrcRect2 = Rect(mAnimationFrameIndexFish * 80, (aRow * 80), 80, 80);
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect2, mirror);
		SetColorHelper(g, mColors[2]);
		aSrcRect2 = Rect(mAnimationFrameIndexFish * 80, (aRow * 5 + 10) * 16, 80, 80);
		g->DrawImageMirror(IMAGE_SIMCARNIVORE, 0, 0, aSrcRect2, mirror);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}

	g->SetColorizeImages(false);
	if (mHungerAnimationTimer != 0)
	{
		SetColorHelper(g, Color(255, 255, 255, mHungerAnimationTimer * 255 / 5));
		g->DrawImageMirror(GetImageToDraw(true), 0, 0, aSrcRect, mirror);
		g->SetColorizeImages(false);
	}

	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);

	if (IsHungryBlipPointer(500))
		g->DrawImageCel(IMAGE_MISCITEMS, 0, -10, 2);
}

bool Sexy::Oscar::HungryBehavior()
{
	GameObject* aFood = FindNearestFood();
	int aX = mXD + 40.0;
	int aY = mYD + 50.0;
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
			if (aFX + 48 < aX)
			{
				if (mVX > -5.0)
					mVX -= 1.5;
			}
			else if (aFX + 32 > aX)
			{
				if (mVX < 5.0)
					mVX += 1.5;
			}
			else if (aFX + 44 < aX)
			{
				if (mVX > -5.0)
					mVX -= 0.2;
			}
			else if (aFX + 36 > aX)
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

			if (aFY + 46 < aY)
			{
				if (mVY > -3.0)
					mVY -= 1.3;
			}
			else if (aFY + 34 > aY)
			{
				if (mVY < 4.0)
					mVY += 1.5;
			}
			else if (aFY + 40 < aY)
			{
				if (mVY > -3.0)
					mVY -= 0.5;
			}
			else if (aFY + 40 > aY)
			{
				if (mVY < 4.0)
					mVY += 0.7;
			}
		}
		else
		{
			if (aFX + 48 < aX)
			{
				if (mVX > -4.0)
					mVX -= 1.5;
			}
			else if (aFX + 32 > aX)
			{
				if (mVX < 4.0)
					mVX += 1.5;
			}
			else if (aFX + 44 < aX)
			{
				if (mVX > -4.0)
					mVX -= 0.2;
			}
			else if (aFX + 36 > aX)
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

			if (aFY + 46 < aY)
			{
				if (mVY > -3.0)
					mVY -= 0.8;
			}
			else if (aFY + 34 > aY)
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

GameObject* Sexy::Oscar::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(mX + mWidth / 2, mY + mHeight / 2 + 20);

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	Fish* aRet = nullptr;
	for (int i = 0; i < aBoard->mFishList->size(); i++)
	{
		Fish* aFish = aBoard->mFishList->at(i);

		if (aFish->mSize == TYPE_GUPPY && aFish->mVirtualTankId < 0 && aFish->mCanBeEatenDelay < 1)
		{
			int ax = (mXD + 40) - (aFish->mX + 40);
			int ay = (mYD + 40) - (aFish->mY + 40);
			int aNewDist = ax * ax + ay * ay;
			if (aNewDist < aDist)
			{
				aRet = aFish;
				aDist = aNewDist;
			}
		}
	}
	if (aDist < 10000)
	{
		VoraciousScream(150);
		mSpeedySpeedState = 100;
	}
	return aRet;
}

void Sexy::Oscar::CollideWithFood()
{
	if (mExoticDietFoodType != 0)
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
		return;
	}

	Board* aBoard = mApp->mBoard;
	for (int i = 0; i < aBoard->mFishList->size(); i++)
	{
		Fish* aFish = aBoard->mFishList->at(i);

		int aFX = aFish->mX;
		int aFY = aFish->mY;
		if (aFish->mVirtualTankId < 0 && aFish->mCanBeEatenDelay < 1)
		{
			if (mXD + 40.0 < aFX + 70 && mXD + 40.0 > aFX + 10 &&
				mYD + 45.0 < aFY + 58 && mYD + 45.0 > aFY + 22 && aFish->mSize == TYPE_GUPPY)
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
			else if (mEatingAnimationTimer == 0 && mXD + 40.0 < aFX + 87 && mXD + 40.0 > aFX - 7
				&& mYD + 45.0 < aFY + 70 && mYD + 45.0 > aFY + 10 && aFish->mSize == TYPE_GUPPY)
			{
				mEatingAnimationTimer = 20;
				ShowInvisibility();
			}
		}
	}
}

void Sexy::Oscar::RemoveFromGame(bool aRemoveShadow)
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (aRemoveShadow && mShadowPtr)
		mShadowPtr->Remove();
	mApp->mBoard->m0x468++;
	if (mApp->mBoard->IsTankAndLevelNB(1, 3))
	{
		if (mApp->mBoard->m0x4b0[28])
			mApp->mBoard->ShowText("Try feeding small guppies to your carnivores!", false, 29);
		if (mApp->mBoard->m0x4b0[27])
			mApp->mBoard->ShowText("Hint: Carnivores won\'t eat fish food!", false, 28);
		mApp->mBoard->ShowText("Warning! Your carnivore has died!", false, 27);
	}
}

