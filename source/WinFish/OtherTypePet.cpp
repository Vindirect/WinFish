#include "OtherTypePet.h"
#include "BilaterusHead.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::OtherTypePet::OtherTypePet()
{
	mClip = false;
	mSleepy = false;
	mType = TYPE_OTHER_TYPE_PET;
}

Sexy::OtherTypePet::OtherTypePet(int theX, int theY, int thePetType, int theBackgroundId, bool isPresto)
{
	mClip = false;
	mSleepy = false;
	mIsPresto = isPresto;
	mOtherTypePetType = thePetType;
	mType = TYPE_OTHER_TYPE_PET;
	mMouseVisible = gUnkBool06;
	mPrestoTransformAnimTimer = 0;
	if (!isPresto || thePetType == PET_PRESTO)
		mPrestoTimer = 0;
	else
		mPrestoTimer = 360;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		mPrestoTimer = 0;

	mXD = theX;
	mYD = theY;

	if (thePetType == PET_STINKY && !isPresto)
		mYD = 360;
	else if (thePetType == PET_RHUBARB && !isPresto)
		mYD = 355;
	else if (thePetType == PET_RUFUS && !isPresto)
		mYD = 365;

	UpdateNikoPosition(theBackgroundId);
	mX = mXD;
	mY = mYD;
	mVX = 0;
	mVY = 0;
	mTargetVX = 0;
	mWidth = 80;
	mHeight = 80;
	mPrevVX = 1;
	mSpeedMod = 2;
	if(mOtherTypePetType == PET_STINKY)
		mSpeedMod = 1.2;
	else if(mOtherTypePetType == PET_CLYDE)
		mSpeedMod = 1.5;

	mChaseEntityTimer = 40;
	mMovementStateChangeTimer = 0;
	mMovementAnimationTimer = 0;
	mTurnAnimationTimer = 0;
	mAnimationIndex = 0;
	mPetSpecialtyTimer = 0;
	mMovementState = mApp->mSeed->Next() % 10;
	m0x1c0 = false;
	mPetAngryTimer = 0;
	m0x1b8 = mApp->mSeed->Next() % 250 + 250;
}

void Sexy::OtherTypePet::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	if ((mIsPresto && (mOtherTypePetType == PET_NIKO || mOtherTypePetType == PET_RUFUS || mOtherTypePetType == PET_RHUBARB || mOtherTypePetType == PET_STINKY))
		&& mYD < 380.0)
		mVY += 0.1;

	if (mApp->mBoard->mTank == 5 || mOtherTypePetType != PET_STINKY) // 32
	{
		if (mOtherTypePetType == PET_CLYDE)
		{
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
				DeterminePetSleepy(&mSleepy);

			if (mApp->mBoard->mTank == 5 || !ChaseEnemyBehavior())
			{
				if (mMovementState == 0 || mPetSpecialtyTimer != 0)
					mTargetVX = 0;
				else if(mMovementState == 1)
					mTargetVX = -0.5;
				else if(mMovementState == 2)
					mTargetVX = 0.5;
			}

			if (mTargetVX < mVX)
				mVX -= 0.1;
			if (mTargetVX > mVX)
				mVX += 0.1;

			if (mMovementAnimationTimer == 2 || mMovementAnimationTimer == 3)
			{
				if (mYD >= 240.0)
					mVY -= 0.75;
				else
					mVY -= 0.3;
			}
			else
				mVY += 0.03;
		}
		else if (mOtherTypePetType == PET_RUFUS) // 73
		{
			if (mApp->mBoard->mTank == 5 || !ChaseEnemyBehavior())
			{
				if (mMovementState == 0)
					mTargetVX = 0;
				else if (mMovementState == 1)
					mTargetVX = -0.5;
				else if (mMovementState == 2)
					mTargetVX = 0.5;
				else if (mMovementState == 3)
					mTargetVX = -1;
				else if (mMovementState == 4)
					mTargetVX = 1;
				else if (mMovementState == 5)
					mTargetVX = 0;
				else if (mMovementState == 6)
					mTargetVX = 0;
				else if(mMovementState == 7)
					mTargetVX = -2.5;
				else if(mMovementState == 8)
					mTargetVX = 2.5;
			}
		}
		else if (mOtherTypePetType == PET_RHUBARB && mApp->mBoard->mTank != 5) // 110
		{
			ChaseEnemyBehavior();
			if (mMovementState == 0)
				mTargetVX = 0;
			else if (mMovementState == 1)
				mTargetVX = -0.5;
			else if (mMovementState == 2)
				mTargetVX = 0.5;
			else if (mMovementState == 3)
				mTargetVX = -1;
			else if (mMovementState == 4)
				mTargetVX = 1;
			else if (mMovementState == 5)
				mTargetVX = 1.5;
			else if (mMovementState == 6)
				mTargetVX = -1.5;
			else if (mMovementState == 7)
				mTargetVX = -2.5;
			else if (mMovementState == 8)
				mTargetVX = 2.5;
		}

		if (mOtherTypePetType == PET_RUFUS || mOtherTypePetType == PET_RHUBARB)
		{
			if (mTargetVX < mVX)
				mVX -= 0.1;
			if (mTargetVX > mVX)
				mVX += 0.1;
		}
	}
	else // 146 // IS stinky and isnt bg 5
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			if(DeterminePetSleepy(&mSleepy), mSleepy)
				mPetAngryTimer = 0;
		if (!ChaseEnemyBehavior())
		{
			if (mMovementState == 0 || mPetSpecialtyTimer != 0)
				mTargetVX = 0;
			else if(mMovementState == 1)
				mTargetVX = -0.5;
			else if(mMovementState == 2)
				mTargetVX = 0.5;

			if (mVX < mTargetVX)
			{
				mVX += 0.1;
				if (mTargetVX < mVX)
					mVX = mTargetVX;
			}
			else if (mVX > mTargetVX)
			{
				mVX -= 0.1;
				if (mTargetVX > mVX)
					mVX = mTargetVX;
			}
		}

		if (mPetSpecialtyTimer != 0)
			mVX = 0;
	}

	mMovementStateChangeTimer++;
	mChaseEntityTimer++;

	if ((mMovementStateChangeTimer > 20 || (mXD <= 10.0 && mTargetVX <= 0.0)) || (mXD >= 540.0 && mYD >= 0.0))
	{
		mMovementStateChangeTimer = 0;
		if (mApp->mSeed->Next() % 10 == 0)
		{
			if (mOtherTypePetType == PET_RUFUS || mOtherTypePetType == PET_RHUBARB)
				mMovementState = mApp->mSeed->Next() % 9;
			else
				mMovementState = mApp->mSeed->Next() % 3;
		}
	}

	if (mPrestoTimer != 0)
		mPrestoTimer--;
	if (mPrestoTransformAnimTimer < 20)
		mPrestoTransformAnimTimer++;
	 
	UpdatePetSpecialAnimations();
	
	double aXCap = 540;
	if (mOtherTypePetType == PET_STINKY || mOtherTypePetType == PET_CLYDE)
		aXCap = 550;
	else if(mOtherTypePetType == PET_RUFUS)
		aXCap = 560;

	if (mXD > aXCap)
		mXD = aXCap;
	if (mXD < 10.0)
		mXD = 10;

	if (mYD > 350 && mOtherTypePetType == PET_NIKO)
	{
		mYD = 350;
		mVY = 0;
	}
	else if (mYD > 365 && mOtherTypePetType == PET_RUFUS)
	{
		mYD = 365;
		mVY = 0;
	}
	else if (mYD > 355 && mOtherTypePetType == PET_RHUBARB)
	{
		mYD = 355;
		mVY = 0;
	}
	else if (mYD > 370)
	{
		mYD = 370;
		mVY = 0;
	}

	if (mYD < 95)
		mYD = 95;

	if (mXD > 535 && mVX > 0.1)
		mMovementState = 1;
	if (mXD < 15 && mVX < -0.1)
		mMovementState = 2;
	UpdatePetAnimations();
	double aYChange = 0.0;
	if (!mSleepy)
	{
		mXD += mVX / mSpeedMod;
		aYChange = mVY / mSpeedMod;
	}
	else
	{
		mXD += mVX / (mSpeedMod * 5.0);
		aYChange = mVY / (mSpeedMod * 5.0);
	}
	mYD += aYChange;
	Move(mXD, mYD);
}

void Sexy::OtherTypePet::Draw(Graphics* g)
{
	UpdateFishSongMgr();
	if (mOtherTypePetType == PET_NIKO || mOtherTypePetType == PET_CLYDE || 
		mOtherTypePetType == PET_RUFUS || mOtherTypePetType == PET_RHUBARB)
		DrawHelper(g, false);

	if (mOtherTypePetType == PET_STINKY)
	{
		if (mPetSpecialtyTimer > 0)
		{
			if (mPrevVX >= 0)
				DrawHelper(g, true);
			else
				DrawHelper(g, false);
		}
		else
		{
			if (mTurnAnimationTimer == 0)
			{
				bool drawn = false;
				if (mVX >= 0.0)
				{
					if ((int)mVX != 0 || mPrevVX >= 0.0)
					{
						if (mVX > 0.0)
							DrawHelper(g, true);
						if((int)mVX == 0 && mPrevVX > 0)
							DrawHelper(g, true);
						drawn = true;
					}
				}
				if(!drawn) 
					DrawHelper(g, false);
			}
			else if (mTurnAnimationTimer > 0)
				DrawHelper(g, true);
			else if (mTurnAnimationTimer < 0)
				DrawHelper(g, false);
		}
	}

	g->SetColorizeImages(false);
}

void Sexy::OtherTypePet::MouseDown(int x, int y, int theClickCount)
{
	bool prestoClicked = HandlePrestoClick(x, y, theClickCount);
	if (!prestoClicked && !gUnkBool07)
	{
		gUnkBool07 = true;
		mApp->mBoard->MouseDown(mX + x, mY + y, theClickCount);
		gUnkBool07 = false;
	}
}

void Sexy::OtherTypePet::PrestoMorph(int thePetId)
{
	if (thePetId == mOtherTypePetType || !PrestoRightClicked(mPrestoTimer))
		return;

	mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
	mApp->mBoard->SpawnPet(thePetId, mX, mY, true, false);
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();
}

void Sexy::OtherTypePet::Remove()
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();
}

void Sexy::OtherTypePet::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncDouble(mVX);
	theSync->SyncDouble(mVY);
	theSync->SyncDouble(mPrevVX);
	theSync->SyncDouble(mTargetVX);
	theSync->SyncDouble(mSpeedMod);
	theSync->SyncLong(mMovementState);
	theSync->SyncLong(mChaseEntityTimer);
	theSync->SyncLong(mMovementStateChangeTimer);
	theSync->SyncLong(mTurnAnimationTimer);
	theSync->SyncLong(mMovementAnimationTimer);
	theSync->SyncLong(mAnimationIndex);
	theSync->SyncLong(mPrestoTransformAnimTimer);
	theSync->SyncLong(mPrestoTimer);
	theSync->SyncLong(mPetSpecialtyTimer);
	theSync->SyncLong(m0x1b8);
	theSync->SyncLong(mPetAngryTimer);
	theSync->SyncBool(m0x1c0);
	theSync->SyncLong(mOtherTypePetType);
	theSync->SyncBool(mIsPresto);
}

void Sexy::OtherTypePet::UpdateNikoPosition(int theBgId)
{
	if (mOtherTypePetType == 1 && !mIsPresto)
	{
		double aX = 0.0;
		double aY = 0.0;
		if (theBgId == 1)
		{
			aX = 95;
			aY = 253;
		}
		else if (theBgId == 2)
		{
			aX = 175;
			aY = 163;
		}
		else if (theBgId == 3)
		{
			aX = 65;
			aY = 156;
		}
		else if (theBgId == 4)
		{
			aX = 145;
			aY = 260;
		}
		else if (theBgId == 5)
		{
			aX = 67;
			aY = 185;
		}
		else
		{
			aX = 160;
			aY = 176;
		}
		mXD = aX;
		mYD = aY;
		mX = (int)aX;
		mY = (int)aY;
	}
}

void Sexy::OtherTypePet::UpdatePetAnimations()
{
	if (mOtherTypePetType == PET_STINKY) // 141 - 208
	{
		if (mPrevVX < 0.0 && mVX > 0.0)
			mTurnAnimationTimer = -20;
		else if (mPrevVX > 0.0 && mVX < 0.0)
			mTurnAnimationTimer = 20;

		if (mTurnAnimationTimer > 0)
			mTurnAnimationTimer--;
		else if (mTurnAnimationTimer < 0)
			mTurnAnimationTimer++;

		if (mTurnAnimationTimer == 0)
		{
			if (abs(mVX) >= 0.3)
			{
				mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 20;
				mAnimationIndex = mMovementAnimationTimer / 2;
			}
			else
			{
				mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 40;
				mAnimationIndex = mMovementAnimationTimer / 4;
			}
		}
		else if (mTurnAnimationTimer > 0)
			mAnimationIndex = 9 - mTurnAnimationTimer / 2;
		else if(mTurnAnimationTimer < 0)
			mAnimationIndex = 9 + mTurnAnimationTimer / 2;

		if (mPetSpecialtyTimer > 0)
			mAnimationIndex = mPetSpecialtyTimer;

		if (mVX != mPrevVX && mVX != 0.0 && mPrevVX != 0.0 && mPetSpecialtyTimer == 0)
			mPrevVX = mVX;
	}
	else if (mOtherTypePetType == PET_NIKO)
	{
		mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 19;
	}
	else if (mOtherTypePetType == PET_CLYDE)
	{
		mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 40;
		if (mMovementAnimationTimer < 7)
			mAnimationIndex = mMovementAnimationTimer / 2;
		else if (mMovementAnimationTimer < 16)
			mAnimationIndex = 4;
		else
			mAnimationIndex = mMovementAnimationTimer / 4;
	}
	else if (mOtherTypePetType == PET_RUFUS)
	{
		if (mVX <= 0.01 && mVX >= -0.01 && mTargetVX == 0.0 && mApp->mBoard->mAlienList->empty())
		{
			mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 60;
			if (mMovementAnimationTimer < 3)
				mAnimationIndex = mMovementAnimationTimer / 2;
			else if(mMovementAnimationTimer < 24)
				mAnimationIndex = 2;
			else if(mMovementAnimationTimer < 33)
				mAnimationIndex = mMovementAnimationTimer / 2 - 10;
			else if(mMovementAnimationTimer < 54)
				mAnimationIndex = 7;
			else if(mMovementAnimationTimer < 60)
				mAnimationIndex = mMovementAnimationTimer / 2 - 20;
		}
		else
		{
			mMovementAnimationTimer %= 40;
			bool handleLogic = false;
			if (mVX > 0.01 || mVX < -0.01 || mTargetVX != 0.0 || mApp->mBoard->mCyraxPtr)
			{
				handleLogic = true;
			}
			else
			{
				if (GetEntityToChase() == nullptr) 
				{
					handleLogic = true;
				}
				else
				{
					mMovementAnimationTimer = (mMovementAnimationTimer + 4) % 40;
					mAnimationIndex = mMovementAnimationTimer / 4;
				}
			}

			if (handleLogic)
			{
				if (mVX < 1.0)
				{
					bool aFlag = false;
					if (mVX > 0.0)
					{
						mMovementAnimationTimer = ((mMovementAnimationTimer + 1) % 40);
					}
					else if (mVX > -1.0)
					{
						mMovementAnimationTimer--;
						aFlag = mMovementAnimationTimer < 0;
					}
					else
					{
						mMovementAnimationTimer -= 2;
						aFlag = mMovementAnimationTimer < 0;
					}

					if (aFlag)
						mMovementAnimationTimer += 40;

					mAnimationIndex = mMovementAnimationTimer / 4;
				}
				else
				{
					mMovementAnimationTimer = (mMovementAnimationTimer + 2) % 40;
					mAnimationIndex = mMovementAnimationTimer / 4;
				}
			}
		}
	}
	else if (mOtherTypePetType == PET_RHUBARB) // 87
	{
		if (mPetSpecialtyTimer > 0)
		{
			mPetSpecialtyTimer = mPetSpecialtyTimer - 1;
			mAnimationIndex = mPetSpecialtyTimer / 2;
			if (mPetSpecialtyTimer == 10)
				mApp->PlaySample(SOUND_BUTTONCLICK);
		}
		else
		{
			if (mVX >= 1.0)
			{
				mMovementAnimationTimer -= 2;
				if (mMovementAnimationTimer < 0)
					mMovementAnimationTimer += 40;
				mAnimationIndex = mMovementAnimationTimer / 4;
			}
			else if(mVX <= -1.0)
			{
				mMovementAnimationTimer = (mMovementAnimationTimer + 2) % 40;
				mAnimationIndex = mMovementAnimationTimer / 4;
			}
			else if (mVX > 0.0)
			{
				mMovementAnimationTimer--;
				if (mMovementAnimationTimer < 0)
					mMovementAnimationTimer += 40;
				mAnimationIndex = mMovementAnimationTimer / 4;
			}
			else
			{
				mMovementAnimationTimer = (mMovementAnimationTimer + 1) % 40;
				mAnimationIndex = mMovementAnimationTimer / 4;
			}
		}
	}

	if (mMisslePtr)
		UpdateCrosshairAnimation();
}

void Sexy::OtherTypePet::UpdatePetSpecialAnimations()
{
	if (mOtherTypePetType == PET_STINKY)
	{
		if (mApp->mBoard->mAlienList->empty() && mApp->mBoard->mBilaterusList->empty())
		{
			if (mPetSpecialtyTimer > 0)
				mPetSpecialtyTimer--;
		}
		else
		{
			if (mPetSpecialtyTimer < 9)
				mPetSpecialtyTimer++;
			mPetAngryTimer = 0;
		}
	}
	else if(mApp->mBoard->mTank != 5 && mOtherTypePetType == PET_NIKO)
	{
		mPetSpecialtyTimer++;
		if (mPetSpecialtyTimer == 1224)
			mApp->mBoard->PlaySample(SOUND_NIKOOPEN_ID, 3, 1.0);
		else if(mPetSpecialtyTimer == 1440)
			mApp->mBoard->PlaySample(SOUND_NIKOCLOSE_ID, 3, 1.0);
		else if (mPetSpecialtyTimer == 1233)
		{
			mApp->mBoard->DropCoin(mX + 1, mY - 2, Coin::COIN_NIKOPEARL, this, -1.0, 0);
			m0x1c0 = false;
			mApp->mBoard->SpawnBubble(mX + 11, mY + 5);
			mApp->mBoard->SpawnBubble(mX + 7, mY + 3);
		}
		else if (mPetSpecialtyTimer == 1450)
			mPetSpecialtyTimer = mApp->mSeed->Next() % 50;
	}
}

bool Sexy::OtherTypePet::ChaseEnemyBehavior()
{
	if (mOtherTypePetType == PET_STINKY || mOtherTypePetType == PET_CLYDE)
	{
		if (!mApp->mBoard->mCoinList1->empty())
		{
			if (mOtherTypePetType == PET_STINKY && mApp->mBoard->AliensInTank())
				return false;

			if (!mSleepy)
			{
				ChaseEntity();
				return true;
			}
		}
	}
	else if (mOtherTypePetType == PET_RUFUS)
	{
		if (mApp->mBoard->AliensInTank())
		{
			ChaseEntity();
			return true;
		}
	}
	else if (mOtherTypePetType == PET_RHUBARB)
	{
		if (ChaseEntity())
			return true;
	}
	return false;
}

bool Sexy::OtherTypePet::ChaseEntity()
{
	GameObject* anEntity = GetEntityToChase();

	if (mChaseEntityTimer > 4 && anEntity != nullptr)
	{
		mChaseEntityTimer = 0;
		if (mOtherTypePetType == PET_STINKY) // 15
		{
			int aX = anEntity->mX;
			double aCenterX = mXD + 40.0;

			if (aCenterX > aX + 48)
			{
				if (mVX > -2.3)
					mVX -= 1.0;
			}
			else if (aCenterX > aX + 40)
			{
				if (mVX > -1.3)
					mVX -= 0.5;
			}
			else if (aCenterX > aX + 36)
			{
				if (mVX > -0.3)
					mVX = 0.0;
			}
			else if (aCenterX < aX + 24)
			{
				if (mVX < 2.3)
					mVX += 1.0;
			}
			else if (aCenterX < aX + 32)
			{
				if (mVX < 1.3)
					mVX += 0.5;
			}
			else if (aCenterX < aX + 36)
			{
				if (mVX < 0.3)
					mVX = 0.0;
			}

			if (mApp->mBoard->mPetsInTank[PET_NIMBUS] != 0)
				mPetAngryTimer++;
		}
		else if (mOtherTypePetType == PET_CLYDE)
		{
			double aCenterX = mXD + 40.0;
			double aX = anEntity->mX + 36;
			if (aCenterX > aX)
			{
				if (mVX > -2.0)
					mVX -= 1.0;
			}
			else if (aCenterX < aX)
			{
				if (mVX < 2.0)
					mVX += 1.0;
			}
			double aCenterY = mYD + 40.0;
			double aY = anEntity->mY + 36;
			if (aCenterY > aY)
			{
				if (mVY > -2.0)
					mVY -= 1.0;
			}
			else if (aCenterY < aY)
			{
				if (mVY < 2.0)
					mVY += 1.0;
			}
		}
		else if (mOtherTypePetType == PET_RUFUS)
		{
			double aCenterX = mXD + 40.0;
			double aX = anEntity->mX + 40 + anEntity->mType != TYPE_BILATERUS ? 40 : 0;
			if (aCenterX > aX)
			{
				if (mVX > -5.0)
					mVX -= 1.8;
			}
			else if(aCenterX < aX)
			{
				if (mVX < 5.0)
					mVX += 1.8;
			}
		}
		else if (mOtherTypePetType == PET_RHUBARB)
		{
			double aCenterX = mXD + 40.0;
			double aX = anEntity->mX + 40;
			if (aCenterX > aX)
			{
				if (mVX > -5.0)
					mVX -= 1.8;
			}
			else if(aCenterX < aX)
			{
				if (mVX < 5.0)
					mVX += 1.8;
			}
		}
	}
	if (anEntity)
		CollideWithObject();
	return anEntity;
}

GameObject* Sexy::OtherTypePet::GetEntityToChase()
{
	Board* aBoard = mApp->mBoard;

	int aDist = 100000000;
	GameObject* aRetEntity = nullptr;

	if (mOtherTypePetType == PET_STINKY || mOtherTypePetType == PET_CLYDE)
	{
		bool pentaInTank = !aBoard->mPentaList->empty();
		for (int i = 0; i < aBoard->mCoinList1->size(); i++)
		{
			Coin* aCoin = aBoard->mCoinList1->at(i);

			if ((pentaInTank && aCoin->mCoinType != 10 && aCoin->mCoinType != 3) || !pentaInTank) // 104
			{
				if (aCoin->mCoinType != 17 && aCoin->mCoinType != 18 && !aCoin->m0x198)
				{
					if (mOtherTypePetType == PET_CLYDE)
						if (RelaxModeCheck(aCoin))
							continue;

					int aX = (mXD + 40.0) - aCoin->mX + 36;
					int aY = (mYD + 40.0) - aCoin->mY + 36;
					int aNewDist = aX * aX + aY * aY;

					if (aNewDist < aDist)
					{
						aRetEntity = aCoin;
						aDist = aNewDist;
					}
				}
			}
		}
	}
	else if (mOtherTypePetType == PET_RUFUS) // 159
	{
		for (int i = 0; i < aBoard->mBilaterusList->size(); i++)
		{
			Bilaterus* aBil = aBoard->mBilaterusList->at(i);

			int aX = (mXD + 40.0) - aBil->mX + 40;
			int aY = (mYD + 40.0) - aBil->mY + 40;
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist)
			{
				aRetEntity = aBil;
				aDist = aNewDist;
			}
		}
		for (int i = 0; i < aBoard->mAlienList->size(); i++)
		{
			Alien* anAlien = aBoard->mAlienList->at(i);

			int aX = (mXD + 40.0) - anAlien->mX + 80;
			int aY = (mYD + 40.0) - anAlien->mY + 80;
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && !anAlien->mIsPsychosquidHealing)
			{
				aRetEntity = anAlien;
				aDist = aNewDist;
			}
		}
	}
	else if (mOtherTypePetType == PET_RHUBARB) // 252
	{
		int aYClick = aBoard->AliensInTank() ? 250 : 260;

		for (int i = 0; i < aBoard->mFishList->size(); i++)
		{
			Fish* aFish = aBoard->mFishList->at(i);

			int aX = (mXD + 40.0) - (aFish->mX + 40);
			int aY = (mYD + 40.0) - (aFish->mY + 40);
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && aYClick < aFish->mY)
			{
				aRetEntity = aFish;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < aBoard->mOscarList->size(); i++)
		{
			Oscar* aFish = aBoard->mOscarList->at(i);

			int aX = (mXD + 40.0) - (aFish->mX + 40);
			int aY = (mYD + 40.0) - (aFish->mY + 40);
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && aYClick < aFish->mY)
			{
				aRetEntity = aFish;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < aBoard->mGekkoList->size(); i++)
		{
			Gekko* aFish = aBoard->mGekkoList->at(i);

			int aX = (mXD + 40.0) - (aFish->mX + 40);
			int aY = (mYD + 40.0) - (aFish->mY + 40);
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && aYClick < aFish->mY)
			{
				aRetEntity = aFish;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < aBoard->mBreederList->size(); i++)
		{
			Breeder* aFish = aBoard->mBreederList->at(i);

			int aX = (mXD + 40.0) - (aFish->mX + 40);
			int aY = (mYD + 40.0) - (aFish->mY + 40);
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && aYClick < aFish->mY)
			{
				aRetEntity = aFish;
				aDist = aNewDist;
			}
		}

		for (int i = 0; i < aBoard->mUltraList->size(); i++)
		{
			Ultra* aFish = aBoard->mUltraList->at(i);

			int aX = (mXD + 40.0) - (aFish->mX + 80);
			int aY = (mYD + 40.0) - (aFish->mY + 80);
			int aNewDist = aX * aX + aY * aY;

			if (aNewDist < aDist && aYClick - 40 < aFish->mY)
			{
				aRetEntity = aFish;
				aDist = aNewDist;
			}
		}
	}

	return aRetEntity;
}

void Sexy::OtherTypePet::DrawHelper(Graphics* g, bool mirror)
{
	switch (mOtherTypePetType)
	{
	case PET_STINKY:
		DrawStinky(g, mirror);
		break;
	case PET_NIKO:
		DrawNiko(g);
		break;
	case PET_CLYDE:
		DrawClyde(g, mirror);
		break;
	case PET_RUFUS:
	{
		Rect aSrcRect = Rect(0, 0, 0, 0);
		if (mVX > 0.01 || mVX < -0.01 || mTargetVX != 0.0)
			aSrcRect = Rect(mAnimationIndex * 80, 0, 80, 80);
		else
			aSrcRect = Rect(mAnimationIndex * 80, 80, 80, 80);
		g->DrawImage(IMAGE_RUFUS, 0, 0, aSrcRect);
		break;
	}
	case PET_RHUBARB:
	{
		Rect aSrcRect = Rect(0, 0, 0, 0);
		if (mPetSpecialtyTimer < 1)
			aSrcRect = Rect(mAnimationIndex * 80, 0, 80, 80);
		else
			aSrcRect = Rect(mAnimationIndex * 80, 80, 80, 80);
		g->DrawImage(IMAGE_RHUBARB, 0, 0, aSrcRect);
		break;
	}
	}

	if (mIsPresto && mPrestoTransformAnimTimer < 20)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColorizeImages(true);
		g->SetColor(Color(0xaf, 0xaf, 0xaf, 0xff));
		g->DrawImageMirror(IMAGE_PRESTO, 0, 0, Rect((mPrestoTransformAnimTimer / 2) * 80, 160, 80, 80), mirror);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		g->SetColorizeImages(false);
	}
	if (mCrosshairAnimationTimer != 0)
		DrawCrosshair(g, 0, 0);
	if (mIsPresto && mOtherTypePetType != PET_PRESTO)
		DrawPrestoMisc(g, mPrestoTimer);
}

void Sexy::OtherTypePet::DrawStinky(Graphics* g, bool mirror)
{
	if (mSleepy)
		g->DrawImage(IMAGE_ZZZ, mirror ? 55 : 25, -14);

	if (mPetAngryTimer > 150)
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(0xff, 0x4b, 0x4b));
	}

	int aRow = 0;
	if (mPetSpecialtyTimer != 0)
		aRow = 2;
	else
	{
		if (mTurnAnimationTimer != 0)
		{
			mirror = mTurnAnimationTimer > 0;
			aRow = 1;
		}
	}

	g->DrawImageMirror(IMAGE_STINKY, 0, 0, Rect(mAnimationIndex * 80, aRow * 80, 80, 80), mirror);
	g->SetColorizeImages(false);
}

void Sexy::OtherTypePet::DrawNiko(Graphics* g)
{
	int aCol = 0;
	int aRow = 0;
	if (mPetSpecialtyTimer < 1224)
	{
		int aVal = mMovementAnimationTimer - 9;
		aCol = abs(aVal);
		aRow = 0;
	}
	else if (mPetSpecialtyTimer < 1234)
	{
		aCol = (mPetSpecialtyTimer - 1224) % 10;
		aRow = 1;
	}
	else
	{
		if (mPetSpecialtyTimer < 1440 || mPetSpecialtyTimer > 1449)
			aCol = 9;
		else
			aCol = 9 - (mPetSpecialtyTimer - 1440) % 10;
		aRow = m0x1c0 + 1;
	}

	g->DrawImage(IMAGE_NIKO, 0, 0, Rect(aCol * 80, aRow * 80, 80, 80));

	if (!mIsPresto && mApp->mBoard->mCurrentBackgroundId == 3)
		g->DrawImage(IMAGE_NIKOTOWER, 4, 67);
}

void Sexy::OtherTypePet::DrawClyde(Graphics* g, bool mirror)
{
	if (mSleepy)
		g->DrawImage(IMAGE_ZZZ, mirror ? 30 : 50, -27);
	g->DrawImage(IMAGE_CLYDE, 0, 0, Rect(mAnimationIndex * 80, 0, 80, 80));
}

bool Sexy::OtherTypePet::HandlePrestoClick(int x, int y, int theClickCount)
{
	if (theClickCount > -1)
		return false;

	if (mIsPresto)
	{
		if (PrestoRightClicked(mPrestoTimer))
			mApp->OpenPrestoDialog(this);
		return true;
	}
	mApp->mBoard->CheckMouseDown(mX + x, mY + y);
	return true;
}

void Sexy::OtherTypePet::CollideWithObject()
{
	if (mOtherTypePetType == PET_STINKY || mOtherTypePetType == PET_CLYDE)
	{
		bool hasPenta = !mApp->mBoard->mPentaList->empty();
		for (int i = 0; i < mApp->mBoard->mCoinList1->size(); i++)
		{
			Coin* aCoin = mApp->mBoard->mCoinList1->at(i);

			if (!hasPenta || (hasPenta && aCoin->mCoinType != 3 && aCoin->mCoinType != 10))
			{
				if (!aCoin->m0x198 && aCoin->mCoinType != 18)
				{
					if (mOtherTypePetType == PET_CLYDE && RelaxModeCheck(aCoin))
						continue;

					if (mXD + 40.0 > aCoin->mX + 16 && mXD + 40.0 < aCoin->mX + 56 &&
						mYD + 40.0 > aCoin->mY + 16 && mYD + 40.0 < aCoin->mY + 56 && aCoin->mCoinType < 15)
					{
						aCoin->PetCollected();
						aCoin->Remove();
						mPetAngryTimer = 0;
						return;
					}
				}
			}
		}
	}
	else if (mOtherTypePetType == PET_RUFUS)
	{
		for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
		{
			Bilaterus* aBil = mApp->mBoard->mBilaterusList->at(i);

			if (abs((mXD + 40.0) - (aBil->mX + 40)) < 10.0 &&
				abs((mYD + 40.0) - (aBil->mY + 40)) < 10.0)
			{
				aBil->mActiveHead->m0x1d8 -= 2.0;
				mApp->mBoard->PlayPunchSound(10);
				mVX = 0.0;
				mTargetVX = 0.0;
			}
		}

		for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
		{
			Alien* anAlien = mApp->mBoard->mAlienList->at(i);

			if (mXD + 40.0 > anAlien->mX + 30 && mXD + 40.0 < anAlien->mX + 140 &&
				mYD + 40.0 > anAlien->mY + 10 && mYD + 40.0 < anAlien->mY + 150 && !anAlien->mIsPsychosquidHealing)
			{
				if (anAlien->mAlienType == Alien::ALIEN_DESTRUCTOR)
					anAlien->mHealth -= 0.25;
				else if(anAlien->mAlienType == Alien::ALIEN_ULYSEES)
					anAlien->mHealth -= 0.5;
				else if(anAlien->mAlienType == Alien::ALIEN_GUS)
					anAlien->mHealth -= 0.5;
				else
					anAlien->mHealth -= 2.0;

				mApp->mBoard->PlayPunchSound(10);
				mVX = 0.0;
				mTargetVX = 0.0;
			}
		}
	}
	else if (mOtherTypePetType == PET_RHUBARB)
	{
		double anYCap = mApp->mBoard->AliensInTank() ? 260 : 270;

		for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
		{
			Fish* aFish = mApp->mBoard->mFishList->at(i);
			if (mXD + 40.0 > aFish->mX && mXD + 40.0 < aFish->mX + 80 && anYCap < aFish->mY && mPetSpecialtyTimer == 5)
			{
				aFish->mBoughtTimer = 50;
				if (mApp->mSeed->Next() % 2 == 0)
					aFish->mVY = -20;
				else
					aFish->mVY = -30;
			}

			if (mXD + 40.0 < aFish->mX + 90 && mXD + 40.0 > aFish->mX - 10 && anYCap < aFish->mY && mPetSpecialtyTimer == 0)
				mPetSpecialtyTimer = 20;
		}
		for (int i = 0; i < mApp->mBoard->mOscarList->size(); i++)
		{
			Oscar* anOscar = mApp->mBoard->mOscarList->at(i);
			if (mXD + 40.0 > anOscar->mX && mXD + 40.0 < anOscar->mX + 80 && anYCap < anOscar->mY && mPetSpecialtyTimer == 5)
			{
				anOscar->mBoughtTimer = 50;
				if (mApp->mSeed->Next() % 2 == 0)
					anOscar->mVY = -20;
				else
					anOscar->mVY = -30;
			}

			if (mXD + 40.0 < anOscar->mX + 90 && mXD + 40.0 > anOscar->mX - 10 && anYCap < anOscar->mY && mPetSpecialtyTimer == 0)
				mPetSpecialtyTimer = 20;
		}
		for (int i = 0; i < mApp->mBoard->mGekkoList->size(); i++)
		{
			Gekko* aGekko = mApp->mBoard->mGekkoList->at(i);
			if (mXD + 40.0 > aGekko->mX && mXD + 40.0 < aGekko->mX + 80 && anYCap < aGekko->mY && mPetSpecialtyTimer == 5)
			{
				aGekko->mBoughtTimer = 50;
				if (mApp->mSeed->Next() % 2 == 0)
					aGekko->mVY = -20;
				else
					aGekko->mVY = -30;
			}

			if (mXD + 40.0 < aGekko->mX + 90 && mXD + 40.0 > aGekko->mX - 10 && anYCap < aGekko->mY && mPetSpecialtyTimer == 0)
				mPetSpecialtyTimer = 20;
		}
		for (int i = 0; i < mApp->mBoard->mBreederList->size(); i++)
		{
			Breeder* aBreeder = mApp->mBoard->mBreederList->at(i);
			if (mXD + 40.0 > aBreeder->mX && mXD + 40.0 < aBreeder->mX + 80 && anYCap < aBreeder->mY && mPetSpecialtyTimer == 5)
			{
				aBreeder->mBoughtTimer = 50;
				if (mApp->mSeed->Next() % 2 == 0)
					aBreeder->mVY = -20;
				else
					aBreeder->mVY = -30;
			}

			if (mXD + 40.0 < aBreeder->mX + 90 && mXD + 40.0 > aBreeder->mX - 10 && anYCap < aBreeder->mY && mPetSpecialtyTimer == 0)
				mPetSpecialtyTimer = 20;
		}
		for (int i = 0; i < mApp->mBoard->mUltraList->size(); i++)
		{
			Ultra* anUltra = mApp->mBoard->mUltraList->at(i);
			if (mXD + 40.0 > anUltra->mX && mXD + 40.0 < anUltra->mX + 160 && anYCap - 40 < anUltra->mY && mPetSpecialtyTimer == 5)
			{
				anUltra->mBoughtTimer = 50;
				if (mApp->mSeed->Next() % 2 == 0)
					anUltra->mVY = -20;
				else
					anUltra->mVY = -30;
			}

			if (mXD + 40.0 < anUltra->mX + 170 && mXD + 40.0 > anUltra->mX - 10 && anYCap - 40 < anUltra->mY && mPetSpecialtyTimer == 0)
				mPetSpecialtyTimer = 20;
		}
	}
}
