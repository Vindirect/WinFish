#include "Grubber.h"
#include "WinFishApp.h"
#include "Res.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::Grubber::Grubber()
{
	mType = TYPE_GRUBBER;
	mClip = false;
}

Sexy::Grubber::Grubber(int theX)
{
	Grubber::Init();
	mX = theX;
	mXD = theX;
}

Sexy::Grubber::Grubber(int theX, int theY)
{
	Grubber::Init();
	mX = theX;
	mY = theY;
	mXD = theX;
	mYD = theY;
}

Sexy::Grubber::~Grubber()
{
}

void Sexy::Grubber::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (!HungryLogic())
	{
		if (mSpeedState == 0)
			mTargetVX = -0.5;
		else if (mSpeedState == 1)
			mTargetVX = 0.5;
		else if (mSpeedState == 2)
			mTargetVX = -1.0;
		else if (mSpeedState == 3)
			mTargetVX = 1.0;
		else if (mSpeedState == 4)
			mTargetVX = 0;
		else if (mSpeedState == 5)
			mTargetVX = 0;
		else if (mSpeedState == 6)
			mTargetVX = -2.5;
		else if (mSpeedState == 7)
			mTargetVX = 2.5;

		if (mVX > mTargetVX)
			mVX -= 0.1;
		if (mVX < mTargetVX)
			mVX += 0.1;
	}

	mEatFoodDelayTimer++;
	mSpeedChangeTimer++;
	if (mSpeedChangeTimer > 20 || mXD <= 10.0 || mXD >= 540.0)
	{
		mSpeedChangeTimer = 0;
		if (mApp->mSeed->Next() % 10 == 0)
			mSpeedState = mApp->mSeed->Next() % 8;
	}

	if (!mApp->mBoard->AliensInTank())
		CoinDrop();
	if (mXD > 560.0)
		mXD = 560.0;
	if (mXD < 10.0)
		mXD = 10.0;
	if (mYD > 355.0)
		mYD = 355.0;
	if (mBoughtTimer > 0)
	{
		int aVal = 5;
		if (mBoughtTimer > 35)
			aVal = 8;
		if (mApp->mSeed->Next() % aVal == 0)
			mApp->mBoard->SpawnBubble(mX + (55 - mApp->mSeed->Next() % 60), mY + (55 - mApp->mSeed->Next() % 60));
		mBoughtTimer--;
	}
	else
	{
		if (mYD < 95.0)
			mYD = 95.0;
	}

	if (mXD > 535.0 && mVX > 0.1)
		mVX -= 0.1;
	if (mXD < 15.0 && mVX < -0.1)
		mVX += 0.1;
	if (mYD < 355.0)
		mVY += 0.4;

	UpdateAnimations();
	double aSpeedMod = mSpeedMod;
	if (mSpeedy)
	{
		if (mSpeedySpeedState == 0)
			aSpeedMod = 0.3;
		else
			aSpeedMod = 1.5;
	}
	mXD += mVX / aSpeedMod;
	mYD += mVY / aSpeedMod;
	Move(mXD, mYD);
}

void Sexy::Grubber::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	DrawGrubber(g);
	if (!mName.empty())
		DrawName(g, false);
}

void Sexy::Grubber::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount < 0)
		mApp->mBoard->CheckMouseDown(mX + x, mY + y);
}

void Sexy::Grubber::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[0]++;
}

int Sexy::Grubber::SpecialReturnValue()
{
	return 7;
}

void Sexy::Grubber::Remove()
{
	Remove(true);
}

void Sexy::Grubber::SetPosition(double newX, double newY)
{
	mX = newX;
	mXD = newX;
	mY = newY;
	mYD = newY;
}

void Sexy::Grubber::OnFoodAte(GameObject* obj)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	mSpeedySpeedState = 100;
	if (mVirtualTankId > -1)
		UpdateMentalState();
	mApp->mBoard->PlayChompSound(mVoracious);
	mHunger += 1000;
	if (mHunger > 1400)
		mHunger = 1400;

	if (mApp->m0x882 && obj->mType != TYPE_FOOD)
	{
		for (int i = mApp->mSeed->Next() % 3 + 2; i > 0; i--)
		{
			mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 20 + 15 + obj->mX, mApp->mSeed->Next() % 20 + 15 + obj->mY, 1);
		}
	}
}

void Sexy::Grubber::UpdateStoreAnimation()
{
	mStoreAnimationTimer++;
	if (mInvisible)
		m0x128++;
	mStoreAnimationIndex = (mStoreAnimationTimer / 2) % 10;
}

void Sexy::Grubber::DrawStoreAnimation(Graphics* g, int justification)
{
	int anX = 0, anY = 0;
	switch (justification)
	{
	case 0:
		anX = 19;
		anY = 17;
		break;
	case 1:
		anX = 5;
		anY = 13;
		break;
	case 2:
		anX = 15;
		anY = -5;
		break;
	case 3:
		anX = 5;
		anY = 0;
		break;
	case 4:
		anX = -40;
		break;
	}
	g->Translate(anX, anY);
	Color aCol = Color::White;
	SetStoreColor(g, aCol);
	g->DrawImage(IMAGE_GRUBBER, 0, 0, Rect(mStoreAnimationIndex * 80, 0, 80, 80));
	g->SetColorizeImages(false);
	g->Translate(-anX, -anY);
}

void Sexy::Grubber::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncDouble(mSpeedMod);
	theSync->SyncLong(mBoughtTimer);
	theSync->SyncDouble(mTargetVX);
	theSync->SyncLong(mSpeedState);
	theSync->SyncLong(mEatFoodDelayTimer);
	theSync->SyncLong(mSpeedChangeTimer);
	theSync->SyncLong(mMovementAnimTimer);
	theSync->SyncLong(mGrubberAnimIndex);
	theSync->SyncLong(mEatingAnimTimer);
	theSync->SyncLong(mCoinDropTimer);
	theSync->SyncLong(mCoinDropThreshold);
}

void Sexy::Grubber::Init()
{
	mType = TYPE_GRUBBER;
	mClip = false;
	mYD = mApp->mSeed->Next() % 5 + 360;
	mY = mYD;
	mVX = 0;
	mVY = 0;
	mTargetVX = 0;
	mWidth = 80;
	mHeight = 80;
	int aRandSpeedMod = mApp->mSeed->Next() % 3;
	if (aRandSpeedMod == 0)
		mSpeedMod = 2.7;
	else if (aRandSpeedMod == 1)
		mSpeedMod = 2.5;
	else
		mSpeedMod = 2.6;
	mHunger = mApp->mSeed->Next() % 200 + 900;
	mEatFoodDelayTimer = 40;
	mSpeedChangeTimer = 0;
	mMovementAnimTimer = 0;
	mGrubberAnimIndex = 0;
	mEatingAnimTimer = 0;
	mCoinDropTimer = 0;
	mSpeedState = mApp->mSeed->Next() % 10;
	mCoinDropThreshold = DetermineCoinDropT(mApp->mSeed->Next() % 200 + 300);
	mMouseVisible = gUnkBool06;
	mBoughtTimer = 45;
}

bool Sexy::Grubber::HungryLogic()
{
	UpdateHungerAnimCounter();
	if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
		UpdateHungerCounter();

	if (ShouldDie())
		Die(true);
	else
	{
		if (mHunger < 900)
			return HungryBehavior();
	}
	return false;
}

bool Sexy::Grubber::HungryBehavior()
{
	GameObject* aNearestFood = FindNearestFood();
	if (mEatFoodDelayTimer >= 5 && aNearestFood != nullptr)
	{
		mEatFoodDelayTimer = 0;
		int aX = mXD + 40.0;
		int aFX = aNearestFood->mX;
		if (mHunger <= 300)
		{
			if (aX > aFX + 48)
			{
				if (mVX > -4.0)
					mVX -= 1.5;
			}
			else if (aX < aFX + 24)
			{
				if (mVX < 4.0)
					mVX += 1.5;
			}
			else if (aX > aFX + 36)
			{
				if (mVX > -2.5)
					mVX -= 0.8;
			}
			else if (aX < aFX + 36)
			{
				if (mVX < 2.5)
					mVX += 0.8;
			}
		}
		else
		{
			if (aX > aFX + 48)
			{
				if (mVX > -2.5)
					mVX -= 0.8;
			}
			else if (aX < aFX + 24)
			{
				if (mVX < 2.5)
					mVX += 0.8;
			}
			else if (aX > aFX + 36)
			{
				if (mVX > -1.5)
					mVX -= 0.6;
			}
			else if (aX < aFX + 36)
			{
				if (mVX < 1.5)
					mVX += 0.6;
			}
		}
	}
	if (aNearestFood)
	{
		if (mYD + 40.0 < (aNearestFood->mHeight > 80 ? aNearestFood->mY + 240 : aNearestFood->mY + 160) &&
			mYD + 40.0 > aNearestFood->mY - 20 &&
			mXD + 40.0 < aNearestFood->mX + 80 && 
			mXD + 40.0 > aNearestFood->mX && mYD >= 355.0)
		{
			mSpeedySpeedState = 100;
			ShowInvisibility();
			mVY = -14.0;
		}
		CollideWithFood();
	}
	return aNearestFood != nullptr;
}

void Sexy::Grubber::Die(bool flag)
{
	if (flag)
		mApp->mBoard->PlayDieSound(mType);
	Remove(false);
	bool isFacingRight = (mVX >= 0.0);
	mApp->mBoard->SpawnDeadFish(mXD, mYD, mVX, mVY, mSpeedMod, mType, isFacingRight, mShadowPtr);
}

void Sexy::Grubber::CollideWithFood()
{
	Board* aBoard = mApp->mBoard;
	if (mExoticDietFoodType == 0)
	{
		for (int i = 0; i < aBoard->mFishList->size(); i++)
		{
			Fish* aFish = aBoard->mFishList->at(i);

			int aFX = aFish->mX;
			int aFY = aFish->mY;
			if (aFish->mSize == TYPE_GUPPY && aFish->mVirtualTankId < 0 && aFish->mCanBeEatenDelay < 1)
			{
				if (mXD + 40.0 < aFX + 80 && mXD + 40.0 > aFX &&
					mYD + 40.0 < aFY + 70 && mYD + 40.0 > aFY + 10)
				{
					OnFoodAte(aFish);
					aFish->RemoveFromGame(true);
					if (mEatingAnimTimer == 0)
					{
						ShowInvisibility();
						mEatingAnimTimer = 12;
					}
					return;
				}
				else if (mEatingAnimTimer == 0 && mXD + 40.0 < aFX + 80 && mXD + 40.0 > aFX
					&& mYD + 40.0 < aFY + 160 && mYD + 40.0 > aFY - 20)
				{
					mEatingAnimTimer = 20;
					ShowInvisibility();
				}
			}
		}
	}
	else
	{
		int aVal = ExoticFoodCollision(mX + mWidth / 2, mY + mHeight / 2);
		if (mEatingAnimTimer == 0)
		{
			if (aVal == 1)
			{
				mEatingAnimTimer = 4;
				ShowInvisibility();
			}
			else if (aVal == 2)
			{
				mEatingAnimTimer = 8;
				ShowInvisibility();
			}
		}
	}
}

Sexy::GameObject* Sexy::Grubber::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(mX + mWidth / 2, mY + mHeight / 2);

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	GameObject* aRet = nullptr;
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
		mSpeedySpeedState = 100;
		VoraciousScream(150);
	}
	return aRet;
}

void Sexy::Grubber::Remove(bool removeShadow)
{
	if (mMisslePtr)
		mMisslePtr->Remove();
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (removeShadow && mShadowPtr)
		mShadowPtr->Remove();
	mApp->mBoard->m0x47c++;
	if (mApp->mBoard->IsTankAndLevelNB(3, 1))
	{
		if (mApp->mBoard->m0x4b0[38])
			mApp->mBoard->ShowText("Try luring small guppies down to your guppycrunchers!", false, 39);
		if (mApp->mBoard->m0x4b0[37])
			mApp->mBoard->ShowText("Guppycrunchers and carnivores share the same diet!", false, 38);
		mApp->mBoard->ShowText("Warning! Your guppycruncher has died!", false, 37);
	}
}

void Sexy::Grubber::CoinDrop()
{
	mCoinDropTimer++;
	if (CoinDropTimerPassed(mCoinDropTimer, mCoinDropThreshold))
	{
		mCoinDropTimer = 0;
		if (RelaxModeCanDrop())
			mApp->mBoard->SpawnLarva(mX + 4, mY - 10);
	}
}

void Sexy::Grubber::UpdateAnimations()
{
	UpdateInvisible();
	if (mEatingAnimTimer > 0)
	{
		mEatingAnimTimer--;
		mGrubberAnimIndex = 9 - mEatingAnimTimer / 2;
	}
	else if(mVX > 1.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer + 1) % 20;
		mGrubberAnimIndex = mMovementAnimTimer / 2;
	}
	else if(mVX < -1.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer - 1) % 20;
		if (mMovementAnimTimer < 0)
			mMovementAnimTimer += 20;
		mGrubberAnimIndex = mMovementAnimTimer / 2;
	}
	else if (mVX > 0.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer + 1) % 40;
		mGrubberAnimIndex = mMovementAnimTimer / 4;
	}
	else
	{
		mMovementAnimTimer = (mMovementAnimTimer - 1) % 40;
		if (mMovementAnimTimer < 0)
			mMovementAnimTimer += 40;
		mGrubberAnimIndex = mMovementAnimTimer / 4;
	}
	if (mVoraciousScreamCounter > 100)
		mGrubberAnimIndex = 4;
	if (mMisslePtr)
		UpdateCrosshairAnimation();
}

void Sexy::Grubber::DrawGrubber(Graphics* g)
{
	if (gZombieMode)
	{
		bool aHungr = IsHungryVisible();
		g->DrawImageMirror(IMAGE_GRUBBER, 0, 0, Rect(aHungr ? 480 : 720, 240, 80, 80), false);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);
		if (IsHungryBlipPointer(900))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
		return;
	}

	bool hungry = IsHungryVisible();
	Rect aSrcRect(mGrubberAnimIndex * 80, GetRowToDraw(hungry) * 80, 80, 80);
	if (mInvisible)
		if (DrawInvisibleEffect(g, IMAGE_GRUBBER, aSrcRect, false))
			return;
	SetColorHelper(g, Color::White);
	g->DrawImage(IMAGE_GRUBBER, 0, 0, aSrcRect);
	if (mHungerAnimationTimer != 0)
	{
		SetColorHelper(g, Color(255, 255, 255, mHungerAnimationTimer * 255 / 5));
		aSrcRect.mY = 160;
		g->DrawImage(IMAGE_GRUBBER, 0, 0, aSrcRect);
	}
	g->SetColorizeImages(false);
	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);
	if (IsHungryBlipPointer(900))
		g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
}

int Sexy::Grubber::GetRowToDraw(bool hungry)
{
	if (mEatingAnimTimer < 1 && mVoraciousScreamCounter <= 100)
		return hungry ? 2 : 0;
	return hungry ? 4 : 1;
}
