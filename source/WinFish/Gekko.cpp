#include "Gekko.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::Gekko::Gekko()
{
	mType = TYPE_GEKKO;
}

Sexy::Gekko::Gekko(int theX, int theY) : Fish(theX, theY)
{
	Gekko::Init();
}

Sexy::Gekko::Gekko(int theX, int theY, bool velocityRight) : Fish(theX, theY)
{
	Gekko::Init();
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

Sexy::Gekko::~Gekko()
{
}

void Sexy::Gekko::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[2]++;
}

void Sexy::Gekko::OnFoodAte(GameObject* obj)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	if (mVirtualTankId > -1)
		UpdateMentalState();
	if (obj->mType == TYPE_FOOD)
		mApp->mBoard->PlaySlurpSound(mVoracious);
	else
		mApp->mBoard->PlayChompSound(mVoracious);
	mHunger += 700;
	if (mHunger > 1000)
		mHunger = 1000;
	UpdateHungerStateIfWasHungry(hungry);
}

void Sexy::Gekko::DrawStoreAnimation(Graphics* g, int justification)
{
	int anXTrans = 0, anYTrans = 0;
	switch (justification)
	{
	case 0:
		anXTrans = 23;
		anYTrans = 20;
		break;
	case 1:
		anXTrans = 15;
		anYTrans = 17;
		break;
	case 2:
		anXTrans = 20;
		break;
	case 3:
		anXTrans = 12;
		anYTrans = 5;
		break;
	case 4:
		anXTrans = -40;
		break;
	}
	g->Translate(anXTrans, anYTrans);
	SetStoreColor(g, Color::White);
	g->DrawImage(IMAGE_GEKKO, 0, 0, Rect(mStoreAnimationIndex * 80, 0, 80, 80));
	g->SetColorizeImages(false);
	g->Translate(-anXTrans, -anYTrans);
}

void Sexy::Gekko::DropCoin()
{
	mCoinDropTimer++;
	if (mPreNamedTypeId == COOKIE)
	{
		if (mCoinDropTimer > 540 || mCoinDropTimer == 504)
		{
			int aReqs[8] = { 0 };
			int aInTank[8] = { 0 };
			mApp->mBoard->GetExoticFoodsRequiredInTank(aReqs);
			mApp->mBoard->GetExoticFoodsInTank(aInTank);
			std::vector<int> aFoodCandidates;
			for (int i = 0; i < 8;i++)
			{
				if (aReqs[i] != aInTank[i] && aReqs[i] - aInTank[i] > -1 && (i != 0 || mApp->mBoard->mPetsInTank[3] == 0))
					aFoodCandidates.push_back(i);
			}

			if (!aFoodCandidates.empty())
			{
				int aRandIdx = Rand() % aFoodCandidates.size();

				if (mCoinDropTimer > 540)
				{
					mApp->mBoard->SpawnVirtualTankFood(aFoodCandidates[aRandIdx]);
					if (mGrowthAnimationTimer == 0)
						mGrowthAnimationTimer = 36;
				}
				else if(mCoinDropTimer == 504)
					mGrowthAnimationTimer = 72;
			}
			if (mCoinDropTimer > 540)
				mCoinDropTimer = 0;
		}
	}
	else
	{
		if (CoinDropTimerPassed(mCoinDropTimer, mCoinDropT))
		{
			mCoinDropTimer = 0;
			if (RelaxModeCanDrop())
				mApp->mBoard->DropCoin(mX + 5, mY + 10, Coin::COIN_PEARL, nullptr, -1.0, 0);
		}
	}
}

bool Sexy::Gekko::Hungry()
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

void Sexy::Gekko::DrawFish(Graphics* g, bool mirror)
{
	DrawGekko(g, mirror);
	if (mGrowthAnimationTimer > 0)
	{
		if(((mGrowthAnimationTimer / 8) % 2) == 0)
		{
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			DrawGekko(g, mirror);
			g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		}
	}
}

bool Sexy::Gekko::HungryBehavior()
{
	GameObject* aFood = FindNearestFood();
	int aX = mXD + 40.0;
	int aY = mYD + 45.0;
	if (mExoticDietFoodType != 0 && mExoticDietFoodType != 6)
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
			if (aFX + 40 < aX)
			{
				if (mVX > -4.0)
					mVX -= 1.3;
			}
			else if (aFX + 32 > aX)
			{
				if (mVX < 4.0)
					mVX += 1.3;
			}
			else if (aFX + 38 < aX)
			{
				if (mVX > -4.0)
					mVX -= 0.2;
			}
			else if (aFX + 34 > aX)
			{
				if (mVX < 4.0)
					mVX += 0.2;
			}
			else if (aFX + 36 < aX)
			{
				if (mVX > -4.0)
					mVX -= 0.05;
			}
			else if (aFX + 36 > aX)
			{
				if (mVX < 4.0)
					mVX += 0.05;
			}

			if (aFY + 36 < aY)
			{
				if (mVY > -4.0)
					mVY -= 1.3;
			}
			else if (aFY + 36 > aY)
			{
				if (mVY < 4.0)
					mVY += 1.3;
			}
		}
		else
		{
			if (aFX + 40 < aX)
			{
				if (mVX > -3.0)
					mVX -= 1.0;
			}
			else if (aFX + 32 > aX)
			{
				if (mVX < 3.0)
					mVX += 1.0;
			}
			else if (aFX + 38 < aX)
			{
				if (mVX > -3.0)
					mVX -= 0.1;
			}
			else if (aFX + 34 > aX)
			{
				if (mVX < 3.0)
					mVX += 0.1;
			}
			else if (aFX + 36 < aX)
			{
				if (mVX > -3.0)
					mVX -= 0.05;
			}
			else if (aFX + 36 > aX)
			{
				if (mVX < 3.0)
					mVX += 0.05;
			}

			if (aFY + 39 < aY)
			{
				if (mVY > -3.0)
					mVY -= 1.0;
			}
			else if (aFY + 33 > aY)
			{
				if (mVY < 3.0)
					mVY += 1.0;
			}
			else if (aFY + 36 < aY)
			{
				if (mVY > -3.0)
					mVY -= 0.5;
			}
			else if (aFY + 36 > aY)
			{
				if (mVY < 3.0)
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

Sexy::GameObject* Sexy::Gekko::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
	{
		int aY = mY + mHeight / 2;
		if (mExoticDietFoodType != 0 && mExoticDietFoodType != 6)
			aY += 20;
		return FindNearestExoticFood(mX + mWidth / 2, aY);
	}

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	GameObject* aRet = nullptr;
	int aCenterX = mX + mWidth / 2;
	int aCenterY = mY + mHeight / 2;
	for (int i = 0; i < aBoard->mLarvaList->size(); i++)
	{
		Larva* aFood = aBoard->mLarvaList->at(i);

		if (!aFood->mPickedUp)
		{
			int ax = (aFood->mX + 20) - aCenterX;
			int ay = (aFood->mY + 20) - aCenterY;
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = aFood;
				aDist = aNewDist;
			}
		}
	}
	for (int i = 0; i < aBoard->mCoinList1->size(); i++)
	{
		Coin* aFood = aBoard->mCoinList1->at(i);
		if (aFood->mCoinType == 18 && !aFood->m0x198)
		{
			int ax = (aFood->mX + 20) - aCenterX;
			int ay = (aFood->mY + 20) - aCenterY;
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = aFood;
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

void Sexy::Gekko::CollideWithFood()
{
	Board* aBoard = mApp->mBoard;
	if (mExoticDietFoodType == 0)
	{
		for (int i = 0; i < aBoard->mLarvaList->size(); i++)
		{
			Larva* aFood = aBoard->mLarvaList->at(i);
			if (!aFood->mPickedUp && FoodCollided(aFood))
				return;
		}
		for (int i = 0; i < aBoard->mCoinList1->size(); i++)
		{
			Coin* aFood = aBoard->mCoinList1->at(i);
			if (aFood->mCoinType == 18 && !aFood->m0x198 && FoodCollided(aFood))
				return;
		}
	}
	else
	{
		int aY = mY + mHeight / 2;
		if (mExoticDietFoodType != 0 && mExoticDietFoodType != 6)
			aY += 20;
		int aVal = ExoticFoodCollision(mX + mWidth / 2, aY);
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

void Sexy::Gekko::RemoveFromGame(bool removeShadow)
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (removeShadow && mShadowPtr)
		mShadowPtr->Remove();
	mApp->mBoard->m0x484++;
}

void Sexy::Gekko::Init()
{
	mType = TYPE_GEKKO;
	mIsGuppy = false;
	mSize = TYPE_GEKKO;
	mYMin = 105;
	mYMax = 360;
	mMouseVisible = gUnkBool06;
	mCoinDropT = DetermineCoinDropT(mApp->mSeed->Next() % 250 + 200);
}

void Sexy::Gekko::DrawGekko(Graphics* g, bool mirror)
{
	if (gZombieMode)
	{
		bool aHungr = IsHungryVisible();
		g->DrawImageMirror(IMAGE_GEKKO, 0, 0, Rect(aHungr ? 480 : 720, 400, 80, 80), mirror);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);
		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
		return;
	}

	bool aHungr = IsHungryVisible();
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, GetDrawRow(aHungr) * 80, 80, 80);

	if (mInvisible)
		if (DrawInvisibleEffect(g, IMAGE_GEKKO, aSrcRect, mirror))
			return;

	SetColorHelper(g, Color::White);
	g->DrawImageMirror(IMAGE_GEKKO, 0, 0, aSrcRect, mirror);
	if (mHungerAnimationTimer != 0)
	{
		aSrcRect.mY = GetDrawRow(true) * 80;
		SetColorHelper(g, Color(255, 255, 255, mHungerAnimationTimer * 255 / 5));
		g->DrawImageMirror(IMAGE_GEKKO, 0, 0, aSrcRect, mirror);
	}
	g->SetColorizeImages(false);

	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);

	if (IsHungryBlipPointer(500))
		g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
}

int Sexy::Gekko::GetDrawRow(bool isHungry)
{
	if (mTurnAnimationTimer != 0)
		return isHungry ? 4 : 1;
	if (mEatingAnimationTimer <= 0 && mVoraciousScreamCounter <= 100)
		return isHungry ? 3 : 0;
	return isHungry ? 6 : 2;
}

bool Sexy::Gekko::FoodCollided(GameObject* theObj)
{
	int aFX = theObj->mX;
	int aFY = theObj->mY;
	if (mXD + 40.0 < aFX + 54 && mXD + 40.0 > aFX + 18 &&
		mYD + 40.0 < aFY + 54 && mYD + 40.0 > aFY + 18)
	{
		OnFoodAte(theObj);
		theObj->Remove();
		if (mEatingAnimationTimer == 0)
		{
			ShowInvisibility();
			mEatingAnimationTimer = 8;
		}
		return true;
	}
	else if (mEatingAnimationTimer == 0 && mXD + 40.0 < aFX + 76 && mXD + 40.0 > aFX - 4
		&& mYD + 40.0 < aFY + 66 && mYD + 40.0 > aFY - 6)
	{
		mEatingAnimationTimer = 20;
		ShowInvisibility();
	}
	return false;
}
