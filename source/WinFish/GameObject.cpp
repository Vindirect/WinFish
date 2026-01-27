#include "GameObject.h"
#include "WinFishApp.h"
#include "Board.h"
#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/MemoryImage.h>
#include <ctime>
#include "Food.h"
#include "Res.h"
#include "WinFishAppCommon.h"

using namespace Sexy;

const double PI = 3.141590118408203;

GameObject::GameObject()
{
	mApp = (WinFishApp*)gSexyAppBase;
	mCrosshairAnimationTimer = 0;
	mShadowPtr = NULL;
	mMisslePtr = NULL;
	mHunger = 0;
	mHungerShown = 0;
	mHungerAnimationTimer = 0;
	m0xd8 = 0;
	mHometownIdx = 0;
	mExoticDietFoodType = 0;
	mForwardlyChallenged = false;
	mInvisible = false;
	mInvisibleTimer = 0;
	mSpeedy = false;
	mSpeedySpeedState = 0;
	mVoracious = false;
	mVoraciousScreamCounter = 0;
	m0x10c = 0;
	mSinging = false;
	m0x114 = 0;
	m0x118 = 0;
	mType = TYPE_DEFAULT;
	mHungerShowTreshold = 300;
	mShown = true;
	mTodayBought = true;
	mVirtualTankId = -1;
	mSongId = -1;
	mLikes[0] = 0;
	mLikes[1] = 0;
	mLikes[2] = 0;
	m0x128 = 0;
	mStoreAnimationTimer = 0;
	mStoreAnimationIndex = 0;
	mCanBeEatenDelay = 0;
	m0x138 = 0;
	m0x140 = 0;
	mTimesFedToday = 0;
	m0x150 = 0;
	mPreNamedTypeId = -1;
	mMentalState = 3;
}

Sexy::GameObject::~GameObject()
{
	if (mApp->mBoard != nullptr && mSongId != -1)
		mApp->mBoard->StopFishSong(mSongId);
}

void Sexy::GameObject::Update()
{
	Widget::Update();
}

void Sexy::GameObject::UpdateF(float theFrac)
{
}

void Sexy::GameObject::Draw(Graphics* g)
{
}

void Sexy::GameObject::CountRequiredFood(int* theFoodReqPtr)
{
}

int Sexy::GameObject::SpecialReturnValue()
{
	return 0;
}

int Sexy::GameObject::GetShellPrice()
{
	if (mTodayBought && gUnkInt01 < 1)
	{
		__time64_t currentTime = time(NULL);

		const __time64_t LIFESPAN_SECONDS = 86400;

		if (currentTime >= mTimeBought + LIFESPAN_SECONDS)
			return -1;
	}

	return mShellPrice / 2;
}

void Sexy::GameObject::PrestoMorph(int thePetId)
{
}

void Sexy::GameObject::VFT74()
{
}

void GameObject::Remove()
{

}

void GameObject::SetPosition(double newX, double newY)
{
}

void GameObject::OnFoodAte(GameObject* obj)
{
}

void GameObject::UpdateStoreAnimation()
{
}

void GameObject::DrawStoreAnimation(Graphics* g, int justification)
{

}

void Sexy::GameObject::Sync(DataSync* theSync)
{
	theSync->RegisterPointer(this);
	theSync->SyncLong(mType);
	theSync->SyncLong(mCrosshairAnimationTimer);
	theSync->SyncPointer((void**)&mMisslePtr);
	theSync->SyncLong(mHunger);
	theSync->SyncLong(mHungerAnimationTimer);
	theSync->SyncLong(mHungerShowTreshold);
	theSync->SyncBool(mHungerShown);
	theSync->SyncByte(mCanBeEatenDelay);
	theSync->SyncLong(mX);
	theSync->SyncLong(mY);
	theSync->SyncLong(mWidth);
	theSync->SyncLong(mHeight);
	theSync->SyncBool(mVisible);
	theSync->SyncBool(mMouseVisible);
	theSync->SyncBool(mDisabled);
	theSync->SyncBool(mDoFinger);
	theSync->SyncLong(mVirtualTankId);
	if (mVirtualTankId > -1)
	{
		theSync->SyncString(mName);
		theSync->SyncBool(mShown);
		theSync->SyncBool(mTodayBought);
		if (theSync->mReader)
			mTimeBought = 0;
		theSync->SyncLong((int&)mTimeBought);
		theSync->SyncLong(mShellPrice);
		theSync->SyncLong(mHometownIdx);
		theSync->SyncByte(m0xd8);
		theSync->SyncLong(mExoticDietFoodType);
		theSync->SyncBool(mForwardlyChallenged);
		theSync->SyncBool(mSpeedy);
		theSync->SyncLong(mSpeedySpeedState);
		theSync->SyncBool(mInvisible);
		theSync->SyncLong(mInvisibleTimer);
		theSync->SyncBool(mVoracious);
		theSync->SyncLong(mVoraciousScreamCounter);
		theSync->SyncLong(m0x10c);
		theSync->SyncBool(mSinging);
		theSync->SyncLong(m0x114);
		theSync->SyncLong(m0x118);
		theSync->SyncLong(mSongId);
		if (theSync->mReader)
			m0x138 = 0;
		theSync->SyncLong((int&)m0x138);
		if (theSync->mReader)
			m0x140 = 0;
		theSync->SyncLong((int&)m0x140);
		theSync->SyncLong(mTimesFedToday);
		theSync->SyncLong(mMentalState);
		theSync->SyncLong(m0x150);
		theSync->SyncLong(mPreNamedTypeId);
		//theSync->SyncBytes((void*) mPreNamedTypeId, 4);
		for (int i = 0; i < 3; i++)
			theSync->SyncLong(mLikes[i]);
	}
}

void GameObject::UpdateCounters()
{
	Widget::Update();
	if (mVoraciousScreamCounter != 0)
		mVoraciousScreamCounter--;
	if (m0x10c != 0)
		m0x10c--;
	if (mSpeedySpeedState != 0)
		mSpeedySpeedState--;

	if (m0x114 > 0) // Something about music
	{
		ShowInvisibility();
		if (mSongId == -1)
		{
			m0x114--;
			if (m0x114 == 0)
			{ 
				mSongId = mApp->mBoard->PlayFishSong(mType, mPreNamedTypeId);
				if (mSongId != -1)
					m0x114 = 1;
			}
		}
		else
		{
			m0x114++;
			bool aFinished = mApp->mBoard->IsSongPlaying(mSongId);
			if (!aFinished)
			{
				m0x114 = 0;
				mSongId = -1;
			}
			else if (m0x114 == 2 || m0x114 % 30 == 0)
				mApp->mBoard->MakeNote(mX + mWidth / 2 - 20, mY - 50, 1, ""); // Makes the note object going upwards
		}
	}

	if (mCanBeEatenDelay != 0)
		mCanBeEatenDelay--;
}

bool Sexy::GameObject::CoinDropTimerPassed(int theCurrentTime, int theToPassTime)
{
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return theToPassTime <= theCurrentTime;

	if (mVirtualTankId <= -1)
		return false;

	if (!mApp->IsScreenSaver() || mApp->mBoard->m0x400 <= gUnkInt07)
	{
		if (theToPassTime > 360)
		{
			theToPassTime += mMentalState * -540;
			if (theToPassTime < 360) theToPassTime = 360;
		}

		if (mApp->mBoard->m0x500)
		{
			if (mHunger < 0)
			{
				if (theToPassTime < theCurrentTime)
					theCurrentTime = theToPassTime - 90;
				return false;
			}
			return theToPassTime <= theCurrentTime;
		}
		if (theToPassTime < theCurrentTime) theCurrentTime = 0;
	}
	return false;
}

bool Sexy::GameObject::RelaxModeCanDrop()
{
	if (mApp->mRelaxMode && mHunger < 1)
		return false;
	return true;
}

void Sexy::GameObject::UpdateCrosshairAnimation()
{
	mCrosshairAnimationTimer = (mCrosshairAnimationTimer + 1) % 20;
}

void GameObject::ShowInvisibility()
{
	if (mInvisible)
	{
		if (mInvisibleTimer == 0)
			mInvisibleTimer = 1;
		else if (mInvisibleTimer > 10)
			mInvisibleTimer = 10;
	}
}

void Sexy::GameObject::UpdateInvisible()
{
	if (mInvisibleTimer > 0 && ++mInvisibleTimer > 80)
		mInvisibleTimer = 0;
}

void GameObject::UpdateHungerAnimCounter()
{
	if (mHungerAnimationTimer > 0)
	{
		if (!mHungerShown)
			mHungerAnimationTimer--;
		else
		{
			mHungerAnimationTimer++;
			if (mHungerAnimationTimer > 5)
				mHungerAnimationTimer = 0;
		}
	}
}

void GameObject::UpdateHungerCounter()
{
	if (mSongId == -1)
		mHunger--;
	if (mHunger == mHungerShowTreshold + 4)
		UpdateHungerState(true);
	else if (mHunger < -1000)
		mHunger = -1000;
}

void GameObject::UpdateHungerState(bool theHungryShown)
{
	if (!mApp->IsScreenSaver())
	{
		mHungerShown = theHungryShown;
		if (!mHungerShown)
			mHungerAnimationTimer = 5;
		else if (mTimesFedToday < 3 || mApp->mBoard->mAlwaysShowWhenHungry)
			mHungerAnimationTimer = 1;
	}
}

bool GameObject::ShouldDie()
{
	if (mVirtualTankId > -1)
		return false;
	if (mApp->mRelaxMode)
	{
		if (mType != TYPE_GEKKO && mType != TYPE_PENTA && mType != TYPE_GRUBBER)
			return mHunger < -2159;
		return mHunger < -4319;
	}
	return mHunger < 1;
}

int GameObject::ExoticFoodCollision(int theCenterX, int theCenterY)
{
	if (mExoticDietFoodType > 999)
		return CarnivorousExoticFoodCollision(theCenterX, theCenterY, mExoticDietFoodType - Food::EXO_FOOD_OBJECTS_START);

	int aFoodTypeToEat = mExoticDietFoodType;	
	if (mExoticDietFoodType == 6)
		aFoodTypeToEat = 0;

	Board* aBoard = mApp->mBoard;

	int aReturnValue = 0;

	for (size_t i = 0; i < aBoard->mFoodList->size(); ++i)
	{
		Food* aTargetFood = aBoard->mFoodList->at(i);

		if (aTargetFood->mExoticFoodType == aFoodTypeToEat && !aTargetFood->mPickedUp && aTargetFood->mCantEatTimer == 0)
		{
			int aDeltaX = aTargetFood->mX - theCenterX;
			int aDeltaY = aTargetFood->mY - theCenterY;

			int aModifiedDeltaX = aDeltaX + 20;
			int aModifiedDeltaY = aDeltaY + 20;

			if (aModifiedDeltaX + 14 < 29 && aModifiedDeltaY > -20 && aModifiedDeltaY < 20)
			{
				OnFoodAte(aTargetFood);
				aTargetFood->Remove();
				return 1;
			}

			if (aModifiedDeltaX + 29 < 59 && aModifiedDeltaY + 24 < 49)
				aReturnValue = 2;
		}
	}

	return aReturnValue;
}

int Sexy::GameObject::CarnivorousExoticFoodCollision(int theCenterX, int theCenterY, int theID)
{
	Board* aBoard = mApp->mBoard;
	GameObjectSet::iterator anIterator;
	int aRetVal = 0;

	for (anIterator = aBoard->mGameObjectSet.begin(); anIterator != aBoard->mGameObjectSet.end(); ++anIterator)
	{
		GameObject* aTargetObject = *anIterator;

		if ((aTargetObject->mVirtualTankId < 0) &&
			(aTargetObject->mCanBeEatenDelay < 1) &&
			(aTargetObject->mType == theID) &&
			(aTargetObject != this))
		{
			Fish* aTargetFish = (Fish*)aTargetObject;

			int aHalfFWidth = aTargetFish->mWidth / 2;
			int aHalfFHeight = aTargetFish->mHeight / 2;

			int val1 = theCenterX - aHalfFWidth - aTargetFish->mX;
			int val2 = theCenterY - aHalfFHeight - aTargetFish->mY;

			if ((val1 > -aHalfFWidth) && (val1 < aHalfFWidth) &&
				(val2 > -aHalfFHeight) && (val2 < aHalfFHeight))
			{
				OnFoodAte(aTargetFish);
				aTargetFish->Remove();
				return 1;
			}

			int anExtendedHalfFWidth = aHalfFWidth + 20;
			int anExtendedHalfFHeight = aHalfFHeight + 20;

			if ((val1 > -anExtendedHalfFWidth) && (val1 < anExtendedHalfFWidth) &&
				(val2 > -anExtendedHalfFHeight) && (val2 < anExtendedHalfFHeight))
				aRetVal = 2;
		}
	}

	return aRetVal;
}

void Sexy::GameObject::RemoveHelper02(bool safeDelete)
{
	if (mApp->mBoard->RemoveGameObjectFromLists(this, true))
	{
		if (mMisslePtr)
		{
			mMisslePtr->RemoveHelper02(true);
			mMisslePtr = nullptr;
		}
		if (mShadowPtr)
		{
			mShadowPtr->RemoveHelper02(true);
			mShadowPtr = nullptr;
		}
		VFT74();
		mApp->mBoard->mWidgetManager->RemoveWidget(this);
		if (safeDelete)
			mApp->SafeDeleteWidget(this);
	}
}

void Sexy::GameObject::UpdateFishSongMgr()
{
	if (gUnkBool02)
		mApp->mBoard->FishSongMgrUpdate();
}

void Sexy::GameObject::Unk02(bool flag)
{
	mSpeedySpeedState = 0;
	mApp->mBoard->mShouldSave = true;
	if (!flag)
	{
		if ((mHungerAnimationTimer > -1 || mApp->mRelaxMode) && mHunger < 300)
			mHunger = 300;

		if (mSinging)
		{
			int aPrevVal = m0x118;
			int unk = aPrevVal / 5;
			m0x118++;
			if (aPrevVal % 5 == 0 && mPreNamedTypeId != KILGORE && m0x114 == 0)
			{
				int aVal = m0x10c;
				if (m0x10c < 5)
					aVal = 5;
				m0x114 = aVal;
			}
		}
	}
}

void Sexy::GameObject::Unk03(long long theTodayInSec, __time64_t theCurTime)
{
	if (mVirtualTankId < 0)
		return;

	if (mTodayBought)
	{
		const time_t ONE_DAY_IN_SECONDS = 86400;
		if (mTimeBought + ONE_DAY_IN_SECONDS <= theCurTime)
			mTodayBought = false;
	}

	if (theTodayInSec != m0x138)
	{
		long long daysPassed = (theTodayInSec - m0x140) - 1;
		if (daysPassed < 0)
		{
			m0x140 = theTodayInSec;
			daysPassed = 0;
		}

		mMentalState -= daysPassed;
		mTimesFedToday = 0;
		m0x138 = theTodayInSec;
		if (mMentalState < 0)
			mMentalState = 0;
	}
}

bool Sexy::GameObject::UpdateMentalState()
{
	__time64_t aCurTime = _time64(NULL);
	long long aTime = GetTodayStartSeconds();

	Unk03(aTime, aCurTime);
	
	if (mTimesFedToday < 3)
	{
		mTimesFedToday++;
		if (mTimesFedToday == 3)
		{
			mMentalState++;
			m0x140 = aTime;
			if (mMentalState > 6)
				mMentalState = 6;
		}
		return true;
	}
	return false;
}

void Sexy::GameObject::DrawName(Graphics* g, bool flag)
{
	if (mName.size() > 0 && mApp->mBoard->m0x4fc && (!mInvisible || mInvisibleTimer != 0 || flag))
	{
		int aVal = SpecialReturnValue();
		if (IsHungryVisible())
			g->SetColor(Color(0xd6cf29));
		else g->SetColor(Color::White);
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		int aStrWdth = g->GetFont()->StringWidth(mName);
		g->DrawString(mName, (mWidth - aStrWdth) / 2, mHeight - 2 + aVal);
		if (mHungerAnimationTimer != 0)
		{
			Color aCol = Color(0xd6cf29);
			aCol.mAlpha = mHungerAnimationTimer * 255 / 5;
			g->SetColor(aCol);
			g->DrawString(mName, (mWidth - aStrWdth) / 2, mHeight - 2 + aVal);
		}
	}
}

void Sexy::GameObject::DrawCrosshair(Graphics* g, int theX, int theY)
{
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->DrawImageCel(IMAGE_CROSSHAIR, theX, theY, (mCrosshairAnimationTimer / 4)); // Might be incorrect
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

bool Sexy::GameObject::IsHungryVisible()
{
	if (mApp->IsScreenSaver())
		return false;
	if (mHunger < 301 && mHungerAnimationTimer == 0 && (mTimesFedToday < 3 || mApp->mBoard->mAlwaysShowWhenHungry))
		return true;
	return false;
}

GameObject* Sexy::GameObject::FindNearestExoticFood(int theX, int theY)
{
	if (mExoticDietFoodType > 999)
		return FindNearestExoticFoodOther(theX, theY, mExoticDietFoodType - Food::EXO_FOOD_OBJECTS_START);

	int aDist = 100000000;
	Food* aRet = nullptr;

	for (int i = 0; i < mApp->mBoard->mFoodList->size(); i++)
	{
		Food* aFood = mApp->mBoard->mFoodList->at(i);

		if (aFood->mExoticFoodType == mExoticDietFoodType && !aFood->mPickedUp && aFood->mCantEatTimer == 0)
		{
			int ax = aFood->mX - theX + 20;
			int ay = aFood->mY - theY + 20;
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = aFood;
				aDist = aNewDist;
			}
		}
	}

	if (mType == TYPE_ULTRA && aDist < 40000)
		VoraciousScream(175);
	if (aDist < 10000)
	{
		VoraciousScream(150);
		mSpeedySpeedState = 100;
	}
	return aRet;
}

GameObject* Sexy::GameObject::FindNearestExoticFoodOther(int theX, int theY, int theObjType)
{
	int aDist = 100000000;
	GameObject* aRet = nullptr;

	for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;

		if (anObj->mVirtualTankId < 0 && anObj->mCanBeEatenDelay < 1 && anObj->mType == theObjType)
		{
			int ax = theX - anObj->mWidth / 2 - anObj->mX;
			int ay = theY - anObj->mHeight / 2 - anObj->mY;
			int aNewDist = ax * ax + ay * ay;

			if (aNewDist < aDist)
			{
				aRet = anObj;
				aDist = aNewDist;
			}
		}
	}

	if(mType == TYPE_ULTRA && aDist < 40000)
		VoraciousScream(175);
	else if(mType == TYPE_ULTRA && aDist < 22500)
		VoraciousScream(150);

	if (aDist < 10000)
	{
		VoraciousScream(150);
		mSpeedySpeedState = 100;
	}

	return aRet;
}

void Sexy::GameObject::VoraciousScream(int theNum)
{
	if (mVoracious && ((mPreNamedTypeId != KILGORE || mSongId != -1) && mVoraciousScreamCounter == 0))
	{
		mApp->PlaySample(SOUND_PRIMALSCREAM);
		mVoraciousScreamCounter = theNum;
		m0x10c = 80;
	}
}

bool Sexy::GameObject::IsHungryBlipPointer(int theTreshold)
{
	if (mHunger < theTreshold && mApp->mBoard->mPetsInTank[13] != 0 && (mTimesFedToday < 3 || mApp->mBoard->mAlwaysShowWhenHungry))
		return true;
	return false;
}

void Sexy::GameObject::SetColorHelper(Graphics* g, Color& theColor)
{
	g->SetColorizeImages(true);
	if (!mInvisible)
	{
		g->SetColor(theColor);
		return;
	}
	int anAlphaVal = 0;
	if (mInvisibleTimer < 10)
		anAlphaVal = (mInvisibleTimer * 255) / 10;
	else if (mInvisibleTimer < 50)
		anAlphaVal = 255;
	else
		anAlphaVal = ((80 - mInvisibleTimer) * 255) / 30;
	if (anAlphaVal < 0)
		anAlphaVal = 0;

	Color aNewColor = Color(theColor.mRed, theColor.mGreen, theColor.mBlue, (theColor.mAlpha * anAlphaVal) / 255);
	g->SetColor(aNewColor);
}

void Sexy::GameObject::UpdateHungerStateIfWasHungry(bool wasHungry)
{
	if (wasHungry)
		if (!IsHungryVisible())
			UpdateHungerState(false);
}



bool Sexy::GameObject::DrawInvisibleEffect(Graphics* g, Image* theImage, Rect& theSrcRect, bool mirror)
{
	if (mApp->mBoard->mCurrentBackgroundId - 1 < 6)
	{
		Image* anImg = GetImageById(mApp->mBoard->mCurrentBackgroundId + IMAGE_MISSILE_ID);
		DrawInvisibleEffectHelper(g, (MemoryImage*) anImg, mX - 2, mY - 2, (MemoryImage*) theImage, theSrcRect, mirror);
	}
	return mInvisibleTimer == 0;
}

void Sexy::GameObject::DrawInvisibleEffectHelper(Graphics* g, MemoryImage* theBGImage, int theX, int theY, MemoryImage* theObjImage, Rect& theSrcRect, bool mirror)
{
	MemoryImage anInvisImg = MemoryImage(gSexyApp);
	anInvisImg.Create(theSrcRect.mWidth, theSrcRect.mHeight);
	anInvisImg.SetImageMode(true, true);

	double aStartVal = mUpdateCnt * 5.0;

	for (int y = 0; y < anInvisImg.mHeight; ++y)
	{
		double aVal = aStartVal + y * 5.0;
		float aValInRad = (float) (aVal * PI / 180.0);
		float aSinVal = sinf(aValInRad); //44

		int anAbsY = theY + y;
		int aSampleY = aSinVal + anAbsY;

		if (aSampleY < 0 || aSampleY >= theBGImage->mHeight) // 49 No nesting.
			continue;

		ulong* aBGBits = theBGImage->GetBits();
		ulong* aBGBitForEffect = aBGBits + theBGImage->mWidth * aSampleY + theX;

		ulong* anInvisBits = anInvisImg.GetBits();
		ulong* anInvisBitForEffect = anInvisBits + y * anInvisImg.mWidth;

		ulong* anObjBits = theObjImage->GetBits();
		ulong anObjBitPos = (theSrcRect.mY + y) * theObjImage->mWidth + theSrcRect.mX;

		if (!mirror) // 56
		{
			ulong* anObjBitForEffect = anObjBits + anObjBitPos;
			int aCnt2 = theX;
			for (int x = 0; x < anInvisImg.mWidth; ++x)
			{
				if (aCnt2 >= 0 && aCnt2 < theBGImage->mWidth)
				{
					ulong anBitAlpha = *anObjBitForEffect & 0xFF000000;
					anObjBitForEffect++;
					if (anBitAlpha != 0)
					{
						anBitAlpha = *aBGBitForEffect;
						*anInvisBitForEffect = anBitAlpha;
					}
					anInvisBitForEffect++;
					aBGBitForEffect++;
				}
				aCnt2++;
			}
		}
		else // 77
		{
			ulong* anObjBitForEffect = anObjBits - 4 + anObjBitPos + theSrcRect.mWidth;
			int aCnt2 = theX;
			for (int x = 0; x < anInvisImg.mWidth; ++x)
			{
				if (aCnt2 >= 0 && aCnt2 < theBGImage->mWidth)
				{
					ulong anBitAlpha = *anObjBitForEffect & 0xFF000000;
					anObjBitForEffect--;
					if (anBitAlpha != 0)
					{
						anBitAlpha = *aBGBitForEffect;
						*anInvisBitForEffect = anBitAlpha;
					}
					anInvisBitForEffect++;
					aBGBitForEffect++;
				}
				aCnt2++;
			}
		}
	}

	g->DrawImage(&anInvisImg, 0, 0);
}

bool Sexy::GameObject::PrestoRightClicked(int theTimer)
{
	if (theTimer < 1)
		return true;

	mApp->mBoard->PlaySample(SOUND_BUZZER_ID, 3, 1.0);

	mApp->mBoard->MakeNote(mX + mWidth / 2 - 20, mY - 10, 2, "Recharging...");
	return false;
}

void Sexy::GameObject::DrawPrestoMisc(Graphics* g, int theTimer)
{
	g->SetColor(Color(0xaaffaa));
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	int aStrWdth = g->GetFont()->StringWidth("Presto");
	int aX = (mWidth - aStrWdth) / 2;
	g->DrawString("Presto", aX, mHeight - 2);

	if (theTimer > 0)
	{
		Graphics g2(*g);

		g2.SetColor(Color(0xff3333));
		g2.ClipRect(aStrWdth - (aStrWdth * theTimer) / 360 + aX, 0, mWidth, mHeight);

		g2.DrawString("Presto", aX, mHeight - 2);
	}
}

void Sexy::GameObject::DeterminePetSleepy(bool* sleepy)
{
	if (!mApp->IsScreenSaver())
	{
		if (mWidgetManager->mUpdateCnt - mWidgetManager->mLastInputUpdateCnt < 6480)
			*sleepy = false;
		else if (*sleepy == false && (Rand() / 60) % 60 == 0)
			*sleepy = true;
	}
}

SexyString Sexy::GameObject::GetStoreDescription()
{
	if (mName.length() != 0)
	{
		if (mPreNamedTypeId == COOKIE)
		{
			// TODO HEre 0051b7bf some string global
			return "This is Cookie.\nCookie will feed\nyour more exotic fish.";
		}

		if (mName[mName.length() - 1] == '.')
			return StrFormat("This is %s", mName.c_str());
		else
			return StrFormat("This is %s.", mName.c_str());
	}

	switch (GetAttribute())
	{
	case 0:
		return "You\'ve never seen\na fish like this!";
	case 1:
		return "This fish is a\nvoracious eater.";
	case 2:
		return "This fish goes\nfrom 0 to 60\nin 1.3 seconds!";
	case 3:
		return "This fish is\na musical genius!";
	case 4:
		return "This fish is\n\"forwardly challenged\"\nbut it\'s great otherwise!";
	case 5:
		return "This fish has\ndeveloped a taste\nfor exotic food!";
	case 6:
		return "This fish has a\nvery special diet.";
	case 7:
	case 8:
		return "This fish is extremely \nrare.  We hardly ever\nget them in stock.";
	}

	switch (mType)
	{
	case TYPE_GUPPY:
	{
		Fish* aFish = (Fish*)this;
		if (aFish->mHasSpecialColors)
			return "These adorable little\nfish are a mainstay\nof any aquarium!";
		if (aFish->mRainbowFish)
			return "Choose from a dazzling\narray of colors.  C\'mon!\nyou know you want one!";
		return "This one\'s pretty\ncool, isn\'t it?";
	}
	case TYPE_OSCAR:
	{
		Oscar* aFish = (Oscar*)this;
		if (aFish->mHasSpecialColors && aFish->mRainbowFish)
			return "This one\'s pretty\ncool, isn\'t it?";
		return "Specially trained not\nto eat store-bought\nguppies.  I promise!";
	}
	case TYPE_ULTRA:
		return "Many regard this\nto be the\nSUV of fish!";
	case TYPE_PENTA:
		return "Isn\'t this little guy\njust the cutest?\nAnswer: Yes.";
	case TYPE_GRUBBER:
		return "It won\'t eat\nyour guppies, but\nwatch your fingers!";
	case TYPE_BREEDER:
		return "Take care of this fish,\nand she\'ll give you a\nbaby fish at\nno extra charge!";
	case TYPE_SYLVESTER_FISH:
		return "Don\'t worry.  This\n\"fish\" has been\ndomesticated.";
	case TYPE_BALL_FISH:
		return "Some people think\nthis fish looks\nlike a ball.";
	case TYPE_BI_FISH:
		return "This \"fish\" may look\ndead, but he is actually\nquite lively!";
	default:
		return "All of our fish come\nwith a free plastic bag!\nTake one home today!";
	}
	return SexyString();
}

int Sexy::GameObject::GetAttribute()
{
	if (mPreNamedTypeId != -1)
		return 8;

	int anAttrib = 9;
	int numOfAttrib = mInvisible;
	if (mInvisible)
		anAttrib = 0;
	if (mVoracious)
	{
		anAttrib = 1;
		numOfAttrib++;
	}
	if (mSpeedy)
	{
		anAttrib = 2;
		numOfAttrib++;
	}
	if (mSinging)
	{
		anAttrib = 3;
		numOfAttrib++;
	}
	if (mForwardlyChallenged)
	{
		anAttrib = 4;
		numOfAttrib++;
	}
	if (mExoticDietFoodType == 3 || mExoticDietFoodType == 4 || mExoticDietFoodType == 5)
	{
		anAttrib = 5;
		numOfAttrib++;
	}
	if (mExoticDietFoodType == Food::EXO_FOOD_GUPPY || mExoticDietFoodType == Food::EXO_FOOD_OSCAR || mExoticDietFoodType == Food::EXO_FOOD_ULTRA)
	{
		anAttrib = 6;
		numOfAttrib++;
	}
	if (numOfAttrib > 1)
		anAttrib = 7;
	return anAttrib;
}

int Sexy::GameObject::GetShellCost()
{
	int aCost = 100;
	switch (mType)
	{
	case TYPE_GUPPY:
	{
		Fish* aFish = (Fish*)this;
		if (!aFish->mHasSpecialColors)
			aCost = 25;
		else
			aCost = aFish->mRainbowFish ? 2500 : 500;
		break;
	}
	case TYPE_OSCAR:
	{
		Oscar* aFish = (Oscar*)this;
		if (aFish->mHasSpecialColors)
			aCost = aFish->mRainbowFish ? 5000 : 1000;
		break;
	}
	case TYPE_ULTRA:
	case TYPE_BI_FISH:
		aCost = 10000;
		break;
	case TYPE_GEKKO:
		aCost = 3500;
		break;
	case TYPE_PENTA:
	case TYPE_GRUBBER:
		aCost = 2500;
		break;
	case TYPE_BREEDER:
		aCost = 5000;
		break;
	case TYPE_SYLVESTER_FISH:
		aCost = 15000;
		break;
	case TYPE_BALL_FISH:
		aCost = 20000;
		break;
	}

	int anAttrib = GetAttribute();
	if (anAttrib != 9)
		aCost += 5000;

	switch (anAttrib)
	{
	case 0:
	case 3:
	case 6:
		return 30000;
	case 1:
	case 2:
	case 4:
	case 5:
		return 25000;
	case 7:
		return 40000;
	case 8:
		return 50000;	
	default:
		aCost = (aCost / 5) * 5;
		if (aCost <= 0)
			aCost = 10;
		break;
	}
	return aCost;
}

void Sexy::GameObject::SetStoreColor(Graphics* g, Color& theColor)
{
	g->SetColorizeImages(true);
	if (!mInvisible)
	{
		g->SetColor(theColor);
	}
	else
	{
		int anAlphaMul = m0x128 % 300;
		if (anAlphaMul > 150)
			anAlphaMul = 300 - anAlphaMul;
		g->SetColor(Color(theColor.mRed, theColor.mGreen, theColor.mBlue, (theColor.mAlpha * anAlphaMul) / 600 + 10));
	}
}

void Sexy::GameObject::ResetSpecialProperties()
{
	mExoticDietFoodType = 0;
	mVoracious = false;
	mForwardlyChallenged = false;
	mInvisible = false;
	mSinging = false;
	mSpeedy = false;
}

void Sexy::GameObject::SomeBoughtFunc()
{
	long long secs = GetTodayStartSeconds();
	m0x138 = secs;
	m0x140 = secs - 1;
	mMentalState = 3;
	m0x150 = Rand() % 1000;
	m0xd8 = Rand() % 12;

	mHometownIdx = (Rand() % 374) + 1;
	int* colors = (int*)&mLikes;

	for (int i = 0; i < 3; i++)
	{
		while (true)
		{
			int rndColor = Rand() % 331;
			colors[i] = rndColor;

			bool isDuplicate = false;
			if (i > 0)
			{
				for (int j = 0; j < i; j++)
				{
					if (colors[j] == rndColor)
					{
						isDuplicate = true;
						break;
					}
				}
			}

			if (!isDuplicate)
				break;
		}
	}
}

bool Sexy::GameObject::CanDropCoin()
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		if (mApp->mBoard->m0x500)
		{
			if (!mApp->IsScreenSaver() || mApp->mBoard->m0x400 <= gUnkInt07)
				return true;
		}
		return false;
	}
	return true;
}

int Sexy::GameObject::DetermineCoinDropT(int supposedTime)
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return Rand() % 200 + 4320;
	return supposedTime;
}

void Sexy::GameObject::CopyBreederDataVT(Breeder* theBreeder)
{
	mName = theBreeder->mName;
	mTimeBought = theBreeder->mTimeBought;
	mShellPrice = theBreeder->mShellPrice;
	m0xd8 = theBreeder->m0xd8;
	mHometownIdx = theBreeder->mHometownIdx;
	mLikes[0] = theBreeder->mLikes[0];
	mLikes[1] = theBreeder->mLikes[1];
	mLikes[2] = theBreeder->mLikes[2];
	mExoticDietFoodType = theBreeder->mExoticDietFoodType;
	mForwardlyChallenged = theBreeder->mForwardlyChallenged;
	mSpeedy = theBreeder->mSpeedy;
	mInvisible = theBreeder->mInvisible;
	mVoracious = theBreeder->mVoracious;
	mSinging = theBreeder->mSinging;
}

bool Sexy::GameObject::RelaxModeCheck(Coin* theCoin)
{
	if (mApp->mRelaxMode)
		if (!mApp->mBoard->mPentaList->empty() && (theCoin->mCoinType == 2 || theCoin->mCoinType == 1))
			return true;
	return false;
}
