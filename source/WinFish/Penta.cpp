#include "Penta.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::Penta::Penta()
{
	mClip = false;
	mType = TYPE_PENTA;
}

Sexy::Penta::Penta(int theX)
{
	Penta::Init();
	mX = theX;
	mXD = theX;
}

Sexy::Penta::Penta(int theX, int theY)
{
	Penta::Init();
	mX = theX;
	mY = theY;
	mXD = theX;
	mYD = theY;
}

Sexy::Penta::~Penta()
{
}

void Sexy::Penta::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if (!HungryLogic())
	{
		if (mSpeedState == 0)
			mTargetVX = 0;
		else if(mSpeedState == 1)
			mTargetVX = -0.5;
		else if (mSpeedState == 2)
			mTargetVX = 0.5;
		else if (mSpeedState == 3)
			mTargetVX = -1.0;
		else if (mSpeedState == 4)
			mTargetVX = 1.0;
		else if (mSpeedState == 5)
			mTargetVX = 0;
		else if (mSpeedState == 6)
			mTargetVX = 0;
		else if (mSpeedState == 7)
			mTargetVX = -2.5;
		else if (mSpeedState == 8)
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
			mSpeedState = mApp->mSeed->Next() % 9;
	}

	if (mXD > 550.0)
		mXD = 550.0;
	if (mXD < 10.0)
		mXD = 10.0;
	if (mYD > 359.0)
	{
		mYD = 359.0;
		mVY = 0.0;
	}
	if (mBoughtTimer <= 0)
	{
		if (mYD < 95.0)
			mYD = 95.0;
	}
	else
	{
		int aVal = 5;
		if (mBoughtTimer > 35)
			aVal = 8;
		if (mApp->mSeed->Next() % aVal == 0)
			mApp->mBoard->SpawnBubble(mX + (55 - mApp->mSeed->Next() % 60), mY + (55 - mApp->mSeed->Next() % 60));
		mBoughtTimer--;
	}

	if (mXD > 535.0 && mVX > 0.1)
		mVX -= 0.1;
	if (mXD < 15.0 && mVX < -0.1)
		mVX += 0.1;
	if (mYD < 359.0)
		mVY += 0.4;
	if (mInvisible)
		UpdateInvisible();

	UpdateAnimations();

	if (mMisslePtr)
		UpdateCrosshairAnimation();

	double aSpeedMod = mSpeedMod;
	if (mSpeedy)
	{
		if (mSpeedySpeedState == 0)
			aSpeedMod = 0.3;
		else
			aSpeedMod = 0.8;
	}
	mXD += mVX / aSpeedMod;
	mYD += mVY / aSpeedMod;
	Move(mXD, mYD);
}

void Sexy::Penta::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	DrawPenta(g);
	if (!mName.empty())
		DrawName(g, false);
}

void Sexy::Penta::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount < 0)
		mApp->mBoard->CheckMouseDown(mX + x, mY + y);
}

void Sexy::Penta::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[1]++;
}

int Sexy::Penta::SpecialReturnValue()
{
	return 4;
}

void Sexy::Penta::Remove()
{
	Remove(true);
}

void Sexy::Penta::SetPosition(double newX, double newY)
{
	mX = newX;
	mXD = newX;
	mY = newY;
	mYD = newY;
}

void Sexy::Penta::OnFoodAte(GameObject* obj)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	if (mVirtualTankId > -1)
		UpdateMentalState();
	mHunger += 900;
	if (mHunger > 1300)
		mHunger = 1300;

	if (DropCoin())
		mApp->mBoard->PlaySample(SOUND_BUY_ID, 3, 1.0);
	else
		mApp->mBoard->PlaySlurpSound(mVoracious);
	UpdateHungerStateIfWasHungry(hungry);
}

void Sexy::Penta::UpdateStoreAnimation()
{
	mStoreAnimationTimer++;
	if (mInvisible)
		m0x128++;
	mStoreAnimationIndex = (mStoreAnimationTimer / 2) % 10;
}

void Sexy::Penta::DrawStoreAnimation(Graphics* g, int justification)
{
	int anX = 0, anY = 0;
	switch (justification)
	{
	case 0:
		anX = 20;
		anY = 17;
		break;
	case 1:
		anX = 5;
		anY = 17;
		break;
	case 2:
		anX = 15;
		anY = -5;
		break;
	case 3:
		anX = 5;
		anY = -1;
		break;
	case 4:
		anX = -40;
		break;
	}
	g->Translate(anX, anY);
	Color aCol = Color::White;
	SetStoreColor(g, aCol);
	g->DrawImage(IMAGE_STARCATCHER, 0, 0, Rect(mStoreAnimationIndex * 80, 0, 80, 80));
	g->SetColorizeImages(false);
	g->Translate(-anX, -anY);
}

void Sexy::Penta::Sync(DataSync* theSync)
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
	theSync->SyncLong(mPentaAnimIndex);
	theSync->SyncLong(mCoinDropTimer);
	theSync->SyncLong(m0x1a8);
}

void Sexy::Penta::Init()
{
	mClip = false;
	mType = TYPE_PENTA;
	mYD = mApp->mSeed->Next() % 5 + 360;
	mY = mYD;
	mVX = 0;
	mVY = 0;
	mTargetVX = 0;
	mWidth = 80;
	mHeight = 80;
	int aRandVal = mApp->mSeed->Next();
	if (aRandVal % 3 == 0)
		mSpeedMod = 2.7;
	else if (aRandVal % 3 == 1)
		mSpeedMod = 2.5;
	else
		mSpeedMod = 2.6;

	mHunger = mApp->mSeed->Next() % 200 + 900;
	mSpeedChangeTimer = 0;
	mMovementAnimTimer = 0;
	mPentaAnimIndex = 0;
	mCoinDropTimer = 0;
	mEatFoodDelayTimer = 40;
	m0x1a8 = 40;
	mSpeedState = mApp->mSeed->Next() % 10;
	mMouseVisible = gUnkBool06;
	mBoughtTimer = 45;
}

bool Sexy::Penta::HungryLogic()
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

bool Sexy::Penta::HungryBehavior()
{
	GameObject* aNearestFood = FindNearestFood();
	if (mEatFoodDelayTimer >= 5 && aNearestFood != nullptr)
	{
		mEatFoodDelayTimer = 0;
		int aX = mXD + 40.0;
		int aFX = aNearestFood->mX;
		if (mHunger <= 300)
		{
			if (aX > aFX + 46)
			{
				if (mVX > -6.0)
					mVX -= 2.5;
			}
			else if(aX < aFX + 26)
			{
				if (mVX < 6.0)
					mVX += 2.5;
			}
			else if (aX > aFX + 36)
			{
				if (mVX > -2.5)
					mVX -= 1.2;
			}
			else if (aX < aFX + 36)
			{
				if (mVX < 2.5)
					mVX += 1.2;
			}
		}
		else
		{
			if (aX > aFX + 46)
			{
				if (mVX > -4.5)
					mVX -= 1.8;
			}
			else if (aX < aFX + 26)
			{
				if (mVX < 4.5)
					mVX += 1.8;
			}
			else if (aX > aFX + 36)
			{
				if (mVX > -1.5)
					mVX -= 1.0;
			}
			else if (aX < aFX + 36)
			{
				if (mVX < 1.5)
					mVX += 1.0;
			}
		}
	}
	if (aNearestFood)
		CollideWithFood();
	return aNearestFood != nullptr;
}

void Sexy::Penta::Die(bool flag)
{
	if (flag)
		mApp->mBoard->PlayDieSound(mType);
	Remove(false);
	bool isFacingRight = (mVX >= 0.0);
	mApp->mBoard->SpawnDeadFish(mXD, mYD, mVX, mVY, mSpeedMod, mType, isFacingRight, mShadowPtr);
}

void Sexy::Penta::DrawPenta(Graphics* g)
{
	if (gZombieMode)
	{
		bool aHungr = IsHungryVisible();
		g->DrawImageMirror(IMAGE_STARCATCHER, 0, 0, Rect(aHungr ? 320 : 720, 160, 80, 80), false);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);
		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, -10, 2);
		return;
	}

	bool hungry = IsHungryVisible();
	Rect aSrcRect(mPentaAnimIndex * 80, hungry ? 80 : 0, 80, 80);
	if (mInvisible)
		if (DrawInvisibleEffect(g, IMAGE_STARCATCHER, aSrcRect, false))
			return;
	SetColorHelper(g, Color::White);
	g->DrawImage(IMAGE_STARCATCHER, 0, 0, aSrcRect);
	if (mHungerAnimationTimer != 0)
	{
		SetColorHelper(g, Color(255, 255, 255, mHungerAnimationTimer * 255 / 5));
		aSrcRect.mY = 80;
		g->DrawImage(IMAGE_STARCATCHER, 0, 0, aSrcRect);
	}
	g->SetColorizeImages(false);
	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);
	if (IsHungryBlipPointer(500))
		g->DrawImageCel(IMAGE_MISCITEMS, 0, -10, 2);
}

void Sexy::Penta::Remove(bool removeShadow)
{
	if (mMisslePtr)
		mMisslePtr->Remove();
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (removeShadow && mShadowPtr)
		mShadowPtr->Remove();
	mApp->mBoard->m0x490++;
	if (mApp->mRelaxMode)
	{
		if (ShouldDie())
			mApp->mBoard->ShowText("The purple guys eat coins!", false, 34);
	}
	else
	{
		if (mApp->mBoard->IsTankAndLevelNB(2, 2) && ShouldDie())
		{
			if(mApp->mBoard->m0x4b0[35])
				mApp->mBoard->ShowText("Hint: Star potions allow guppies to produce stars!", false, 36);
			if(mApp->mBoard->m0x4b0[34])
				mApp->mBoard->ShowText("Hint: Starcatchers need stars to stay alive!", false, 35);
			mApp->mBoard->ShowText("Warning! Your starcatcher has died!", false, 34);
		}
	}
}

bool Sexy::Penta::DropCoin()
{
	mCoinDropTimer++;
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK || mCoinDropTimer > 2)
	{
		mCoinDropTimer = 0;
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		{
			if (!CanDropCoin() || !mApp->mBoard->m0x500)
				return false;
		}
		if (RelaxModeCanDrop())
		{
			mApp->mBoard->DropCoin(mX + 5, mY - 12, 5, nullptr, -1.0, 0);
			return true;
		}
	}
	return false;
}

Sexy::GameObject* Sexy::Penta::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(mX + mWidth / 2, mY + mHeight / 2);

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	GameObject* aRet = nullptr;
	for (int i = 0; i < aBoard->mCoinList1->size(); i++)
	{
		Coin* aCoin = aBoard->mCoinList1->at(i);

		if (IsCoinValid(aCoin))
		{
			int ax = (mXD + 40) - (aCoin->mX + 36);
			int ay = (mYD + 40) - (aCoin->mY + 36);
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = aCoin;
				aDist = aNewDist;
			}
		}
	}
	if (aDist < 10000)
	{
		mSpeedySpeedState = 100;
	}
	return aRet;
}

bool Sexy::Penta::IsCoinValid(Coin* aCoin)
{
	if (aCoin->m0x198)
		return false;
	int aType = aCoin->mCoinType;
	if (mApp->mRelaxMode)
	{
		if (aType == 1 || aType == 2)
			return true;
	}
	else
	{
		if (aType == 3 || aType == 10 || aType == 18)
			return true;
	}
	return false;
}

void Sexy::Penta::CollideWithFood()
{
	if (mExoticDietFoodType != 0)
	{
		if (ExoticFoodCollision(mX + mWidth / 2, mY + mHeight / 2) > 0)
			ShowInvisibility();
	}
	else
	{
		Board* aBoard = mApp->mBoard;
		for (int i = 0; i < aBoard->mCoinList1->size(); i++)
		{
			Coin* aCoin = aBoard->mCoinList1->at(i);

			int aFX = aCoin->mX;
			int aFY = aCoin->mY;
			if (IsCoinValid(aCoin))
			{
				if (mXD + 40.0 < aFX + 56 && mXD + 40.0 > aFX + 16 &&
					mYD + 40.0 < aFY + 66 && mYD + 40.0 > aFY + 16)
				{
					ShowInvisibility();
					aCoin->Remove();
					OnFoodAte(aCoin);
					return;
				}
			}
		}
	}
}

void Sexy::Penta::UpdateAnimations()
{
	if (mVX > 1.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer + 1) % 20;
		mPentaAnimIndex = mMovementAnimTimer / 2;
	}
	else if (mVX < -1.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer - 1) % 20;
		if (mMovementAnimTimer < 0)
			mMovementAnimTimer += 20;
		mPentaAnimIndex = mMovementAnimTimer / 2;
	}
	else if (mVX > 0.0)
	{
		mMovementAnimTimer = (mMovementAnimTimer + 1) % 40;
		mPentaAnimIndex = mMovementAnimTimer / 4;
	}
	else
	{
		mMovementAnimTimer = (mMovementAnimTimer - 1) % 40;
		if (mMovementAnimTimer < 0)
			mMovementAnimTimer += 40;
		mPentaAnimIndex = mMovementAnimTimer / 4;
	}
}
