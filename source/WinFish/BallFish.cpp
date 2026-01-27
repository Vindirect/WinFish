#include "BallFish.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

Sexy::BallFish::BallFish()
{
	mType = TYPE_BALL_FISH;
}

Sexy::BallFish::BallFish(int theX, int theY, bool velocityRight) : Fish(theX, theY)
{
	BallFish::Init();
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

Sexy::BallFish::~BallFish()
{
}

void Sexy::BallFish::CountRequiredFood(int* theFoodReqPtr)
{
	if (mSize == SIZE_SMALL)
		theFoodReqPtr[7]++;
	else if(mSize == SIZE_MEDIUM)
		theFoodReqPtr[5]++;
	else
		theFoodReqPtr[6]++;
}

int Sexy::BallFish::GetShellPrice()
{
	int aVal = GameObject::GetShellPrice();
	if (aVal > 0)
	{
		if (mSize == SIZE_MEDIUM)
			aVal *= 2;
		else if (mSize == SIZE_LARGE)
			aVal *= 3;
	}
	return aVal;
}

void Sexy::BallFish::OnFoodAte(GameObject* obj)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	mApp->mBoard->PlaySlurpSound(mVoracious);
	mHunger += 700;
	if (mHunger > 1000)
		mHunger = 1000;
	if (UpdateMentalState())
	{
		mFoodAte++;
		if (mSize < SIZE_LARGE && mFoodNeededToGrow <= mFoodAte)
		{
			mSize++;
			mFoodAte = 0;
			mGrowthAnimationTimer = 10;
			mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
		}
	}
	UpdateHungerStateIfWasHungry(hungry);
}

void Sexy::BallFish::UpdateStoreAnimation()
{
	mStoreAnimationTimer++;
	if (mInvisible)
		m0x128++;
	mStoreAnimationIndex = (mStoreAnimationTimer / 4) % 10;
}

void Sexy::BallFish::DrawStoreAnimation(Graphics* g, int justification)
{
	int anXTrans = 0, anYTrans = 0;
	int aRow = 0;
	if (mSize == SIZE_MEDIUM)
		aRow = 2;
	else if (mSize == SIZE_LARGE)
		aRow = 1;
	switch (justification)
	{
	case 0:
		anXTrans = 35;
		anYTrans = 35;
		break;
	case 1:
		anXTrans = 20;
		anYTrans = 30;
		break;
	case 2:
		anXTrans = 30;
		anYTrans = 10;
		break;
	case 3:
		anXTrans = 20;
		anYTrans = 15;
		break;
	case 4:
		anXTrans = -28;
		anYTrans = 10;
		break;
	}

	g->Translate(anXTrans, anYTrans);
	SetStoreColor(g, Color::White);
	g->DrawImage(IMAGE_BALLS, 0, 0, Rect(mStoreAnimationIndex * 50, aRow * 50, 50, 50));
	g->SetColorizeImages(false);
	g->Translate(-anXTrans, -anYTrans);
}

void Sexy::BallFish::DropCoin()
{
	mCoinDropTimer++;
	if (!CoinDropTimerPassed(mCoinDropTimer, mCoinDropT))
		return;
	mCoinDropTimer = 0;
	if (RelaxModeCanDrop())
	{
		int aShellType = 1;
		if (mSize == SIZE_MEDIUM)
			aShellType = 2;
		else if (mSize == SIZE_LARGE)
			aShellType = 4;
		mApp->mBoard->DropCoin(mX + 5, mY + 10, aShellType, nullptr, -1.0, 0);
	}
}

bool Sexy::BallFish::Hungry()
{
	UpdateHungerAnimCounter();
	if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
		UpdateHungerCounter();
	if (mHunger < 500)
		return HungryBehavior();
	return false;
}

void Sexy::BallFish::DrawFish(Graphics* g, bool mirror)
{
	int aRow = 0;
	if (mSize == SIZE_MEDIUM)
		aRow = 2;
	else if (mSize == SIZE_LARGE)
		aRow = 1;

	Rect aSrcRect(mAnimationFrameIndexFish * 50, aRow * 50, 50, 50);
	if(mInvisible && mGrowthAnimationTimer == 0)
		if (DrawInvisibleEffect(g, IMAGE_BALLS, aSrcRect, mirror))
			return;

	SetColorHelper(g, Color::White);
	if (mGrowthAnimationTimer <= 0)
		g->DrawImageMirror(IMAGE_BALLS, 0, 0, aSrcRect, mirror);
	else
	{
		double aGrowthVal = 0;
		if (mGrowthAnimationTimer > 3)
			aGrowthVal = ((10 - mGrowthAnimationTimer) * 0.7) / 7.0 + 0.5;
		else
			aGrowthVal = (mGrowthAnimationTimer * 0.2) / 3.0 + 1.0;

		int aVal = (aGrowthVal - 1.0) * 80.0;

		Rect aDestRect(0, 0, 0, 0);
		aDestRect.mX = -aVal;
		aDestRect.mY = aDestRect.mY;
		aDestRect.mWidth = aVal * 2 + 50;
		aDestRect.mHeight = aDestRect.mWidth;

		g->SetFastStretch(!mApp->Is3DAccelerated());
		g->DrawImageMirror(IMAGE_BALLS, aDestRect, aSrcRect, mirror);
	}
	g->SetColorizeImages(false);
	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);
	if (IsHungryBlipPointer(500))
		g->DrawImageCel(IMAGE_MISCITEMS, -15, -20, 2);
}

bool Sexy::BallFish::HungryBehavior()
{
	GameObject* aNearestFood = FindNearestFood();

	int aCenterX = mX + mWidth / 2;
	int aCenterY = mY + mHeight / 2;

	if (mSpecialMovementStateChangeTimer > 2)
	{
		if (!aNearestFood)
			return false;
		mSpecialMovementStateChangeTimer = 0;
		int aFCX = aNearestFood->mX;
		int aFCY = aNearestFood->mY;
		if (mHunger < 301)
		{
			if (aCenterX > aFCX + 24) {
				if (mVX > -4.0) mVX -= 1.3;
			}
			else if (aCenterX < aFCX + 16) {
				if (mVX < 4.0) mVX += 1.3;
			}
			else if (aCenterX > aFCX + 22) {
				if (mVX > -4.0) mVX -= 0.2;
			}
			else if (aCenterX < aFCX - 18) {
				if (mVX < 4.0) mVX += 0.2;
			}
			else if (aCenterX > aFCX + 20) {
				if (mVX > -4.0) mVX -= 0.05;
			}
			else if (aCenterX < aFCX + 20) {
				if (mVX < 4.0) mVX += 0.05;
			}

			if (aCenterY > aFCY + 20) {
				if (mVY > -4.0) mVY -= 1.3;
			}
			else if (aCenterY < aFCY + 20) {
				if (mVY < 4.0) mVY += 1.3;
			}
		}
		else
		{
			if (aCenterX > aFCX + 24) {
				if (mVX > -3.0) mVX -= 1.0;
			}
			else if (aCenterX < aFCX + 16) {
				if (mVX < 3.0) mVX += 1.0;
			}
			else if (aCenterX > aFCX + 22) {
				if (mVX > -3.0) mVX -= 0.1;
			}
			else if (aCenterX < aFCX + 18) {
				if (mVX < 3.0) mVX += 0.1;
			}
			else if (aCenterX > aFCX + 20) {
				if (mVX > -3.0) mVX -= 0.05;
			}
			else if (aCenterX < aFCX + 20) {
				if (mVX < 3.0) mVX += 0.05;
			}

			if (aCenterY > aFCY + 23) {
				if (mVY > -3.0) mVY -= 1.0;
			}
			else if (aCenterY < aFCY + 17) {
				if (mVY < 3.0) mVY += 1.0;
			}
			else if (aCenterY > aFCY + 20) {
				if (mVY > -3.0) mVY -= 0.5;
			}
			else if (aCenterY < aFCY + 20) {
				if (mVY < 3.0) mVY += 0.5;
			}
		}
		if (mVXAbs < 5)
			mVXAbs++;
	}
	if (aNearestFood)
		CollideWithFood();
	return aNearestFood != nullptr;
}

Sexy::GameObject* Sexy::BallFish::FindNearestFood()
{
	int aXCenter = mX + mWidth / 2;
	int aYCenter = mY + mHeight / 2;
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(aXCenter, aYCenter);

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	GameObject* aRet = nullptr;
	int aFoodType = GetFoodType();

	for (GameObjectSet::iterator it = aBoard->mGameObjectSet.begin(); it != aBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;

		if (anObj->mVirtualTankId < 0 && anObj->mCanBeEatenDelay < 1 && anObj->mType == TYPE_FOOD)
		{
			if (CanBallFishEatFood(anObj, aFoodType))
			{
				int ax = aXCenter - (anObj->mX + anObj->mWidth / 2);
				int ay = aYCenter - (anObj->mY + anObj->mHeight / 2);
				int aNewDist = ax * ax + ay * ay;
				if (aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}
	}
	if (aDist < 10000)
	{
		mSpeedySpeedState = 100;
	}
	return aRet;
}

void Sexy::BallFish::CollideWithFood()
{
	Board* aBoard = mApp->mBoard;
	int aCenterX = mX + mWidth / 2;
	int aCenterY = mY + mHeight / 2;
	if (mExoticDietFoodType == 0)
	{
		int aFoodType = GetFoodType();
		for (GameObjectSet::iterator it = aBoard->mGameObjectSet.begin(); it != aBoard->mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;

			if (anObj->mVirtualTankId < 0 && anObj->mCanBeEatenDelay < 1 && 
				anObj->mType == TYPE_FOOD && CanBallFishEatFood(anObj, aFoodType))
			{
				int ax = aCenterX - (anObj->mX + anObj->mWidth / 2);
				int ay = aCenterY - (anObj->mY + anObj->mHeight / 2);
				if (ax > -30 && ax < 30 && ay > -30 && ay < 30)
				{
					ShowInvisibility();
					OnFoodAte(anObj);
					anObj->Remove();
					return;
				}
			}
		}
	}
	else
	{
		if (ExoticFoodCollision(aCenterX, aCenterY) >= 1)
			ShowInvisibility();
	}
}

void Sexy::BallFish::RemoveFromGame(bool removeShadow)
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

void Sexy::BallFish::FishUpdateAnimation()
{
	if (mInvisible)
		UpdateInvisible();

	if (mPrevVX < 0.0 && mVX > 0.0)
		mTurnAnimationTimer = -10;
	else if (mPrevVX > 0.0 && mVX < 0.0)
		mTurnAnimationTimer = 10;

	if (mTurnAnimationTimer > 0)
		mTurnAnimationTimer--;
	else if (mTurnAnimationTimer < 0)
		mTurnAnimationTimer++;

	if (mVX < -1.0)
		mSwimFrameCounter -= 3;
	else if(mVX > 1.0)
		mSwimFrameCounter += 3;
	else if (mVX < -0.5)
		mSwimFrameCounter -= 2;
	else if (mVX > 0.5)
		mSwimFrameCounter += 2;
	else if (mVX < 0.0)
		mSwimFrameCounter--;
	else
		mSwimFrameCounter++;

	int aFrameSpeed = mSize != SIZE_MEDIUM ? 6 : 12;
	if (mSwimFrameCounter < 0)
		mSwimFrameCounter += aFrameSpeed * 10;

	mAnimationFrameIndexFish = (mSwimFrameCounter / aFrameSpeed) % 10;

	if (mGrowthAnimationTimer > 0)
		mGrowthAnimationTimer--;

	if (mPrevVX != mVX && mPrevVX != 0 && mVX != 0)
		mPrevVX = mVX;

	if (mMisslePtr)
		UpdateCrosshairAnimation();
}

void Sexy::BallFish::Init()
{
	mType = TYPE_BALL_FISH;
	mIsGuppy = false;
	mSize = SIZE_SMALL;
	mYMin = 105;
	mYMax = 360;
	mMouseVisible = gUnkBool06;
	mCoinDropT = DetermineCoinDropT(mApp->mSeed->Next() % 250 + 200);
	mWidth = 50;
	mHeight = 50;
}

int Sexy::BallFish::GetFoodType()
{
	if (mSize == SIZE_SMALL)
		return Food::EXO_FOOD_ICE_CREAM;
	else if (mSize == SIZE_MEDIUM)
		return Food::EXO_FOOD_CHICKEN;
	return Food::EXO_FOOD_PIZZA;
}

bool Sexy::BallFish::CanBallFishEatFood(GameObject* theFood, int theExoticFoodTypePreffered)
{
	if (theFood->mType != TYPE_FOOD)
		return false;
	Food* aFood = (Food*)theFood;
	if (aFood->mCantEatTimer == 0 && aFood->mExoticFoodType == theExoticFoodTypePreffered)
		return true;
	return false;
}
