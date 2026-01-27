#include "FishTypePet.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "BilaterusHead.h"
#include "SexyAppFramework/WidgetManager.h"

const int gConst01 = 360;

Sexy::FishTypePet::FishTypePet()
{
	mClip = false;
	m0x245 = false;
	m0x24c = nullptr;
	mType = TYPE_FISH_TYPE_PET;
}

Sexy::FishTypePet::FishTypePet(int theX, int theY, int thePetType, bool flag)
	: Fish(theX, theY)
{
	mClip = false;
	m0x245 = false;
	mFishTypePetType = thePetType;
	mType = TYPE_FISH_TYPE_PET;
	m0x230 = flag;
	m0x248 = 0;
	if (!flag || thePetType == PET_PRESTO)
		m0x238 = 0;
	else
		m0x238 =  360;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		m0x238 = 0;

	m0x23c = 0;
	m0x258 = 0;
	m0x240 = 0;
	m0x264 = 0;
	mIsGuppy = false;
	m0x250 = false;
	mEatingAnimationTimer = 0;
	m0x260 = 0;
	m0x24c = nullptr;
	m0x244 = true;
	mMouseVisible = gUnkBool06;

	if (thePetType == PET_WALTER)
	{
		mMouseVisible = true;
		mDoFinger = true;
		mMouseInsets.mTop = 10;
		mMouseInsets.mBottom = 15;
	}
	else if (thePetType == PET_PREGO)
	{
		mCoinDropT = 930;
		mYMax = 360;
	}
	else if (thePetType == PET_ZORF)
	{
		mCoinDropT = 65;
		mSpeedMod = 3.0;
		mYMax = 270;
	}
	else if (thePetType == PET_MERYL)
	{
		mCoinDropT = 4320 - (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK ? 0 : 2920);
	}
	else if (thePetType == PET_WADSWORTH)
	{
		mCoinDropT = 120;
		mSpeedMod = 4.0;
	}
	else if (thePetType == PET_VERT)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			mCoinDropT = Rand() % 200 + 1080;
		else
			mCoinDropT = 216;
	}
	else if(thePetType == PET_STANLEY)
		mCoinDropT = 100;
	else if (thePetType == PET_NOSTRADAMUS)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			mCoinDropT = Rand() % 200 + 1080;
		else
			mCoinDropT = mApp->mSeed->Next() % 300 + 300;
	}
	else if (thePetType == PET_SHRAPNEL)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			mCoinDropT = Rand() % 200 + 1080;
		else
			mCoinDropT = mApp->mSeed->Next() % 20 + 633;
	}
	else if (thePetType == PET_NIMBUS)
	{
		mSpeedMod = 0.5;
		mYMin = 320;
	}
	else if (thePetType == PET_AMP)
	{
		mSpeedMod = 0.5;
		mCoinDropT = 3000;
		mWidth = 160;
		mHeight = 80;
		mXMax = 460;
		mCoinDropTimer = 300;
		mMouseInsets.mTop = 0;
		mMouseInsets.mBottom = 25;
	}
	else if (thePetType == PET_GASH)
	{
		mCoinDropT = 1570;
		mCoinDropTimer = mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK ? 0 : -1550;
		if (m0x230)
			mCoinDropTimer = 1520;
	}
}

Sexy::FishTypePet::~FishTypePet()
{
	WadsworthAndMerylFunc01();
	if (m0x24c)
		delete m0x24c;
}

void Sexy::FishTypePet::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	GameObject::UpdateCounters();
	Board* aBoard = mApp->mBoard;

	if (mFishTypePetType == PET_BLIP && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK
		&& aBoard->m0x2e0 < aBoard->m0x2dc && aBoard->mTank != 5 && aBoard->mGameUpdateCnt > 200
		&& (!m0x230 || mUpdateCnt > 720))
	{
		aBoard->PlaySample(SOUND_SONAR_ID, 3, 1.0);
		for (int i = 0; i < Board::SLOT_END; i++)
			aBoard->MakeAndUnlockMenuButton(i, true);
	}

	if (m0x24c != nullptr)
	{
		m0x24c->Update();
		if (m0x24c->m0x15c < 1)
		{
			if(m0x24c)
				delete m0x24c;
			m0x24c = nullptr;
		}
	}

	if (m0x264 != 0)
	{
		m0x264--;
		if (mFishTypePetType == PET_WALTER && m0x264 == 0)
			mDoFinger = true;
	}

	if (m0x23c != 0)
		m0x23c--;

	if (aBoard->mTank == 5 || !Hungry())
	{ // 72
		double aVX = 0.5;
		double aVY = 1.0;

		if (mMovementState == 0)
		{
			if (m0x264 < 1 || mFishTypePetType != PET_WALTER)
				mVY = 0.5;
			else
				mVY = 1.0;

			if(mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;
				if (mVX < 0.0 && mVX < -0.5)
					mVX += 0.5;
				else if(mVX > 0.5)
					mVX -= 0.5;
				mVXAbs = (int)abs(mVX);
			}
			mYD -= 0.25 / mSpeedMod;
		}
		else if (mMovementState == 1)
		{
			mVY = -0.5;
			if (mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;

				if (mVX < 1.0)
					mVX += 1.0;
				else if (mVX > 1.0)
					mVX -= 1.0;
				mVXAbs = (int)abs(mVX);
			}
			mYD -= 0.5 / mSpeedMod;
		}
		else if (mMovementState == 2)
		{
			mVY = -0.5;
			if (mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;
				if (mVX < -1.0)
					mVX += 1.0;
				else if (mVX > -1.0)
					mVX -= 1.0;
				mVXAbs = (int)abs(mVX);
			}
			mYD -= 0.5 / mSpeedMod;
		}
		else if (mMovementState == 3)
		{
			if (mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;
				if (mVX < -1.0)
					mVX += 1.0;
				else if (mVX > -1.0)
					mVX -= 1.0;

				if (mVY < 3.0)
					mVY += 1.0;
				else if (mVY > 3.0)
					mVY -= 1.0;

				if (mVXAbs < 5)
				{
					if (mVY < 4.0)
						mVXAbs++;
				}
				else
					mVXAbs--;
			}

			if (mYD > 240.0)
				mMovementState = 0;
		}
		else if (mMovementState == 4)
		{
			if (mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;

				if (mVX < 1.0)
					mVX += 1.0;
				else if (mVX > 1.0)
					mVX -= 1.0;

				if (mVY < 3.0)
					mVY += 1.0;
				else if (mVY > 3.0)
					mVY -= 1.0;

				if (mVXAbs < 5)
				{
					if (mVY < 4.0)
						mVXAbs++;
				}
				else
					mVXAbs--;
			}

			if (mYD > 240.0)
				mMovementState = 0;
		}
		else if (mMovementState < 0)
		{
			if (mMovementState == -1)
				mVY = -10;
		}
		else if (mMovementState >= 5)
		{
			if (mYD < 115)
				mVY = -0.1;
			else
				mVY = -0.5;

			if (mSpecialMovementStateChangeTimer >= 40)
			{
				mSpecialMovementStateChangeTimer = 0;

				if (mXDirection == 1)
				{
					if (mVX < 0.0)
						mVX += 2.0;
					else if (mVX >= 0.0)
						mVX += 1.0;

					mVXAbs = (int)abs(mVX);
					if (mXD > 250.0)
					{
						mXDirection = -1;
						mVX -= mVXAbs; // Maybe Incorrect TODO
					}
				}
				else if (mXDirection == -1)
				{
					if (mVX > 0.0)
						mVX -= 2.0;
					else if (mVX <= 0.0)
						mVX -= 1.0;

					mVXAbs = (int)abs(mVX);
					if (mXD < 175.0)
					{
						mXDirection = 1;
						mVX += mVXAbs; // Maybe Incorrect TODO
					}
				}
			}
		}
	}

	mSpecialMovementStateChangeTimer++;
	mMovementStateChangeTimer++;

	if (mMovementStateChangeTimer > 20 || (mFishTypePetType == PET_NOSTRADAMUS && mYMax - 50 < mYD))
	{
		mMovementStateChangeTimer = 0;
		if (mApp->mSeed->Next() % 10 == 0 && (m0x264 == 0 || mFishTypePetType != PET_WALTER))
			mMovementState = mApp->mSeed->Next() % 9 + 1;
	}

	if ((!aBoard->AliensInTank() && aBoard->mBilaterusList->empty()) || 
		(mFishTypePetType == PET_STANLEY && (aBoard->AliensInTank() || !aBoard->mBilaterusList->empty())))
		DropCoin();

	if (mWadsworthVXModCounter != 0)
	{
		mWadsworthVXModCounter--;
		mUnusedVXWadsworthAddon *= 0.9;
		mXD += mUnusedVXWadsworthAddon;

		if (m0x230 && mWadsworthVXModCounter <= 40 && mFishTypePetType != PET_PRESTO)
		{ // 280
			m0x238 = 0;
			PrestoMorph(PET_PRESTO);
			return;
		}
	}

	if (mUnusedTimer > 0)
		mUnusedTimer--;

	if (mFishTypePetType != PET_AMP)
	{
		if (mFishTypePetType == 0.0)
			mYD += 1.0 / mSpeedMod;
		if (mFishTypePetType == 1.0)
			mYD += 0.75 / mSpeedMod;
		if (mFishTypePetType == 2.0)
			mYD += 0.5 / mSpeedMod;
		if (mFishTypePetType == 3.0)
			mYD += 0.25 / mSpeedMod;
	}
	else
	{
		if (mVY < -0.5)
			mVY = -0.5;
		else if (mVY > 0.5)
			mVY = 0.5;
	}

	if (m0x238 != 0)
		m0x238--;

	if ((m0x230 || mFishTypePetType == PET_NOSTRADAMUS) && m0x248 < 20)
		m0x248++;

	if (mFishTypePetType == PET_BRINKLEY)
	{
		if (abs(mVX) <= 0)
			mYD++;
		else if (abs(mVX) <= 1)
			mYD += 0.75;
		else if (abs(mVX) <= 3)
			mYD += 0.5;
	}

	if (mXD > mXMax)
		mXD = mXMax;
	if (mXD < mXMin)
		mXD = mXMin;

	if(mYD > mYMax && m0x230 && mFishTypePetType == PET_ZORF)
		mMovementState = 2;
	else if (mYD > mYMax)
			mYD = mYMax;

	if (mYD < mYMin && m0x230 && mFishTypePetType == PET_NIMBUS)
	{
		mMovementState = 0;
		mVY += 0.25;
	}
	else if (mYD < mYMin)
			mYD = mYMin;

	if (mYD <= mYMin && mMovementState == -1)
		mMovementState = mApp->mSeed->Next() % 9 + 1;

	if (mXMax - 5 < mXD && mVX > 0.1)
		mVX -= 0.1;
	if (mXD < 15.0 && mVX < -1.0)
		mVX += 0.1;

	FishUpdateAnimation();
	if (m0x245)
	{
		mXD += mVX / (mSpeedMod * 5.0);
		mYD += mVY / (mSpeedMod * 5.0);
	}
	else
	{
		mXD += mVX / mSpeedMod;
		mYD += mVY / mSpeedMod;
	}
	Move(mXD, mYD);
}

void Sexy::FishTypePet::MouseDown(int x, int y, int theClickCount)
{
	if (!HandleMouseDown(x, y, theClickCount) && !gUnkBool08)
	{
		gUnkBool08 = true;
		mApp->mBoard->MouseDown(mX + x, mY + y, theClickCount);
		gUnkBool08 = false;
	}
}

void Sexy::FishTypePet::CountRequiredFood(int* theFoodReqPtr)
{
	if (mFishTypePetType == PET_GASH || mFishTypePetType == PET_AMP)
		theFoodReqPtr[0]++;
}

void Sexy::FishTypePet::PrestoMorph(int thePetId)
{
	if (thePetId == mFishTypePetType || !PrestoRightClicked(m0x238))
		return;

	mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
	GameObject* aPet = mApp->mBoard->SpawnPet(thePetId, mX, mY, true, false);
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();

	if ((mFishTypePetType == PET_WALTER && thePetId == PET_PRESTO) ||
		(mFishTypePetType == PET_PRESTO && thePetId == PET_WALTER))
	{
		FishTypePet* aFishPet = (FishTypePet*)aPet;
		aFishPet->m0x260 = m0x260;
		aFishPet->m0x264 = m0x264;
	}
	WadsworthAndMerylFunc01();
}

void Sexy::FishTypePet::Remove()
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();
}

void Sexy::FishTypePet::Sync(DataSync* theSync)
{
	Fish::Sync(theSync);
	theSync->SyncBool(m0x230);
	theSync->SyncLong(mFishTypePetType);
	theSync->SyncLong(m0x23c);
	theSync->SyncLong(m0x240);
	theSync->SyncBool(m0x244);
	theSync->SyncBool(m0x250);
	theSync->SyncDouble(m0x258);
	theSync->SyncLong(m0x248);
	theSync->SyncLong(m0x238);
	theSync->SyncLong(m0x260);
	theSync->SyncLong(m0x264);
}

void Sexy::FishTypePet::DropCoin()
{
	if (mApp->mBoard->mTank == 5 || mApp->mBoard->mPause)
		return;

	int anExoticFoodReqsInTank[8];
	int anExoticFoodInTank[8];

	if (mFishTypePetType == PET_PREGO)
	{
		mCoinDropTimer++;
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && mCoinDropTimer == mCoinDropT - 210)
		{
			 mApp->mBoard->GetExoticFoodsRequiredInTank(anExoticFoodReqsInTank);
			 mApp->mBoard->GetExoticFoodsInTank(anExoticFoodInTank);
			 if (anExoticFoodReqsInTank[0] - anExoticFoodInTank[0] == -1 || (anExoticFoodReqsInTank[0] - anExoticFoodInTank[0]) + 1 < 0)
				 mCoinDropTimer -= 300;
		}
		if (mCoinDropTimer >= mCoinDropT)
		{
			mCoinDropTimer = 0;
			Fish* aGuppy = mApp->mBoard->SpawnGuppy(mX + 7, mY + 25);
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
				aGuppy->mCanBeEatenDelay = 40;
			mApp->mBoard->PlayBirthSound(false);
			if (mApp->mBoard->mFishList->size() > 10)
			{
				mCoinDropT = 2000;
			}
			else
			{
				mCoinDropT = mApp->mBoard->mFishList->size() > 5 ? 1230 : 930;
			}
		}
	}
	else if (mFishTypePetType == PET_VERT)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer >= mCoinDropT)
		{
			mCoinDropTimer = 0;
			if(CanDropCoin())
				mApp->mBoard->DropCoin(mX + 15, mY + 10, 2, nullptr, -1.0, 0);
		}
	}
	else if (mFishTypePetType == PET_NOSTRADAMUS)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer >= mCoinDropT)
		{
			mCoinDropTimer = 0;
			mApp->mBoard->NostradamusDropFood(mX + 15, mY + 10);
		}
	}
	else if (mFishTypePetType == PET_SHRAPNEL)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer >= mCoinDropT)
		{
			mCoinDropTimer = 0;
			mApp->mBoard->DropCoin(mX + 15, mY + 10, Coin::COIN_SHRAPNEL_BOMB, nullptr, -1.0, 0);
			mApp->mBoard->PlaySample(SOUND_UNLEASH_ID, 3, 1.0);
		}
	}
	else if (mFishTypePetType == PET_ZORF)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer >= mCoinDropT)
		{
			bool foundHungry = false;
			for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
			{
				Fish* aFish = mApp->mBoard->mFishList->at(i);
				if (aFish->mHunger < 300 && aFish->mExoticDietFoodType == 0)
				{
					foundHungry = true;
					break;
				}
			}

			for (int i = 0; i < mApp->mBoard->mBreederList->size(); i++)
			{
				Breeder* aFish = mApp->mBoard->mBreederList->at(i);
				if (aFish->mHunger < 300 && aFish->mExoticDietFoodType == 0)
				{
					foundHungry = true;
					break;
				}
			}

			if (mApp->mBoard->mPetsInTank[PET_BRINKLEY] == 0)
			{
				if (!foundHungry)
					return;
			}
			else
			{
				m0x240++;
				int aModeVal = (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK ? 4 : 2);
				if (m0x240 % aModeVal != 0 && !foundHungry)
				{
					mCoinDropTimer = 0;
					return;
				}
			}

			mCoinDropTimer = -10;
			if (mVX < 0.0)
				mApp->mBoard->DropFood(mX + 15, mY + 10, 1, false, 0, -1);
			else
				mApp->mBoard->DropFood(mX + 15, mY + 10, 2, false, 0, -1);
		}
	}
	else if (mFishTypePetType == PET_MERYL)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer == mCoinDropT - 100)
		{
			gMerylActive = true;
			mApp->mBoard->DropCoin(mX + 15, mY - 5, 16, nullptr, -1.0, 0);
			mApp->mBoard->PlaySample(SOUND_SING_ID, 3, 1.0);
		}
		else if(mCoinDropTimer >= mCoinDropT)
		{
			gMerylActive = false;
			mCoinDropTimer = 0;
		}
	}
	else if (mFishTypePetType == PET_GASH)
	{
		mCoinDropTimer++;
	}
	else if (mFishTypePetType == PET_AMP)
	{
		mCoinDropTimer++;
		if (mCoinDropTimer >= mCoinDropT)
		{
			if(mCoinDropTimer == mCoinDropT)
				mApp->mBoard->PlaySample(SOUND_EEL2_ID, 3, 1.0);
			mMouseVisible = true;
			mDoFinger = true;
		}
	}
	else if (mFishTypePetType == PET_STANLEY)
	{
		if (!mApp->mBoard->AliensInTank())
		{
			if (m0x260 > 0)
				m0x260--;
		}
		else if (m0x260 < 10)
			m0x260++;
		mCoinDropTimer++;
		StanleyFunction();
		if (mCoinDropTimer > mCoinDropT)
		{
			mCoinDropTimer = 0;
			if (m0x23c == 0)
				m0x23c = 50;

			GameObject* anObj = FindNearestFood();
			if (anObj != nullptr)
			{
				mApp->mBoard->SpawnMissle((mVX < 0.0) ? mX : mX + 40, mY, anObj, Missle::MISSLE_BALL);
				mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
				mEatingAnimationTimer = 10;
			}
		}
	}
}

bool Sexy::FishTypePet::Hungry()
{
	if (mFishTypePetType == PET_ANGIE && !mApp->mBoard->mDeadFishList->empty())
	{
		HungryBehavior();
		return true;
	}

	if (mFishTypePetType == PET_NIMBUS)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			DeterminePetSleepy(&m0x245);
		if ((!mApp->mBoard->mCoinList1->empty() || (!mApp->mBoard->mFoodList->empty() && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)) 
			&& !m0x245)
		{
			mSpeedMod = 0.5;
			HungryBehavior();
			return true;
		}
		mSpeedMod = 1.8;
	}

	if (mFishTypePetType == PET_ITCHY && mApp->mBoard->AliensInTank())
	{
		HungryBehavior();
		return true;
	}

	if (mFishTypePetType == PET_BRINKLEY)
	{
		if (!mApp->mBoard->AliensInTank() && !mApp->mBoard->mFoodList->empty() && m0x23c == 0)
		{
			HungryBehavior();
			return true;
		}
	}

	if (mFishTypePetType == PET_GASH)
	{
		if (mApp->mBoard->AliensInTank() || (mCoinDropT < mCoinDropTimer && !mApp->mBoard->mFishList->empty()))
		{
			HungryBehavior();
			return true;
		}
	}

	if (mFishTypePetType == PET_GUMBO && mApp->mBoard->AliensInTank())
	{
		HungryBehavior();
		return true;
	}

	if (mFishTypePetType == PET_WADSWORTH)
	{
		if (mCoinDropTimer > 0)
			mCoinDropTimer--;
		else
		{
			if (m0x250 && WadsworthTimerResetCheck())
				mCoinDropTimer = 100;
		}

		if ((!mApp->mBoard->AliensInTank() && mApp->mBoard->mMissleList1->empty()) || 
			(mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && mApp->mBoard->m0x45c > 1 && WadsworthFishesCheck()))
		{
			if (m0x250)
			{
				m0x250 = false;
				gWadsworthTimer--;
				if (gWadsworthTimer < 0)
					gWadsworthTimer = 0;
				mCoinDropTimer = 30;
				mApp->mBoard->SpawnBubble(mX + 11, mY + 5);
				mApp->mBoard->SpawnBubble(mX + 4, mY + 2);
			}
		}
		else
		{
			if (!m0x250)
			{
				gWadsworthTimer++;
				m0x250 = true;
				mCoinDropTimer = mCoinDropT;
			}
			gWadsworthX = mX;
			gWadsworthY = mY;
		}

	}

	return false;
}

void Sexy::FishTypePet::DrawFish(Graphics* g, bool mirror)
{
	if (m0x24c != nullptr)
	{
		g->Translate(m0x24c->mX - g->mTransX, m0x24c->mY - g->mTransY);
		m0x24c->Draw(g);
		g->Translate(mX - g->mTransX, mY - g->mTransY);
	}
	if (m0x230)
	{
		g->SetColor(Color(0xffffff));
	}

	switch (mFishTypePetType)
	{
	case PET_ITCHY:
		DrawItchy(g, mirror);
		break;
	case PET_PREGO:
		DrawPrego(g, mirror);
		break;
	case PET_ZORF:
		DrawZorf(g, mirror);
		break;
	case PET_VERT:
		DrawCommonPet(g, IMAGE_VERT, mirror);
		break;
	case PET_MERYL:
		DrawMeryl(g, mirror);
		break;
	case PET_WADSWORTH:
		DrawWadsworth(g, mirror);
		break;
	case PET_SEYMOUR:
		DrawCommonPet(g, IMAGE_SEYMOUR, mirror);
		break;
	case PET_SHRAPNEL:
		DrawShrapnel(g, mirror);
		break;
	case PET_GUMBO:
		DrawGumbo(g, mirror);
		break;
	case PET_BLIP:
		DrawCommonPet(g, IMAGE_BLIP, mirror);
		break;
	case PET_NIMBUS:
		DrawCommonPet(g, IMAGE_NIMBUS, mirror);
		break;
	case PET_AMP:
		DrawAmp(g, mirror);
		break;
	case PET_GASH:
		DrawGash(g, mirror);
		break;
	case PET_ANGIE:
		DrawAngie(g, mirror);
		break;
	case PET_PRESTO:
		DrawCommonPet(g, IMAGE_PRESTO, mirror);
		break;
	case PET_BRINKLEY:
		DrawCommonPet(g, IMAGE_BRINKLEY, mirror);
		break;
	case PET_NOSTRADAMUS:
	{
		int anXTrans = 0;
		int anYTrans = 0;
		if (mApp->mBoard->m0x2b4 != 0 && mApp->mBoard->m0x2b4 < 25 && !mApp->mBoard->mPause)
		{
			anXTrans = rand() % 21 - 10;
			anYTrans = rand() % 21 - 10;
			g->Translate(anXTrans, anYTrans);
		}
		DrawCommonPet(g, IMAGE_NOSTRADAMUS, mirror);
		g->Translate(-anXTrans, -anYTrans);
		break;
	}
	case PET_STANLEY:
		DrawStanley(g, mirror);
		break;
	case PET_WALTER:
		DrawWalter(g, mirror);
		break;
	}

	if ((m0x230 || mFishTypePetType == PET_NOSTRADAMUS) && m0x248 < 20)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColorizeImages(true);
		g->SetColor(Color(0xaf, 0xaf, 0xaf));
		g->DrawImageMirror(IMAGE_PRESTO, 0, 0, Rect((m0x248/2) * 80, 160, 80, 80), mirror);
		g->SetColorizeImages(false);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}
	if (mMisslePtr)
		DrawCrosshair(g, 0, 0);
	if (m0x230 && mFishTypePetType != PET_PRESTO)
		DrawPrestoMisc(g, m0x238);
}

bool Sexy::FishTypePet::HungryBehavior()
{
	GameObject* aFood = FindNearestFood();
	if (mFishTypePetType == PET_NIMBUS && aFood == nullptr)
	{
		mSpeedMod = 1.8;
		return false;
	}

	if (mSpecialMovementStateChangeTimer >= 5 && aFood != nullptr)
	{
		mSpecialMovementStateChangeTimer = 0;
		if (mFishTypePetType == PET_ITCHY)
		{
			double aCX = mXD + 40.0;
			double aCY = mYD + 40.0;
			int aDist = (aFood->mType == TYPE_BILATERUS) ? 40 : 80;
			if (aCX < aFood->mX + aDist && mVX < 10.0)
				mVX += 2.5;
			else if (aCX > aFood->mX + aDist && mVX > -10.0)
				mVX -= 2.5;

			if (aCY < aFood->mY + aDist && mVY < 4.0)
				mVY += 1.5;
			else if (aCY > aFood->mY + aDist && mVY > -4.0)
				mVY -= 1.5;
			if (mVXAbs < 5)
				mVXAbs++;
		}
		else if (mFishTypePetType == PET_GASH)
		{
			double aCX = mXD + 40.0;
			double aCY = mYD + 40.0;
			int aDist = 40;
			if (!mApp->mBoard->AliensInTank()) // Chase Guppy
			{
				if (aCX < aFood->mX + 40 && mVX < 8.0)
					mVX += 2.5;
				else if (aCX > aFood->mX + 40 && mVX > -8.0)
					mVX -= 2.5;
			}
			else
			{
				aDist = (aFood->mType == TYPE_BILATERUS) ? 40 : 80;
				if (aCX < aFood->mX + aDist && mVX < 9.0)
					mVX += 2.5;
				else if (aCX > aFood->mX + aDist && mVX > -9.0)
					mVX -= 2.5;
			}

			if (aCY < aFood->mY + aDist && mVY < 4.0)
				mVY += 1.5;
			else if(aCY > aFood->mY + aDist && mVY > -4.0)
				mVY -= 1.5;
			if (mVXAbs < 5)
				mVXAbs++;
		}
		else if (mFishTypePetType == PET_GUMBO)
		{
			int aDist = (aFood->mType == TYPE_BILATERUS) ? 40 : 80;
			if (aFood->mY + aDist > 260 && mVY > -8.0)
				mVY -= 2.0;
			else if (aFood->mY + aDist < 300 && mVY < 8.0)
				mVY += 2.0;

			if (aFood->mX + aDist > 290 && mVX > -8.0)
				mVX -= 2.0;
			else if (aFood->mX + aDist < 330 && mVX < 8.0)
				mVX += 2.0;
		}
		else if (mFishTypePetType == PET_ANGIE)
		{
			DeadFish* aDeadFish = (DeadFish*) aFood;
			int aDist = 40;
			if (aDeadFish->mObjType == TYPE_ULTRA)
				aDist = 80;
			int aFX = aFood->mX + aDist;
			double aCX = mXD + 40.0;
			if (aCX > aFX + 4 && mVX > -3.0)
				mVX -= 1.0;
			else if (aCX < aFX - 4 && mVX < 3.0)
				mVX += 1.0;
			else if (aCX > aFX + 2 && mVX > -3.0)
				mVX -= 0.1;
			else if (aCX < aFX - 2 && mVX < 3.0)
				mVX += 0.1;
			else if (aCX > aFX && mVX > -3.0)
				mVX -= 0.05;
			else if (aCX < aFX && mVX < 3.0)
				mVX += 0.05;

			int aFY = aFood->mY + aDist;
			double aCY = mYD + 40.0;
			if (aCY > aFY + 3 && mVY > -2.0)
				mVY -= 0.6;
			else if (aCY < aFY - 3 && mVY < 3.0)
				mVY += 1.0;
			else if (aCY > aFY && mVY > -2.0)
				mVY -= 0.3;
			else if (aCY < aFY && mVY < 3.0)
				mVY += 0.5;

			if (mVXAbs < 5)
				mVXAbs++;
		}
		else if (mFishTypePetType == PET_NIMBUS)
		{
			double aCX = mXD + 40.0;
			if (aCX > aFood->mX + 54 && mVX > -2.3)
				mVX -= 0.5;
			else if (aCX < aFood->mX + 18 && mVX < 2.3)
				mVX += 0.5;
			else if (aCX > aFood->mX + 44 && mVX > -1.3)
				mVX -= 0.1;
			else if (aCX < aFood->mX + 28 && mVX < 1.3)
				mVX += 0.1;
			else if (aCX > aFood->mX + 36 && mVX > -0.3)
				mVX = 0.0;
			else if (aCX < aFood->mX + 36 && mVX < 0.3)
				mVX = 0.0;

			double aCY = mYD + 40.0;
			if (aCY > aFood->mY + 42 && mVY > -2.0)
				mVY -= 0.6;
			else if (aCY < aFood->mY + 30 && mVY < 3.0)
				mVY += 1.0;
			else if (aCY > aFood->mY + 36 && mVY > -2.0)
				mVY -= 0.3;
			else if (aCY < aFood->mY + 36 && mVY < 3.0)
				mVY += 0.5;
		}
		else if (mFishTypePetType == PET_BRINKLEY)
		{
			double aCX = mXD + 40.0;
			if (aCX > aFood->mX + 28 && mVX > -3.0)
				mVX -= 1.0;
			else if (aCX < aFood->mX + 12 && mVX < 3.0)
				mVX += 1.0;
			else if (aCX > aFood->mX + 24 && mVX > -3.0)
				mVX -= 0.1;
			else if (aCX < aFood->mX + 16 && mVX < 3.0)
				mVX += 0.1;
			else if (aCX > aFood->mX + 20 && mVX > -3.0)
				mVX -= 0.05;
			else if (aCX < aFood->mX + 20 && mVX < 3.0)
				mVX += 0.05;

			double aCY = mYD + 40.0;
			if (aCY > aFood->mY + 26 && mVY > -2.0)
				mVY -= 0.6;
			else if (aCY < aFood->mY + 14 && mVY < 3.0)
				mVY += 1.0;
			else if (aCY > aFood->mY + 20 && mVY > -2.0)
				mVY -= 0.3;
			else if (aCY < aFood->mY + 20 && mVY < 3.0)
				mVY += 0.5;

			if (mVXAbs < 5)
				mVXAbs++;
		}
	}
	if (aFood)
		CollideWithFood();
	return false;
}

Sexy::GameObject* Sexy::FishTypePet::FindNearestFood()
{
	int aDist = 10000;
	GameObject* aRet = nullptr;
	if (mFishTypePetType == PET_ITCHY || mFishTypePetType == PET_STANLEY)
	{
		if (mFishTypePetType == PET_STANLEY)
		{
			for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
			{
				Bilaterus* anObj = mApp->mBoard->mBilaterusList->at(i);
				if (anObj->mMisslePtr != nullptr)
					continue;
				int ax = (mXD + 40.0) - (anObj->mX + 40);
				int ay = (mYD + 40.0) - (anObj->mY + 40);
				int aNewDist = sqrt(ax * ax + ay * ay);
				if (aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}
		else
		{
			for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
			{
				Bilaterus* anObj = mApp->mBoard->mBilaterusList->at(i);
				int ax = (mXD + 40.0) - (anObj->mX + 40);
				int ay = (mYD + 40.0) - (anObj->mY + 40);
				int aNewDist = sqrt(ax * ax + ay * ay);
				if (aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}

		for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
		{
			Alien* anObj = mApp->mBoard->mAlienList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 80);
			int ay = (mYD + 40.0) - (anObj->mY + 80);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (!anObj->mIsPsychosquidHealing && aNewDist < aDist)
			{
				if (anObj->mMisslePtr == nullptr || mFishTypePetType != PET_STANLEY)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}
	}
	else if (mFishTypePetType == PET_GASH)
	{
		if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
		{
			if (mCoinDropTimer <= mCoinDropT)
				return aRet;
			for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
			{
				Fish* anObj = mApp->mBoard->mFishList->at(i);
				if (anObj->mVirtualTankId > -1)
					continue;
				int ax = (mXD + 40.0) - (anObj->mX + 40);
				int ay = (mYD + 40.0) - (anObj->mY + 40);
				int aNewDist = sqrt(ax * ax + ay * ay);
				if (aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}

		for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
		{
			Bilaterus* anObj = mApp->mBoard->mBilaterusList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 40);
			int ay = (mYD + 40.0) - (anObj->mY + 40);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
		{
			Alien* anObj = mApp->mBoard->mAlienList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 80);
			int ay = (mYD + 40.0) - (anObj->mY + 80);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (!anObj->mIsPsychosquidHealing && aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}
	}
	else if (mFishTypePetType == PET_GUMBO)
	{
		for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
		{
			Bilaterus* anObj = mApp->mBoard->mBilaterusList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 40);
			int ay = (mYD + 40.0) - (anObj->mY + 40);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
		{
			Alien* anObj = mApp->mBoard->mAlienList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 80);
			int ay = (mYD + 40.0) - (anObj->mY + 80);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (!anObj->mIsPsychosquidHealing && aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}
	}
	else if (mFishTypePetType == PET_ANGIE)
	{
		for (int i = 0; i < mApp->mBoard->mDeadFishList->size(); i++)
		{
			DeadFish* anObj = mApp->mBoard->mDeadFishList->at(i);
			if (anObj->m0x1a0 > 95)
			{
				int ax = (mXD + 40.0) - (anObj->mX + 40);
				int ay = (mYD + 40.0) - (anObj->mY + 40);
				int aNewDist = sqrt(ax * ax + ay * ay);
				if (aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}
	}
	else if (mFishTypePetType == PET_NIMBUS)
	{ 
		if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty() && 
			mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		{
			for (int i = 0; i < mApp->mBoard->mFoodList->size(); i++)
			{
				Food* anObj = mApp->mBoard->mFoodList->at(i);
				int ax = (mXD + 40.0) - (anObj->mX + 20);
				int ay = (mYD + 40.0) - (anObj->mY + 20);
				int aNewDist = sqrt(ax * ax + ay * ay);
				if (anObj->mExoticFoodType == 0 && anObj->m0x18c == 0 && aNewDist < aDist)
				{
					aRet = anObj;
					aDist = aNewDist;
				}
			}
		}

		int aPentaCnt = mApp->mBoard->mPentaList->size();

		for (int i = 0; i < mApp->mBoard->mCoinList1->size(); i++)
		{
			Coin* anObj = mApp->mBoard->mCoinList1->at(i);
			if (anObj->mCoinType != 17)
			{ 
				if ((anObj->mCoinType == 3 || anObj->mCoinType == 10) && aPentaCnt != 0)
					continue;

				if (!anObj->m0x198 && anObj->mCoinType != 18 && !RelaxModeCheck(anObj))
				{
					if (mApp->IsScreenSaver() && anObj->mUpdateCnt > gUnkInt11)
						continue;
					int ax = (mXD + 40.0) - (anObj->mX + 36);
					int ay = (mYD + 40.0) - (anObj->mY + 36);
					int aNewDist = sqrt(ax * ax + ay * ay);
					if (aNewDist < aDist)
					{
						aRet = anObj;
						aDist = aNewDist;
					}
				}
			}
		}
	}
	else if (mFishTypePetType == PET_BRINKLEY)
	{ 
		for (int i = 0; i < mApp->mBoard->mFoodList->size(); i++)
		{
			Food* anObj = mApp->mBoard->mFoodList->at(i);
			int ax = (mXD + 40.0) - (anObj->mX + 20);
			int ay = (mYD + 40.0) - (anObj->mY + 20);
			int aNewDist = sqrt(ax * ax + ay * ay);
			if (aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}
	}
	return aRet;
}

void Sexy::FishTypePet::CollideWithFood()
{
	if (mFishTypePetType == PET_ITCHY)
	{
		for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
		{
			Bilaterus* aBil = mApp->mBoard->mBilaterusList->at(i);
			int aBilX = aBil->mX + 40;
			int aBilY = aBil->mY + 40;
			if (abs(mXD + 40.0 - aBilX) < 20.0 && abs(mYD + 40.0 - aBilY) < 20.0)
			{
				aBil->mActiveHead->m0x1d8 -= 1.0;
				mApp->mBoard->PlayPunchSound(11);
			}
		}

		for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
		{
			Alien* anAlien = mApp->mBoard->mAlienList->at(i);
			int anAlienX = anAlien->mX + 80;
			int anAlienY = anAlien->mY + 80;
			if (abs(mXD + 40.0 - anAlienX) < 20.0 && abs(mYD + 40.0 - anAlienY) < 20.0)
			{
				if (!anAlien->mIsPsychosquidHealing)
				{
					DamageAlien(anAlien);
					break;
				}
			}
		}
	}
	else if (mFishTypePetType == PET_GASH)
	{
		if (mApp->mBoard->AliensInTank())
		{
			for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
			{
				Bilaterus* aBil = mApp->mBoard->mBilaterusList->at(i);
				int aBilX = aBil->mX + 40;
				int aBilY = aBil->mY + 40;
				double aDistX = abs(mXD + 40.0 - aBilX);
				double aDistY = abs(mYD + 40.0 - aBilY);
				if (aDistX < 20.0 && aDistY < 20.0)
				{
					aBil->mActiveHead->m0x1d8 -= 3.0;
					mApp->mBoard->PlayPunchSound(9);
					if (aBil->mActiveHead->m0x1d8 <= 0.0)
					{
						aBil->mActiveHead->Die(true);
						break;
					}
				}
				else if(aDistX < 30.0 && aDistY < 30.0 && mEatingAnimationTimer == 0)
				{
					mEatingAnimationTimer = 10;
					break;
				}
			}

			for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
			{
				Alien* anAlien = mApp->mBoard->mAlienList->at(i);
				int anAlienX = anAlien->mX + 80;
				int anAlienY = anAlien->mY + 80;
				if (abs(mXD + 40.0 - anAlienX) < 20.0 && abs(mYD + 40.0 - anAlienY) < 20.0)
				{
					if (!anAlien->mIsPsychosquidHealing)
					{
						DamageAlien(anAlien);
						break;
					}
				}

				if (abs(mXD + 40.0 - anAlienX) < 30.0 && abs(mYD + 40.0 - anAlienY) < 30.0 && mEatingAnimationTimer == 0)
				{
					mEatingAnimationTimer = 10;
					break;
				}
			}
		}
		else if(mCoinDropT < mCoinDropTimer && !mApp->mBoard->mFishList->empty())
		{
			for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
			{
				Fish* aFish = mApp->mBoard->mFishList->at(i);

				if (aFish->mVirtualTankId < 0)
				{
					int aFishX = aFish->mX + 40;
					int aFishY = aFish->mY + 40;
					double aDistX = abs(mXD + 40.0 - aFishX);
					double aDistY = abs(mYD + 40.0 - aFishY);
					if (aDistX < 20.0 && aDistY < 20.0)
					{
						aFish->RemoveFromGame(true);
						mApp->mBoard->PlayChompSound(false);
						mCoinDropTimer = 0;
						break;
					}
					if (aDistX < 30.0 && aDistY < 30.0 && mEatingAnimationTimer == 0)
					{
						mEatingAnimationTimer = 10;
						break;
					}
				}
			}
		}
	}
	else if (mFishTypePetType == PET_ANGIE)
	{
		for (int i = 0; i < mApp->mBoard->mDeadFishList->size(); i++)
		{
			DeadFish* aFish = mApp->mBoard->mDeadFishList->at(i);

			int aFishX = aFish->mX + 40;
			int aFishY = aFish->mY + 40;
			double anAbsDist = 30.0;
			if (aFish->mObjType == TYPE_ULTRA)
			{
				aFishX = aFish->mX + 80;
				aFishY = aFish->mY + 80;
				anAbsDist = 60.0;
			}
			double aDistX = abs(mXD + 40.0 - aFishX);
			double aDistY = abs(mYD + 40.0 - aFishY);
			if (aDistX < anAbsDist && aDistY < anAbsDist && aFish->m0x1a0 > 95)
			{
				if (aFish->m0x190 == 100)
					aFish->m0x190 = 10;
				break;
			}
		}
	}
	else if (mFishTypePetType == PET_NIMBUS)
	{
		bool isScrSvr = mApp->IsScreenSaver();
		bool noPenta = mApp->mBoard->mPentaList->empty();
		for (int i = 0; i < mApp->mBoard->mCoinList1->size(); i++)
		{
			Coin* aCoin = mApp->mBoard->mCoinList1->at(i);

			int aCoinX = aCoin->mX + 40;
			int aCoinY = aCoin->mY + 40;

			if (abs(mXD + 40.0 - aCoinX) < 30.0 && abs(mYD + 40.0 - aCoinY) < 30.0 &&
				aCoin->mCoinType != 17 && ((aCoin->mCoinType != 3 && aCoin->mCoinType != 10) || noPenta) &&
				!aCoin->m0x198 && aCoin->mCoinType != 18 && (!RelaxModeCheck(aCoin) && (!isScrSvr || aCoin->mUpdateCnt <= gUnkInt11)))
			{
				aCoin->Remove();
				mApp->mBoard->DropCoin(aCoin->mX, mYD - 25.0, 
					(aCoin->mCoinType < 8 ? aCoin->mCoinType + 7 : aCoin->mCoinType),
					nullptr, -1.0, aCoin->mUpdateCnt);
				break;
			}
		}

		if (!mApp->mBoard->AliensInTank() && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		{
			for (int i = 0; i < mApp->mBoard->mFoodList->size(); i++)
			{
				Food* aFood = mApp->mBoard->mFoodList->at(i);

				int aFoodX = aFood->mX + 20;
				int aFoodY = aFood->mY + 20;
				double aDistX = abs(mXD + 40.0 - aFoodX);
				double aDistY = abs(mYD + 40.0 - aFoodY);
				if (aDistX < 30.0 && aDistY < 30.0 && aFood->m0x18c == 0)
				{
					aFood->Remove();
					mApp->mBoard->DropFood(aFood->mX, mYD - 30.0, 0, true, 0, aFood->mFoodType);
					break;
				}
			}
		}
	}
	else if (mFishTypePetType == PET_BRINKLEY)
	{
		for (int i = 0; i < mApp->mBoard->mFoodList->size(); i++)
		{
			Food* aFood = mApp->mBoard->mFoodList->at(i);

			double aCX = mXD + 40.0;
			double aCY = mYD + 40.0;
			if (aCX < aFood->mX + 35 && aCX > aFood->mX + 5 && 
				aCY < aFood->mY + 35 && aCY > aFood->mY)
			{
				mApp->mBoard->PlaySlurpSound(false);
				aFood->Remove();
				m0x240++;
				m0x23c = mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK ? 108 : 45;
				if (m0x240 % 3 == 0 && CanDropCoin())
				{
					int aCoinType = 10;
					if (m0x240 % 135 == 0)
						aCoinType = 14;
					else if (m0x240 % 45 == 0)
						aCoinType = 13;
					else if (m0x240 % 15 == 0)
						aCoinType = 11;

					mApp->mBoard->DropCoin(mX + 5, mY - 10, aCoinType, nullptr, -1.0, 0);
				}
				break;
			}
		}
	}
}

void Sexy::FishTypePet::FishUpdateAnimation()
{
	if (mPrevVX < 0.0 && mVX > 0.0)
	{
		if (mFishTypePetType == PET_PREGO && mCoinDropT - 220 < mCoinDropTimer)
			mVX = 0;
		else if (mFishTypePetType == PET_WALTER && m0x24c != 0)
			mVX = 0;
		else
			mTurnAnimationTimer = -20;
	}
	else if (mPrevVX > 0.0 && mVX < 0.0)
	{
		if (mFishTypePetType == PET_PREGO && mCoinDropT - 220 < mCoinDropTimer)
			mVX = 0;
		else if (mFishTypePetType == PET_WALTER && m0x24c != 0)
			mVX = 0;
		else
			mTurnAnimationTimer = 20;
	}

	if (mTurnAnimationTimer > 0)
	{
		mTurnAnimationTimer--;
		if (mTurnAnimationTimer == 0)
			mEatingAnimationTimer = 0;
	}
	else if (mTurnAnimationTimer < 0)
	{
		mTurnAnimationTimer++;
		if (mTurnAnimationTimer == 0)
			mEatingAnimationTimer = 0;
	}

	if (mTurnAnimationTimer != 0)
	{
		if (mFishTypePetType == PET_NIMBUS)
		{
			if (mTurnAnimationTimer > 0)
				mAnimationFrameIndexFish = 9 - mTurnAnimationTimer / 2;
		}
		else if(mTurnAnimationTimer > 0)
			mAnimationFrameIndexFish = 9 - mTurnAnimationTimer / 2;
		if(mTurnAnimationTimer < 0)
			mAnimationFrameIndexFish = mTurnAnimationTimer / 2 + 9;
	}
	else
	{ // 70
		if (mFishTypePetType == PET_VERT)
		{
			mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			if (mSwimFrameCounter < 10)
				mAnimationFrameIndexFish = mSwimFrameCounter;
			else
				mAnimationFrameIndexFish = 19 - mSwimFrameCounter;
		}
		else if (mFishTypePetType == PET_WALTER)
		{
			if (mEatingAnimationTimer <= 0)
			{
				if (mVXAbs < 2)
					mSwimFrameCounter++;
				else
					mSwimFrameCounter += 2;
				if (mSwimFrameCounter >= 40)
					mSwimFrameCounter = 0;
				if (mSwimFrameCounter < 20)
					mAnimationFrameIndexFish = mSwimFrameCounter / 2;
				else
					mAnimationFrameIndexFish = 19 - mSwimFrameCounter / 2;
			}
			else
			{
				int aPrevEat = mEatingAnimationTimer;
				mEatingAnimationTimer--;
				if (mEatingAnimationTimer >= 30)
					mAnimationFrameIndexFish = 9 - (aPrevEat - 21) / 2;
				else if (mEatingAnimationTimer >= 10)
					mAnimationFrameIndexFish = 4;
				else
					mAnimationFrameIndexFish = mEatingAnimationTimer / 2;
			}
		}
		else if (mFishTypePetType == PET_NIMBUS || mFishTypePetType == PET_BRINKLEY)
		{
			if (mVXAbs < 2 || m0x245)
				mSwimFrameCounter++;
			else
				mSwimFrameCounter += 2;
			if (mSwimFrameCounter >= 40)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 4;
		}
		else if (mFishTypePetType == PET_STANLEY)
		{ // 137
			if (m0x260 > 0 && m0x260 < 10)
				mAnimationFrameIndexFish = m0x260;
			else
			{
				if (mEatingAnimationTimer < 1)
				{
					if (mVXAbs < 2)
						mSwimFrameCounter++;
					else
						mSwimFrameCounter += 2;
					if (mSwimFrameCounter >= 40)
						mSwimFrameCounter = 0;
					mAnimationFrameIndexFish = mSwimFrameCounter / 4;
				}
				else
				{
					mEatingAnimationTimer--;
					mAnimationFrameIndexFish = mEatingAnimationTimer;
				}
			}
		}
		else if (mFishTypePetType == PET_GASH)
		{
			if (mEatingAnimationTimer < 1)
			{
				mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
				if (mSwimFrameCounter > 19)
					mSwimFrameCounter = 0;
				mAnimationFrameIndexFish = mSwimFrameCounter/2;
			}
			else
			{
				mEatingAnimationTimer--;
				mAnimationFrameIndexFish = mEatingAnimationTimer;
			}
		}
		else if (mFishTypePetType == PET_GUMBO)
		{
			mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
			if (mApp->mBoard->AliensInTank())
			{
				m0x258 += 0.1;
				if (m0x258 >= 1.0)
					m0x258 = -1;
			}
		}
		else if (mFishTypePetType == PET_SHRAPNEL)
		{
			if (mVXAbs < 3)
				mSwimFrameCounter++;
			else
				mSwimFrameCounter += 2;
			if (mSwimFrameCounter > 59)
				mSwimFrameCounter = 0;
			m0x258 += 0.1;
			if (m0x258 >= 1.0)
				m0x258 = -1;
			mAnimationFrameIndexFish = mSwimFrameCounter / 6;
		}
		else if (mFishTypePetType == PET_ANGIE)
		{
			mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			m0x258 += 0.1;
			if (m0x258 >= 1.0)
				m0x258 = -1;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
		}
		else if (mFishTypePetType == PET_SEYMOUR)
		{
			mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
			if (mSwimFrameCounter > 79)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 8;
		}
		else if (mFishTypePetType == PET_ZORF)
		{
			mSwimFrameCounter++;
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
			if (mCoinDropTimer == -1)
				mSwimFrameCounter = 10;
		}
		else if (mFishTypePetType == PET_AMP)
		{
			mSwimFrameCounter++;
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
			if (mCoinDropTimer < mCoinDropT)
				m0x258 += 0.5;
			else
				m0x258 += 0.1;
			if (m0x258 >= 1.0)
				m0x258 = -1;
		}
		else
		{
			mSwimFrameCounter += (mVXAbs > 1 ? 2 : 1);
			if (mSwimFrameCounter > 19)
				mSwimFrameCounter = 0;
			mAnimationFrameIndexFish = mSwimFrameCounter / 2;
		}

		if (mFishTypePetType == PET_MERYL && mAnimationFrameIndexFish == 0)
		{
			if (!m0x244)
				m0x244 = ((mApp->mSeed->Next() % 10) == 0);
			else
				m0x244 = false;
		}
	}

	if (mPrevVX != mVX && mPrevVX != 0 && mVX != 0)
		mPrevVX = mVX;

	if(mMisslePtr)
		UpdateCrosshairAnimation();
}

void Sexy::FishTypePet::WadsworthAndMerylFunc01()
{
	if (mFishTypePetType == PET_WADSWORTH && m0x250)
	{
		m0x250 = false;
		gWadsworthTimer--;
		if (gWadsworthTimer < 0)
			gWadsworthTimer = 0;
	}
	if (mFishTypePetType == PET_MERYL)
		gMerylActive = false;
}

bool Sexy::FishTypePet::HandleMouseDown(int x, int y, int theClickCount)
{
	if (theClickCount < 0)
	{
		if (!m0x230)
		{
			mApp->mBoard->CheckMouseDown(mX + x, mY + y);
			return true;
		}

		if (PrestoRightClicked(m0x238))
			mApp->OpenPrestoDialog(this);
		return true;
	}

	if (mFishTypePetType == PET_AMP && mCoinDropT <= mCoinDropTimer)
	{
		if (m0x260 < 2)
		{
			m0x260++;
			mApp->mBoard->PlaySample(SOUND_EEL3_ID, 3, 1.0);
		}
		else
		{
			mMouseVisible = gUnkBool06;
			mDoFinger = false;
			ShowFinger(false);
			m0x258 = 0;

			int aMaxDropCount = 0;
			int aCounter = 0;
			std::vector<Fish*> aFishVector;
			for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
			{
				Fish* aFish = mApp->mBoard->mFishList->at(i);

				if (aFish->mVirtualTankId < 0)
				{
					aMaxDropCount++;
					aFishVector.push_back(aFish);
				}
			}

			aMaxDropCount = (aMaxDropCount - 5) / 2 + 20;
			if (aMaxDropCount < 20)
				aMaxDropCount = 20;

			for (int i = 0; i < aFishVector.size(); i++)
			{
				Fish* aFish = aFishVector.at(i);

				aFish->Die(false);
				if (aCounter < aMaxDropCount)
					mApp->mBoard->DropCoin(mXD + 15, mYD + 10, 5, nullptr, -1, 0);

				mApp->mBoard->SpawnShot(mXD, mYD, mApp->mSeed->Next() % 3 + 3);
				aCounter++;
			}

			std::vector<FishTypePet*> aFTPVector;
			for (int i = 0; i < mApp->mBoard->mFishTypePetList->size(); i++)
			{
				FishTypePet* aPet = mApp->mBoard->mFishTypePetList->at(i);

				if (!aPet->m0x230 && aPet->mFishTypePetType == PET_NOSTRADAMUS && aPet->m0x240 > 0)
					aFTPVector.push_back(aPet);
			}

			for (int i = 0; i < aFTPVector.size(); i++)
			{
				FishTypePet* aPet = aFTPVector.at(i);

				aPet->RemoveHelper02(true);
				mApp->mBoard->DropCoin(mXD + 15, mYD + 10, 5, nullptr, -1, 0);
				mApp->mBoard->SpawnShot(mXD, mYD, mApp->mSeed->Next() % 3 + 3);
				aCounter++;
			}

			if (!mApp->mBoard->HasAnyFish() && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				mApp->mBoard->SpawnGuppyBought();
			mApp->mBoard->PlaySample(SOUND_EEL1_ID, 3, 1.0);
			if (aCounter > 0)
				mApp->mBoard->PlayDieSound(-1);
			if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				mCoinDropT += 200;
			mCoinDropTimer = -20;
			m0x260 = 0;
		}
		return true;
	}

	if (mFishTypePetType == PET_WALTER && mEatingAnimationTimer == 0 && m0x24c == nullptr
		&& m0x264 == 0 && mTurnAnimationTimer == 0)
	{
		if (mVX < 0.0)
			m0x24c = new BoxingGlove(this, false);
		else
		{
			if ((int)mVX == 0 && mPrevVX < 0)
				m0x24c = new BoxingGlove(this, false);
			else
				m0x24c = new BoxingGlove(this, true);
		}

		if (m0x24c != nullptr)
		{
			if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				m0x260++;
				if (m0x260 > 4)
				{
					m0x264 = gConst01;
					m0x260 = 0;
					mDoFinger = false;
					mMovementState = 0;
					ShowFinger(false);
				}
			}

			mAnimationFrameIndexFish = 0;
			mEatingAnimationTimer = 40;
			mApp->mBoard->PlaySample(SOUND_SFX_ID, 3, 1.0);
			mApp->mBoard->SortGameObjects();
		}

		return true;
	}


	return false;
}

bool Sexy::FishTypePet::WadsworthTimerResetCheck()
{
	if (gWadsworthTimer > 1)
		return false;

	for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
	{
		Fish* aFish = mApp->mBoard->mFishList->at(i);

		int aX = aFish->mX + 40;
		int aY = aFish->mY + 40;
		if (abs(mXD + 40.0 - aX) > 10.0 && abs(mYD + 40.0 - aY) > 10.0 && aFish->mSize < TYPE_BIG_GUPPY)
			return true;
	}
	return false;
}

bool Sexy::FishTypePet::WadsworthFishesCheck()
{
	Board* aB = mApp->mBoard;
	if (aB->mOscarList->empty() && aB->mPentaList->empty() && aB->mGrubberList->empty() && aB->mGekkoList->empty() &&
		aB->mBreederList->empty() && aB->mUltraList->empty())
	{
		if (aB->mFishList->empty())
			return true;
		for (int i = 0; i < aB->mFishList->size(); i++)
		{
			Fish* aFish = aB->mFishList->at(i);
			if (aFish->mSize > TYPE_MEDIUM_GUPPY)
				break;
			if(i == aB->mFishList->size() - 1)
				return true;
		}
	}
	return false;
}

void Sexy::FishTypePet::StanleyFunction()
{
	for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
	{
		Alien* anAl = mApp->mBoard->mAlienList->at(i);
	}

	if (m0x23c < 1)
	{
		int aDist = 100000000;
		int loc8 = 0;
		Missle* aClosestMis = nullptr;
		for (int i = 0; i < mApp->mBoard->mMissleList1->size(); i++)
		{
			Missle* aMis = mApp->mBoard->mMissleList1->at(i);
			int aPrevloc8 = loc8;
			if (aMis->m0x17c < 11 && aMis->m0x1a8 < 1 && (aMis->mMissleType == Missle::MISSLE_ENERGYBALL ||
				aMis->mMissleType == Missle::MISSLE_CLASSIC) && aMis->mTarget != nullptr)
			{
				if (aMis->mMissleType == Missle::MISSLE_CLASSIC)
				{
					if (loc8 != 0) continue;
				}
				else if (!aMis->m0x190)
				{
					if (loc8 > 1) continue;
					loc8 = 1;
				}
				else
					loc8 = 2;

				GameObject* aTar = aMis->mTarget;
				int ax = (aTar->mWidth / 2 + aTar->mX) - (aMis->mWidth / 2 + aMis->mX);
				int ay = (aTar->mHeight / 2 + aTar->mY) - (aMis->mHeight / 2 + aMis->mY);
				int aNewDist = ax * ax + ay * ay;
				if (aNewDist < aDist || loc8 != aPrevloc8)
				{
					aClosestMis = aMis;
					aDist = aNewDist;
				}
			}
		}

		if (m0x23c < 1)
		{
			if (aClosestMis != nullptr)
			{
				int aVal = Rand() % 100;
				if (aVal < 10)
					m0x23c = 30;
				else
					m0x23c = (aVal > 59) ? 90 : 60;

				mCoinDropTimer = 0;
				mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);

				int aStanleyCenterX = mX + mWidth / 2;
				int aStanleyShootY = mY + 10;

				aClosestMis->m0x1b0 = aStanleyCenterX;
				aClosestMis->m0x1b4 = aStanleyShootY;
				aClosestMis->m0x1a8 = 1;

				int aAlienCenterX = aClosestMis->mX + aClosestMis->mWidth / 2;
				int aAlienCenterY = aClosestMis->mY + aClosestMis->mHeight / 2;

				int dX = aStanleyCenterX - aAlienCenterX;
				int dY = aStanleyShootY - aAlienCenterY;

				int aDist = sqrt(dX * dX + dY * dY);
				aClosestMis->m0x1ac = aDist * 0.0625;

				if (mEatingAnimationTimer == 0) mEatingAnimationTimer = 10;
			}
		}
		else if (loc8 > 1)
		{
			m0x23c -= 2;
			if (m0x23c < 0)
				m0x23c = 0;
		}
	}
}

void Sexy::FishTypePet::DamageAlien(Alien* theAlien)
{
	if (mFishTypePetType == PET_ITCHY)
	{
		if (theAlien->mAlienType == Alien::ALIEN_DESTRUCTOR || theAlien->mAlienType == Alien::ALIEN_GUS)
			theAlien->mHealth -= 0.25;
		else
			theAlien->mHealth -= 1.0;
		mApp->mBoard->PlayPunchSound(11);
	}
	else if (mFishTypePetType == PET_GASH)
	{
		if (theAlien->mAlienType == Alien::ALIEN_DESTRUCTOR || theAlien->mAlienType == Alien::ALIEN_GUS)
			theAlien->mHealth -= 0.5;
		else
			theAlien->mHealth -= 3.0;
		if (theAlien->mHealth <= 0.0)
			theAlien->Remove(true);
		mApp->mBoard->PlayPunchSound(9);
	}
}

void Sexy::FishTypePet::DrawItchy(Graphics* g, bool mirror)
{
	int aRow = 0;
	Board* aBoard = mApp->mBoard;
	if (mTurnAnimationTimer == 0)
	{
		if (!aBoard->mAlienList->empty() || !aBoard->mBilaterusList->empty())
			aRow = 2;
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		if (!aBoard->mAlienList->empty() || !aBoard->mBilaterusList->empty())
			aRow = 3;
		else
			aRow = 1;
	}
	g->DrawImageMirror(IMAGE_ITCHY, 0, 0, Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80), mirror);
}

void Sexy::FishTypePet::DrawPrego(Graphics* g, bool mirror)
{
	int aFrame = mAnimationFrameIndexFish;
	int aRow = 0;
	if (mTurnAnimationTimer == 0)
	{
		if (mCoinDropT - 200 < mCoinDropTimer)
		{
			if (mCoinDropTimer < mCoinDropT - 190)
			{
				aRow = 3;
				aFrame = (mCoinDropTimer - mCoinDropT + 200) / 2;
			}
			else if (mCoinDropT - 5 < mCoinDropTimer)
			{
				aRow = 3;
				aFrame = (mCoinDropTimer - mCoinDropT + 10);
			}
			else
				aRow = 1;
		}
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		aRow = 2;
	}
	g->DrawImageMirror(IMAGE_PREGO, 0, 0, Rect(aFrame * 80, aRow * 80, 80, 80), mirror);
}

void Sexy::FishTypePet::DrawZorf(Graphics* g, bool mirror)
{
	int aFrame = mAnimationFrameIndexFish;
	int aRow = 0;
	if (mTurnAnimationTimer == 0)
	{
		if (mCoinDropTimer < 0)
		{
			aRow = 2;
			aFrame = mCoinDropTimer + 10;
		}
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		aRow = 1;
	}
	g->DrawImageMirror(IMAGE_ZORF, 0, 0, Rect(aFrame * 80, aRow * 80, 80, 80), mirror);
}

void Sexy::FishTypePet::DrawMeryl(Graphics* g, bool mirror)
{
	int aRow = 0;
	if (mTurnAnimationTimer == 0)
	{
		if (mCoinDropT - 100 < mCoinDropTimer)
			aRow = 1;
		else if (mAnimationFrameIndexFish < 5 && m0x244)
			aRow = 3;
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		aRow = 2;
	}
	g->DrawImageMirror(IMAGE_MERYL, 0, 0, Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80), mirror);
}

void Sexy::FishTypePet::DrawWadsworth(Graphics* g, bool mirror)
{
	int aFrame = mAnimationFrameIndexFish;
	int aRow = 0;
	if (mCoinDropTimer < 1)
	{
		if (mTurnAnimationTimer != 0)
		{
			mirror = mTurnAnimationTimer > 0;
			aRow = 1;
		}
	}
	else
	{
		aRow = 2;
		bool framehandled = false;
		if (!m0x250)
		{
			if (mCoinDropTimer > 20)
			{
				aFrame = 9 - (29 - mCoinDropTimer) / 2;
				framehandled = true;
			}
		}
		else if (mCoinDropT - 10 < mCoinDropTimer)
		{
			aFrame = 9 - (mCoinDropT - mCoinDropTimer - 1) / 2;
			framehandled = true;
		}
		if (!framehandled)
		{
			if (mCoinDropTimer < 11)
				aFrame = 9 - (mCoinDropTimer - 1) / 2;
			else
				aFrame = 5;
		}
	}
	g->DrawImageMirror(IMAGE_WADSWORTH, 0, 0, Rect(aFrame * 80, aRow * 80, 80, 80), mirror);
	if (!m0x250)
	{
		if ((!mApp->mBoard->mMissleList1->empty() || !mApp->mBoard->mAlienList->empty() || !mApp->mBoard->mBilaterusList->empty()) 
			&& mApp->mBoard->mTank != 5)
				g->DrawImage(IMAGE_ZZZ, 40, -15);
	}
}

void Sexy::FishTypePet::DrawShrapnel(Graphics* g, bool mirror)
{
	if (mTurnAnimationTimer != 0)
		mirror = mTurnAnimationTimer > 0;
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, (mTurnAnimationTimer != 0) * 80, 80, 80);
	g->DrawImageMirror(IMAGE_SHRAPNEL, 0, 0, aSrcRect, mirror);
	if (mCoinDropT - 50 < mCoinDropTimer)
	{
		g->SetColorizeImages(true);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColor(Color(255,255,255, abs(m0x258) * 255));
		g->DrawImageMirror(IMAGE_SHRAPNEL, 0, 0, aSrcRect, mirror);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		g->SetColorizeImages(false);
	}
}

void Sexy::FishTypePet::DrawGumbo(Graphics* g, bool mirror)
{
	if (mTurnAnimationTimer != 0)
		mirror = mTurnAnimationTimer > 0;
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, (mTurnAnimationTimer != 0) * 80, 80, 80);
	g->DrawImageMirror(IMAGE_GUMBO, 0, 0, aSrcRect, mirror);

	if (!mApp->mBoard->mAlienList->empty() || !mApp->mBoard->mBilaterusList->empty())
	{
		g->SetColorizeImages(true);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColor(Color(255, 255, 0, abs(m0x258) * 255));
		g->DrawImageMirror(IMAGE_GUMBOLIGHT, 0, 0, aSrcRect, mirror);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		g->SetColorizeImages(false);
	}
}

void Sexy::FishTypePet::DrawAmp(Graphics* g, bool mirror)
{
	if (mTurnAnimationTimer != 0)
		mirror = mTurnAnimationTimer > 0;
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 160, (mTurnAnimationTimer != 0) * 60, 160, 60);
	g->DrawImageMirror(IMAGE_AMP, 0, 0, aSrcRect, mirror);
	int anAlpha = 0;
	int aGreen = 255;
	int aBlue = 200;
	if (mCoinDropTimer < mCoinDropT)
	{
		if (mCoinDropTimer > -1)
			return;
		if (mApp->mBoard->AliensInTank())
			return;
		g->SetColorizeImages(true);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		anAlpha = (abs(m0x258) * 2) * 255;
	}
	else
	{
		g->SetColorizeImages(true);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		anAlpha = (abs(m0x258) * 2) * 255;
		if (m0x260 != 0)
		{
			aBlue = 100;
			if (m0x260 == 1)
				aGreen = 200;
			else
				aGreen = 100;
		}
	}
	if (anAlpha > 255) anAlpha = 255;
	g->SetColor(Color(255, aGreen, aBlue, anAlpha));
	g->DrawImageMirror(IMAGE_AMPCHARGE, 0, 0, aSrcRect, mirror);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);
}

void Sexy::FishTypePet::DrawGash(Graphics* g, bool mirror)
{
	int aRow = 0;
	if (mTurnAnimationTimer == 0)
	{
		if (mEatingAnimationTimer > 0)
			aRow = 2;
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		aRow = 1;
	}
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80);
	g->DrawImageMirror(IMAGE_GASH, 0, 0, aSrcRect, mirror);
}

void Sexy::FishTypePet::DrawAngie(Graphics* g, bool mirror)
{
	if (mTurnAnimationTimer != 0)
		mirror = mTurnAnimationTimer > 0;
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, (mTurnAnimationTimer != 0) * 80, 80, 80);
	g->DrawImageMirror(IMAGE_ANGIE, 0, 0, aSrcRect, mirror);

	g->SetColorizeImages(true);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->SetColor(Color(255, 255, 255, abs(m0x258) * 255));
	g->DrawImageMirror(IMAGE_HALO, 0, 0, aSrcRect, mirror);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);
}

void Sexy::FishTypePet::DrawStanley(Graphics* g, bool mirror)
{
	int aRow = 0;
	bool mirrorHandled = false;
	if (!mApp->mBoard->mAlienList->empty() || !mApp->mBoard->mBilaterusList->empty())
	{
		if (m0x260 < 10)
		{
			aRow = 5;
			mirrorHandled = true;
		}
		else if (mEatingAnimationTimer != 0)
		{
			aRow = 4;
			mirrorHandled = true;
		}
		else
		{
			if (mTurnAnimationTimer == 0)
			{
				aRow = 2;
				mirrorHandled = true;
			}
			else
				aRow = 3;
		}
	}
	else
	{
		if (m0x260 != 0)
		{
			aRow = 5;
			mirrorHandled = true;
		}
		else if (mTurnAnimationTimer != 0)
			aRow = 1;
	}
	if (mTurnAnimationTimer != 0 && !mirrorHandled)
		mirror = mTurnAnimationTimer > 0;

	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80);
	g->DrawImageMirror(IMAGE_STANLEY, 0, 0, aSrcRect, mirror);
}

void Sexy::FishTypePet::DrawWalter(Graphics* g, bool mirror)
{
	if (m0x264 > 0 && m0x264 < gConst01 - 25)
		g->DrawImage(IMAGE_ZZZ, mirror ? 50 : 30, -15);
	int aRow = 0;
	if (mTurnAnimationTimer == 0)
	{
		if (mEatingAnimationTimer > 0)
			aRow = 2;
	}
	else
	{
		mirror = mTurnAnimationTimer > 0;
		aRow = 1;
	}
	Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80);
	g->DrawImageMirror(IMAGE_WALTER, 0, 0, aSrcRect, mirror);
}

void Sexy::FishTypePet::DrawCommonPet(Graphics* g, Image* theImage, bool mirror)
{
	if (m0x245)
		g->DrawImage(IMAGE_ZZZ, mirror ? 50 : 30, -10);

	if (mTurnAnimationTimer != 0)
		mirror = mTurnAnimationTimer > 0;

	g->DrawImageMirror(theImage, 0, 0, Rect(mAnimationFrameIndexFish * 80, (mTurnAnimationTimer != 0) * 80, 80, 80), mirror);
}

