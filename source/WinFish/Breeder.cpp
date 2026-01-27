#include "Breeder.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

#include <time.h>

Sexy::Breeder::Breeder()
{
	mClip = false;
	mType = TYPE_BREEDER;
}

Sexy::Breeder::Breeder(int theX, int theY) 
{
	Breeder::Init(theX, theY);
	mSize = SIZE_SMALL;
	m0x1d4 = mApp->mSeed->Next() % 400 + 1000;
}

Sexy::Breeder::Breeder(int theX, int theY, int theSize, bool velocityRight)
{
	Breeder::Init(theX, theY);
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
	mSize = theSize;
	if (theSize == 0 || theSize == 1)
		m0x1d4 = mApp->mSeed->Next() % 400 + 1000;
	else
		m0x1d4 = mApp->mSeed->Next() % 200 + 800;
}

Sexy::Breeder::~Breeder()
{
}

void Sexy::Breeder::Update()
{
	if (mApp->mBoard == NULL || mApp->mBoard->mPause)
		return;
	Board* aBoard = mApp->mBoard;
	UpdateCounters();

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK || gUnkInt02 == 0 || mSize > 1)
	{
		if (!HungryLogic())
		{
			if (aBoard->AliensInTank() && aBoard->mPetsInTank[PET_GUMBO] != 0)
			{
				FishTypePet* aGumbo = nullptr;
				for (int i = 0; i < aBoard->mFishTypePetList->size(); i++)
				{
					if (aBoard->mFishTypePetList->at(i)->mFishTypePetType == PET_GUMBO)
					{
						aGumbo = aBoard->mFishTypePetList->at(i);
						break;
					}
				}

				if (aGumbo)
				{
					double aCenterX = mXD + 40.0;
					double aGX = aGumbo->mX;
					if (aCenterX > aGX + 50)
					{
						if (mVX > -4.0)
							mVX -= 1.3;
					}
					else if (aCenterX < aGX + 30)
					{
						if (mVX < 4.0)
							mVX += 1.3;
					}
					else if (aCenterX > aGX + 45)
					{
						if (mVX > -4.0)
							mVX -= 0.2;
					}
					else if (aCenterX < aGX + 35)
					{
						if (mVX < 4.0)
							mVX += 0.2;
					}
					else if (aCenterX > aGX + 40)
					{
						if (mVX > -4.0)
							mVX -= 0.05;
					}
					else if (aCenterX < aGX + 40)
					{
						if (mVX < 4.0)
							mVX += 0.05;
					}

					double aCenterY = mYD + 40.0;
					double aGY = aGumbo->mY;
					if (aCenterY > aGY + 25)
					{
						if (mVY > -3.0)
							mVY -= 1.0;
					}
					else if (aCenterY < aGY + 15)
					{
						if (mVY < 4.0)
							mVY += 1.3;
					}
					if (aCenterY > aGY + 20)
					{
						if (mVY > -3.0)
							mVY -= 0.5;
					}
					else if (aCenterY < aGY + 20)
					{
						if (mVY < 4.0)
							mVY += 0.7;
					}

					if (mY <= mYMin && mVY < 0.0)
						mVY = 0.0;
					if (mVXAbs < 5)
						mVXAbs++;
				}
			}
			else
			{
				if (mMovementState == 0)
				{
					if (mBoughtTimer == 0)
						mVY = 0.5;
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;
						if (mVX < -0.5)
							mVX += 0.5;
						else if (mVX > 0.5)
							mVX -= 0.5;

						mVXAbs = (int)abs(mVX);
					}

					mYD -= 0.25 / mSpeedMod;
				}
				else if (mMovementState == 1)
				{
					if (mBoughtTimer == 0)
						mVY = -0.5;
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;
						if (mVX < 1.0)
							mVX++;
						else if (mVX > 1.0)
							mVX--;
						mVXAbs = (int)abs(mVX);
					}

					mYD -= 0.5 / mSpeedMod;
				}
				else if (mMovementState == 2)
				{
					if (mBoughtTimer == 0)
						mVY = -0.5;
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;
						if (mVX < -1.0)
							mVX++;
						else if (mVX > -1.0)
							mVX--;

						mVXAbs = (int)abs(mVX);
					}

					mYD -= 0.5 / mSpeedMod;
				}
				else if (mMovementState == 3)
				{
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;
						if (mVX < -1.0)
							mVX++;
						else if (mVX > -1.0)
							mVX--;

						if (mVY < 3.0)
							mVY++;
						else if (mVY > 3.0)
							mVY--;

						if (mVXAbs < 5)
						{
							if (mVY >= 4.0)
							{
								if (mYD > 240.0)
									mMovementState = 0;
							}
							else
							{
								mVXAbs++;
							}
						}
						else
						{
							mVXAbs--;
						}
					}
					if (mYD > 240.0)
						mMovementState = 0;
				}
				else if (mMovementState == 4)
				{
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;

						if (mVX < 1.0)
							mVX++;
						else if (mVX > 1.0)
							mVX--;

						if (mVY < 3.0)
							mVY++;
						else if (mVY > 3.0)
							mVY--;

						if (mVXAbs < 5)
						{
							if (mVY >= 4.0)
							{
								if (mYD > 240.0)
									mMovementState = 0;
							}
							else
							{
								mVXAbs++;
							}
						}
						else
						{
							mVXAbs--;
						}
					}
					if (mYD > 240.0)
						mMovementState = 0;
				}
				else if (mMovementState > 4)
				{
					if (mBoughtTimer == 0)
					{
						if (mYD >= 115.0)
							mVY = -0.5;
						else
							mVY = -0.1;
					}
					if (mSpecialMovementStateChangeTimer > 39)
					{
						mSpecialMovementStateChangeTimer = 0;
						if (mXDirection == 1)
						{
							if (mVX < 0.0)
								mVX += 2.0;
							else
								mVX += 1.0;

							mVXAbs = (int)abs(mVX);
							if (mXD > 250.0)
							{
								mXDirection = -1;
								mVX -= 2.0;
							}
						}
						else if (mXDirection == -1)
						{
							if (mVX > 0.0)
								mVX -= 2.0;
							else
								mVX -= 1.0;

							mVXAbs = (int)abs(mVX);
							if (mXD < 175.0)
							{
								mXDirection = 1;
								mVX += 2.0;
							}
						}
					}
				}
			}
		}
	}
	else 
	{
		if (mVX < 0.0)
			mVX = -0.5;
		else
			mVX = 0.5;

		if (mBoughtTimer == 0)
			mVY = 0.0;

		if (mXD > gUnkInt05 + 50)
			mXD -= 5.0;
		else if(mXD < gUnkInt05 - 50)
			mXD += 5.0;
		else if(mXD > gUnkInt05 + 5)
			mXD -= 3.0;
		else if(mXD < gUnkInt05 - 5)
			mXD += 3.0;

		if (mYD > gUnkInt06 + 50)
			mYD -= 4.0;
		else if (mYD < gUnkInt06 - 50)
			mYD += 4.0;
		else if (mYD > gUnkInt06 + 5)
			mYD -= 3.0;
		else if (mYD < gUnkInt06 - 5)
			mYD += 3.0;
	}
	mSpecialMovementStateChangeTimer++;
	m0x1b8++;
	if (m0x1b8 > 20)
	{
		m0x1b8 = 0;
		if (mApp->mSeed->Next() % 10 == 0 || mMovementState == -1)
			mMovementState = mApp->mSeed->Next() % 9 + 1;
	}

	if (aBoard->mAlienList->empty() && aBoard->mBilaterusList->empty())
		DropCoin();

	if (mBoughtTimer != 0)
	{
		mBoughtTimer--;
		mVY *= 0.9;
	}
	if (mVX == 0.0)
		mYD += 1.0 / mSpeedMod;
	if (mVX == 1.0)
		mYD += 0.75 / mSpeedMod;
	if (mVX == 2.0)
		mYD += 0.5 / mSpeedMod;
	if (mVX == 3.0)
		mYD += 0.25 / mSpeedMod;

	if (mYMax < 321)
		mYD -= 0.25;
	if (mXD > (double)mXMax)
		mXD = (double)mXMax;
	if (mXD < (double)mXMin)
		mXD = (double)mXMin;
	if (mYD > (double)mYMax)
		mYD = (double)mYMax;

	if (mBoughtTimer < 1 || mVY <= 0.0)
	{
		if (mYD < (double)mYMin)
			mYD = (double)mYMin;
	}
	else if (mBoughtTimer > 30)
	{
		int aChance = 2;
		int aSpawnX;
		int aSpawnY;
		if (mBoughtTimer > 40)
			aChance = 1;

		if (mApp->mSeed->Next() % aChance == 0)
		{
			aSpawnX = mX + (40 - mApp->mSeed->Next() % 30);
			aSpawnY = mY + (40 - mApp->mSeed->Next() % 30);
			aBoard->SpawnBubble(aSpawnX, aSpawnY);
		}
	}

	if (mYD <= mYMin && mMovementState == -1)
		mMovementState = mApp->mSeed->Next() % 9 + 1;

	if (mXD > mXMax - 5 && mVX > 0.1)
		mVX -= 0.1;
	if (mXD < 15 && mVX < -0.1)
		mVX += 0.1;

	UpdateAnimations();
	
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

void Sexy::Breeder::Draw(Graphics* g)
{
	UpdateFishSongMgr();

	if (gUnkInt02 != 0 && mXD < gUnkInt05 + 8 && mXD > gUnkInt05 - 8 &&
		mYD < gUnkInt06 + 8 && mYD > gUnkInt06 - 8 && mSize < SIZE_LARGE)
	{
		return;
	}
	if (mShadowPtr)
		mShadowPtr->m0x168 = 1.0;

	bool shouldFlip;
	if (mTurnAnimationTimer > 0)
		shouldFlip = (!mForwardlyChallenged);
	else if (mTurnAnimationTimer < 0)
		shouldFlip = (mForwardlyChallenged);
	else
	{
		bool facingLeft = false;

		if (mVX < 0.0)
			facingLeft = true;
		else if ((int)mVX == 0)
		{
			if (mPrevVX < 0.0)
				facingLeft = true;
			else
				facingLeft = false;
		}
		else
			facingLeft = false;

		if (facingLeft)
			shouldFlip = (mForwardlyChallenged);
		else
			shouldFlip = (!mForwardlyChallenged);
	}

	DrawBreeder(g, shouldFlip);

	if (mName.size() > 0)
		DrawName(g, false);
}

void Sexy::Breeder::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount < 0)
	{
		mApp->mBoard->CheckMouseDown(mX + x, mY + y);
		return;
	}
	Board* aBoard = mApp->mBoard;

	if (aBoard->mAlienList->size() == 0 && aBoard->mBilaterusList->size() == 0 &&
		aBoard->mMissleList1->size() == 0)
	{
		double absX = x + mXD;
		double absY = y + mYD;
		if (absX < 587.0 && absX > 30.0 && absY < 400.0 && absY > 60.0)
		{
			bool unk = aBoard->Unk11(mX + x, mY + y);
			if (!unk && aBoard->Buy(aBoard->m0x4ac, true))
			{
				int theXVal = 0;
				int theYVal = 0;
				int unk1 = 0;
				theXVal = x + mXD;
				theYVal = y + mYD - 10;
				if (x >= 10 && x <= 70 && y >= 10 && y <= 70)
					unk1 = (70 - y) / 2;
				else
					unk1 = 0;

				aBoard->DropFood(theXVal, theYVal, 0, false, unk1, -1);
				aBoard->m0x4ec = true;
				aBoard->m0x3c0 = aBoard->Unk01();
			}
		}
	}
}

void Sexy::Breeder::MouseUp(int x, int y, int theClickCount)
{
	GameObject::MouseUp(x, y, theClickCount);
	mApp->mBoard->m0x4ec = false;
	mApp->mBoard->m0x4ed = false;
}

int Sexy::Breeder::GetShellPrice()
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

void Sexy::Breeder::Remove()
{
	RemoveFromGame(true);
}

void Sexy::Breeder::SetPosition(double newX, double newY)
{
	mX = newX;
	mXD = newX;
	mY = newY;
	mYD = newY;
}

void Sexy::Breeder::OnFoodAte(Food* theFood)
{
	bool hungry = IsHungryVisible();
	Unk02(false);
	bool flag = true;
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		if (mVirtualTankId < 0)
			flag = false;
		else
			flag = UpdateMentalState();
	}

	mApp->mBoard->PlaySlurpSound(mVoracious);
	if (theFood->mFoodType == 0)
	{
		mHunger += 500;
		if (mHunger > 800)
			mHunger = 800;
		if (flag)
			m0x1a4++;
	}
	else if (theFood->mFoodType == 1)
	{
		mHunger += 700;
		if (mHunger > 1000)
			mHunger = 1000;
		if (flag)
			m0x1a4 += (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK ? 2 : 1);
	}
	else
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		{
			mHunger += 500;
			if (mHunger > 800)
				mHunger = 800;
		}
		else
		{
			mHunger += 1100;
			if (mHunger > 1400)
				mHunger = 1400;
		}
		if (flag)
			m0x1a4 += (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK ? 3 : 2);
	}

	if (m0x1a8 <= m0x1a4 && mSize < SIZE_LARGE)
	{
		mSize++;
		if (mSize == SIZE_MEDIUM)
		{
			m0x1d0 = 900;
			if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				m0x1a8 = mApp->mSeed->Next() % 5 + 5;
			mApp->mBoard->MakeAndUnlockMenuButton(Board::SLOT_BREEDER, true);
		}
		if (mSize == SIZE_LARGE)
		{
			if (mVirtualTankId > -1)
				m0x1d0 = 0;
			m0x1d4 = mApp->mSeed->Next() % 200 + 500;
			if(mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
				mApp->mBoard->MakeAndUnlockMenuButton(Board::SLOT_EGG, true);
		}
		m0x1a4 = 0;
		mGrowthAnimationTimer = 10;
		mApp->PlaySample(SOUND_GROW);
	}
	UpdateHungerStateIfWasHungry(hungry);
}

void Sexy::Breeder::UpdateStoreAnimation()
{
	mStoreAnimationTimer++;
	if (mInvisible)
		m0x128++;
	mStoreAnimationIndex = (mStoreAnimationTimer / 2) % 10;
}

void Sexy::Breeder::DrawStoreAnimation(Graphics* g, int justification)
{
	int anX = 0, anY = 0;
	switch (justification)
	{
	case 0:
		anX = 18;
		anY = 20;
		break;
	case 1:
		anX = 7;
		anY = 20;
		break;
	case 2:
		anX = 15;
		anY = 0;
		break;
	case 3:
		anX = 12;
		anY = 0;
		break;
	case 4:
		anX = -40;
		anY = -5;
		break;
	}
	g->Translate(anX, anY);
	Color aCol = Color::White;
	SetStoreColor(g, aCol);
	g->DrawImage(IMAGE_BREEDER, 0, 0, Rect(mStoreAnimationIndex * 80, mSize * 240, 80, 80));
	g->SetColorizeImages(false);
	g->Translate(-anX, -anY);
}

void Sexy::Breeder::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncLong(mXDirection);
	theSync->SyncLong(m0x17c);
	theSync->SyncDouble(mSpeedMod);
	theSync->SyncDouble(mPrevVX);
	theSync->SyncLong(mYMax);
	theSync->SyncLong(mXMin);
	theSync->SyncLong(mYMin);
	theSync->SyncLong(mXMax);
	theSync->SyncLong(mSize);
	theSync->SyncLong(m0x1a4);
	theSync->SyncLong(m0x1a8);
	theSync->SyncLong(mGrowthAnimationTimer);
	theSync->SyncLong(mMovementState);
	theSync->SyncLong(mSpecialMovementStateChangeTimer);
	theSync->SyncLong(m0x1b8);
	theSync->SyncLong(mSwimAnimationTimer);
	theSync->SyncLong(mVXAbs);
	theSync->SyncLong(mAnimationIndex);
	theSync->SyncLong(mTurnAnimationTimer);
	theSync->SyncLong(mEatingAnimationTimer);
	theSync->SyncLong(m0x1d0);
	theSync->SyncLong(m0x1d4);
	theSync->SyncLong(mBoughtTimer);
	if (theSync->mVersion >= 54)
		theSync->SyncLong(m0x1dc);
	else
		m0x1dc = 0;
}

void Sexy::Breeder::Init(int theX, int theY)
{
	mXD = theX;
	mYD = theY;
	mClip = false;
	mType = TYPE_BREEDER;
	m0x1dc = 0;
	mX = mXD;
	mY = mYD;
	mVX = 0;
	mVY = -0.5;
	mWidth = 80;
	mHeight = 80;
	mPrevVX = 1.0;
	if (mApp->mSeed->Next() % 2 == 0)
	{
		mVX = -0.1;
		mPrevVX = -1.0;
	}
	mXDirection = 1;
	m0x17c = 0;
	mYMax = 370;
	mYMin = 95;
	mXMin = 10;
	mXMax = 540;
	int aRand = mApp->mSeed->Next();
	if (aRand % 3 == 0)
		mSpeedMod = 2.0;
	else if (aRand % 3 == 1)
		mSpeedMod = 1.8;
	else
		mSpeedMod = 1.6;
	m0x1a4 = 0;
	mHunger = mApp->mSeed->Next() % 200 + 400;
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		m0x1a8 = Rand() % 9 + 12;
	else
		m0x1a8 = mApp->mSeed->Next() % 2 + 4;
	mSpecialMovementStateChangeTimer = 40;
	m0x1b8 = 0;
	mSwimAnimationTimer = 0;
	mVXAbs = 0;
	mAnimationIndex = 0;
	mTurnAnimationTimer = 0;
	mEatingAnimationTimer = 0;
	m0x1d0 = 0;
	mMouseVisible = true;
	mMovementState = mApp->mSeed->Next() % 10;
	if (mApp->mBoard != nullptr)
	{
		if(!mApp->mBoard->mAlienList->empty() || !mApp->mBoard->mBilaterusList->empty())
			mMouseVisible = false;
	}
	mGrowthAnimationTimer = 0;
	mBoughtTimer = 0;
}

bool Sexy::Breeder::HungryLogic()
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
			if(!mApp->mBoard->AliensInTank())
				return HungryBehavior();
			if(!mApp->mBoard->mAlienList->empty() && mApp->mBoard->mAlienList->at(0)->mAlienType != Alien::ALIEN_GUS)
				return HungryBehavior();
		}
	}
	return false;
}

void Sexy::Breeder::Die(bool playDieSound)
{
	if (playDieSound)
		mApp->mBoard->PlayDieSound(mType);

	RemoveFromGame(false);
	bool isFacingRight = (mVX >= 0.0);
	mApp->mBoard->SpawnDeadFish(mXD, mYD, mVX, mVY, mSpeedMod, mSize + TYPE_BREEDER, isFacingRight, mShadowPtr);
}

bool Sexy::Breeder::HungryBehavior()
{
	GameObject* aNearestFood = FindNearestFood();

	int aCenterX = mXD + 40.0;
	int aCenterY = mYD + 45.0;

	if (mSpecialMovementStateChangeTimer > 2)
	{
		if (!aNearestFood)
			return false;
		mSpecialMovementStateChangeTimer = 0;
		int aFCX = aNearestFood->mX;
		int aFCY = aNearestFood->mY;
		if (mHunger < 301)
		{
			if (aCenterX > aFCX + 28) {
				if (mVX > -4.0) mVX -= 1.3;
			}
			else if (aCenterX < aFCX + 12) {
				if (mVX < 4.0) mVX += 1.3;
			}
			else if (aCenterX > aFCX + 24) {
				if (mVX > -4.0) mVX -= 0.2;
			}
			else if (aCenterX < aFCX + 16) {
				if (mVX < 4.0) mVX += 0.2;
			}
			else if (aCenterX > aFCX + 20) {
				if (mVX > -4.0) mVX -= 0.05;
			}
			else if (aCenterX < aFCX + 20) {
				if (mVX < 4.0) mVX += 0.05;
			}

			if (aCenterY > aFCY + 26) {
				if (mVY > -3.0) mVY -= 1.0;
			}
			else if (aCenterY < aFCY + 14) {
				if (mVY < 4.0) mVY += 1.3;
			}
			else if (aCenterY > aFCY + 20) {
				if (mVY > -3.0) mVY -= 0.5;
			}
			else if (aCenterY < aFCY + 20) {
				if (mVY < 4.0) mVY += 0.7;
			}
		}
		else
		{
			if (aCenterX > aFCX + 28) {
				if (mVX > -3.0) mVX -= 1.0;
			}
			else if (aCenterX < aFCX + 12) {
				if (mVX < 3.0) mVX += 1.0;
			}
			else if (aCenterX > aFCX + 24) {
				if (mVX > -3.0) mVX -= 0.1;
			}
			else if (aCenterX < aFCX + 16) {
				if (mVX < 3.0) mVX += 0.1;
			}
			else if (aCenterX > aFCX + 20) {
				if (mVX > -3.0) mVX -= 0.05;
			}
			else if (aCenterX < aFCX + 20) {
				if (mVX < 3.0) mVX += 0.05;
			}

			if (aCenterY > aFCY + 26) {
				if (mVY > -2.0) mVY -= 0.6;
			}
			else if (aCenterY < aFCY + 14) {
				if (mVY < 3.0) mVY += 1.0;
			}
			else if (aCenterY > aFCY + 20) {
				if (mVY > -2.0) mVY -= 0.3;
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

void Sexy::Breeder::DropCoin()
{
	if (mSize > SIZE_SMALL)
	{
		m0x1d0++;
		if (mInvisible && m0x1d0 == m0x1d4 - 30 && mSize == SIZE_LARGE)
			ShowInvisibility();

		if (m0x1d4 <= m0x1d0)
		{
			m0x1d0 = 0;
			if (RelaxModeCanDrop())
			{
				if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
				{
					if (GiveBirth())
						return;
				}
				if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
				{
					m0x1d4 = 2160;
					if (mSize != SIZE_LARGE)
						return;

					int aReqs[8] = { 0 };
					int aInTank[8] = { 0 };
					mApp->mBoard->GetExoticFoodsRequiredInTank(aReqs);
					mApp->mBoard->GetExoticFoodsInTank(aInTank);
					if (aReqs[0] - aInTank[0] == -1 || (aReqs[0] - aInTank[0]) + 1 < 0)
						return;
				}
				Fish* aGuppy = mApp->mBoard->SpawnGuppy(mX + 5, mY + 10);

				if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
					aGuppy->mCanBeEatenDelay = mExoticDietFoodType != 0 ? 40 : 150;
				mApp->mBoard->PlayBirthSound(true);
			}
		}
	}
}

void Sexy::Breeder::UpdateAnimations()
{
	if (mInvisible)
		UpdateInvisible();
	
	if (mPrevVX < 0 && mVX > 0)
		mTurnAnimationTimer = -20;
	else if (mPrevVX > 0 && mVX < 0)
		mTurnAnimationTimer = 20;

	if (mTurnAnimationTimer > 0)
		mTurnAnimationTimer--;
	else if (mTurnAnimationTimer < 0)
		mTurnAnimationTimer++;

	if (mEatingAnimationTimer != 0)
		mEatingAnimationTimer--;

	if (mTurnAnimationTimer == 0)
	{
		if (mEatingAnimationTimer > 0)
			mAnimationIndex = 9 - (mEatingAnimationTimer / 2);
		else
		{
			if (mVXAbs < 2)
				mSwimAnimationTimer += 1;
			else
				mSwimAnimationTimer += 2;

			if (mSwimAnimationTimer >= 20)
				mSwimAnimationTimer = 0;
			mAnimationIndex = mSwimAnimationTimer / 2;
		}
	}
	else
	{
		if (mTurnAnimationTimer > 0)
			mAnimationIndex = 9 - (mTurnAnimationTimer / 2);
		else
			mAnimationIndex = 9 + (mTurnAnimationTimer / 2);
	}

	if (mPrevVX != mVX && mPrevVX != 0 && mVX != 0)
		mPrevVX = mVX;

	if (mMisslePtr)
		UpdateCrosshairAnimation();

	if (mVoraciousScreamCounter > 100 && mTurnAnimationTimer == 0)
		mAnimationIndex = 4;

	if (mGrowthAnimationTimer > 0)
		mGrowthAnimationTimer--;
}

Sexy::GameObject* Sexy::Breeder::FindNearestFood()
{
	if (mExoticDietFoodType != 0)
		return FindNearestExoticFood(mX + mWidth / 2, mY + mHeight / 2);

	int aCenterX = mXD + 40.0;
	int aCenterY = mYD + 40.0;

	Board* aBoard = mApp->mBoard;
	int aDist = 100000000;
	Food* aRet = nullptr;
	for (int i = 0; i < aBoard->mFoodList->size(); i++)
	{
		Food* aFood = aBoard->mFoodList->at(i);

		if (aFood->mExoticFoodType != 2 && aFood->mCantEatTimer == 0)
		{
			int ax = aCenterX - (aFood->mX + 20);
			int ay = aCenterY - (aFood->mY + 20);
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

void Sexy::Breeder::CollideWithFood()
{
	Board* aBoard = mApp->mBoard;
	int aCenterX = mXD + 40.0;
	int aCenterY = mYD + 35.0;
	if (mExoticDietFoodType == 0)
	{
		for (int i = 0; i < aBoard->mFoodList->size(); i++)
		{
			Food* aFood = aBoard->mFoodList->at(i);

			if (aCenterX > aFood->mX && aCenterX < aFood->mX + 40 &&
				aCenterY > aFood->mY && aCenterY < aFood->mY + 35)
			{
				if (aFood->mExoticFoodType != 2 && aFood->mCantEatTimer == 0)
				{
					OnFoodAte(aFood);
					aFood->Remove();
					if (mEatingAnimationTimer == 0)
					{
						mEatingAnimationTimer = 8;
						ShowInvisibility();
					}
				}
			}

			if (mEatingAnimationTimer == 0) 
			{
				if (aCenterX > aFood->mX - 15 && aCenterX < aFood->mX + 55 &&
					aCenterY > aFood->mY - 5 && aCenterY < aFood->mY + 40)
				{
					if (aFood->mCantEatTimer == 0) 
					{
						mEatingAnimationTimer = 20;
						ShowInvisibility();
					}
				}
			}
		}
	}
	else
	{
		int aExoticFoodVal = ExoticFoodCollision(mX + mWidth / 2, mY + mHeight / 2);
		if (mEatingAnimationTimer == 0)
		{
			if (aExoticFoodVal == 1)
			{
				mEatingAnimationTimer = 8;
				ShowInvisibility();
			}
			else if (aExoticFoodVal == 2)
			{
				mEatingAnimationTimer = 20;
				ShowInvisibility();
			}
		}
	}
}

bool Sexy::Breeder::GiveBirth()
{
	int aNextVirtId = 0;
	bool aSuccess = false;
	if (mVirtualTankId > -1 && mSize == SIZE_LARGE)
	{
		int anId = mApp->mBoard->GetNextVirtualTankId();
		if (anId > -1 && m0x1dc < 1)
		{
			aNextVirtId = anId;
			aSuccess = true;
		}
	}
	if (!aSuccess)
		return false;

	Fish* aGuppy = new Fish(mX + 5, mY + 10);
	mApp->mBoard->PlayBirthSound(false);
	aGuppy->SomeBoughtFunc();
	aGuppy->CopyBreederDataVT(this);
	aGuppy->mVirtualTankId = aNextVirtId;
	aGuppy->mTimeBought = time(NULL);
	aGuppy->mName += " JR.";
	aGuppy->mHometownIdx = 0;
	aGuppy->mVirtualFish = true;
	if (aGuppy->mInvisible)
		aGuppy->ShowInvisibility();
	mApp->mBoard->SpawnGameObject(aGuppy, false);
	m0x1dc++;
	return true;
}

void Sexy::Breeder::RemoveFromGame(bool removeShadow)
{
	if (mVirtualTankId > -1)
	{
		GameObject* aGameObject = mApp->mBoard->GetGameObjectByVirtualId(mVirtualTankId + 100);
		if (aGameObject)
			aGameObject->mVirtualTankId = mVirtualTankId;
	}
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (removeShadow && mShadowPtr)
		mShadowPtr->Remove();
}

void Sexy::Breeder::DrawBreeder(Graphics* g, bool mirror)
{
	if (gZombieMode)
	{
		bool hungry = IsHungryVisible();
		g->DrawImageMirror(IMAGE_HUNGRYBREEDER, 0, 0, Rect(hungry ? 480 : 720, (mSize*3+2) * 80, 80, 80), mirror);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);
		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
	}
	else
	{
		bool hungry = IsHungryVisible();
		int aRow = GetRowToDraw(hungry);
		Rect aSrcRect(mAnimationIndex * 80, aRow * 80, 80, 80);
		if (mInvisible && mGrowthAnimationTimer == 0)
			if (DrawInvisibleEffect(g, hungry ? IMAGE_HUNGRYBREEDER : IMAGE_BREEDER, aSrcRect, mirror))
				return;

		Rect aDestRect = Rect(0, 0, 80, 80);
		if (mGrowthAnimationTimer > 0 && mSize != SIZE_STAR)
		{
			double aGrowthVal = 0;
			if(mGrowthAnimationTimer > 3)
				aGrowthVal = ((10 - mGrowthAnimationTimer) * 0.5) / 7.0 + 0.7;
			else
				aGrowthVal = (mGrowthAnimationTimer * 0.2) / 3.0 + 1.0;

			int aVal = (aGrowthVal - 1.0) * 80.0; // 80.0 * 0.5

			aDestRect.mX -= aVal;
			aDestRect.mY -= aVal;
			aDestRect.mWidth += aVal * 2;
			aDestRect.mHeight += aVal * 2;

			g->SetFastStretch(!mApp->Is3DAccelerated());
		}

		SetColorHelper(g, Color::White);

		g->DrawImageMirror(hungry ? IMAGE_HUNGRYBREEDER : IMAGE_BREEDER, aDestRect, aSrcRect, mirror);
		if (mHungerAnimationTimer != 0)
		{
			aSrcRect.mY = GetRowToDraw(true) * 80;
			SetColorHelper(g, Color(255, 255, 255, mHungerAnimationTimer * 255 / 5));
			g->DrawImageMirror(IMAGE_HUNGRYBREEDER, aDestRect, aSrcRect, mirror);
		}
		g->SetColorizeImages(false);
		if (mMisslePtr)
			DrawCrosshair(g, 0, 0);

		if (IsHungryBlipPointer(500))
			g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 2);
	}
}

int Sexy::Breeder::GetRowToDraw(bool hungry)
{
	if (mTurnAnimationTimer != 0)
		return mSize * 3 + 1;
	if (mEatingAnimationTimer < 1 && mVoraciousScreamCounter < 101)
		return mSize * 3;
	if (hungry)
		return mSize + 9;
	return mSize * 3 + 2;
}
