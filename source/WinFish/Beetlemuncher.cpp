//#include "Beetlemuncher.h"
//#include "Res.h"
//
//
//#include "SexyAppFramework/SoundManager.h"
//#include "SexyAppFramework/SoundInstance.h"
//using namespace Sexy;
//
//extern bool mBlipInTank;
//
//Beetlemuncher::Beetlemuncher(Board* theAdvScr, WinFishApp* theApp) : Guppy(theApp)
//{
//	mIsGuppy = false;
//	mID = -1;
//	mFishType = FISH_TYPE_BEETLEMUNCHER;
//}
//
//Beetlemuncher::~Beetlemuncher()
//{
//}
//
//void Beetlemuncher::Update()
//{
//	if (mHidden)
//		return;
//	Fish::Update();
//	if (mTimer >= 0)
//		++mTimer;
//
//	double prevVX = mVX;
//
//	if (mState == STATE_BOUGHT)
//	{
//		if (mTimer % 2 == 0)
//		{
//			Bubble b;
//			b.mTimer = Rand(4);
//			b.mX = mX + Rand(70);
//			b.mY = mY + Rand(45);
//			b.mSize = Rand(3);
//			b.mSpeed = Rand(1.0f) + 2.0;
//			b.mShake = Rand(0.2f) + 1.0;
//			//mTheApp->mBubble.push_back(b);
//		}
//
//		int randomy = Rand(30);
//		if (mY < randomy + mYMin + 30 && mTimer < 40)
//			mY += ((randomy + mYMin + 30 - mY) / 12);
//		else if (mTimer == 40)
//			mState = STATE_NONE;
//	}
//
//	// Normal Swimming Logic
//	if (!FishHungry() && mState != STATE_DYING && mState != STATE_REVIVAL && mState != STATE_BOUGHT) // If Not Hungry
//	{
//		Guppy::FishNormalBehavior();
//	} // End of normal swimming logic
//
//
//	++mFishMovementChangeTimer;
//	++mMovementChangeTimerMod;
//	if (mState != STATE_DYING && mState != STATE_REVIVAL)
//	{
//		if (mMovementChangeTimerMod > 20) {
//			mMovementChangeTimerMod = 0;
//			if (Rand() % 10 == 0)
//			{
//				mSpeedLevel = Rand(9) + 1;
//			}
//		}
//	}
//
//	if (mState != STATE_DYING && mState != STATE_REVIVAL) {
//
//		if (mVX == 0.0)
//			mY += 1.0 / mVelMod;
//
//		if (mVX == 1.0)
//			mY += 0.75 / mVelMod;
//
//		if (mVX == 2.0)
//			mY += 0.5 / mVelMod;
//
//		if (mVX == 3.0)
//			mY += 0.25 / mVelMod;
//	}
//
//	if (mState != STATE_DYING && mState != STATE_REVIVAL) {
//
//		if (mX > (double)(mXMax - 5) && mVX > 0.1)
//			mVX -= 0.1;
//
//		if (mX < (double)(mXMin + 5) && mVX < -0.1)
//			mVX += 0.1;
//	}
//
//
//
//	if (mYMod <= 290 - 50) // Can't be reached
//		mY -= 0.25;
//
//	mX += mVX / mVelMod;
//	mY += mVY / mVelMod;
//
//	double afterVX = mVX;
//	if (prevVX < 0.0 && afterVX >= 0.0)
//		mTurnAnimTimer = 0;
//	else if (prevVX >= 0.0 && afterVX < 0.0)
//		mTurnAnimTimer = 0;
//
//	// Bounds
//	if (mX < mXMin)
//		mX = mXMin;
//	if (mX > mXMax)
//		mX = mXMax;
//	if (mY < mYMin && mState != STATE_BOUGHT)
//		mY = mYMin;
//	if (mY > mYMax&& mState != STATE_DYING)
//		mY = mYMax;
//
//	mAnimSpeed = 2;
//	if (abs(mVX) >= 4)
//		mAnimSpeed = 1;
//
//	if (mState != STATE_DYING && mState != STATE_REVIVAL)
//	{
//		/*if (!mTheApp->mAlienInTank)
//			PearlDrop();*/
//
//		// Animation Updates
//		if (mTimer % mAnimSpeed == 0)
//			if (++mSwimmingAnimTimer > 9)
//				mSwimmingAnimTimer = 0;
//
//		if (mTimer % 2 == 0)
//			if (++mTurnAnimTimer > 9)
//				mTurnAnimTimer = 10;
//
//		if (mTimer % 2 == 0)
//			if (mEatAnimTimer > 0)
//				--mEatAnimTimer;
//
//		if (mHungerTimer < 310)
//			if (++mStarvingAnimTimer > 4)
//				mStarvingAnimTimer = 4;
//
//		if (mHungerTimer > 310)
//			if (--mStarvingAnimTimer < 0)
//				mStarvingAnimTimer = 0;
//	}
//
//	if (mState == STATE_REVIVAL)
//	{
//		if (--mSwimmingAnimTimer < 0)
//		{
//			mSwimmingAnimTimer = 0;
//			mState = STATE_NONE;
//		}
//	}
//}
//
//void Beetlemuncher::Draw(Graphics* g)
//{
//	if (mHidden)
//		return;
//	g->SetColorizeImages(true);
//	if (mStarvingAnimTimer == 4)
//		g->SetColor(Color(255, 255, 255, 0));
//	else
//		g->SetColor(Color(255, 255, 255, 255));
//	bool swimmingMirror = mVX < 0 ? 0 : 1;
//	bool eatingMirror = mVX < 0 ? 0 : 1;
//	bool turningMirror = mVX < 0 ? 1 : 0;
//	if (mState != STATE_DYING && mState != STATE_REVIVAL)
//	{
//		if (mTurnAnimTimer == 10 && mEatAnimTimer == 0)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mSwimmingAnimTimer, 0, 80, 80), swimmingMirror);
//
//		if (mTurnAnimTimer != 10)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mTurnAnimTimer, 80, 80, 80), turningMirror);
//
//		if (mEatAnimTimer != 0 && mTurnAnimTimer == 10)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mEatAnimTimer, 160, 80, 80), eatingMirror);
//
//		g->SetColor(Color(255, 255, 255, mStarvingAnimTimer * (255 / 4))); // HUNGRY
//		if (mTurnAnimTimer == 10 && mEatAnimTimer == 0)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mSwimmingAnimTimer, 240, 80, 80), swimmingMirror);
//
//		if (mTurnAnimTimer != 10)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mTurnAnimTimer, 320, 80, 80), turningMirror);
//
//		if (mEatAnimTimer != 0 && mTurnAnimTimer == 10)
//			g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mEatAnimTimer, 480, 80, 80), eatingMirror);
//	}
//	else if (mState == STATE_DYING || mState == STATE_REVIVAL)
//	{
//		g->SetColor(Color(255, 255, 255, mDisappearValue));
//		g->DrawImageMirror(IMAGE_GEKKO, Rect(mX, mY, 80, 80), Rect(80 * mSwimmingAnimTimer, 400, 80, 80), swimmingMirror);
//		g->SetColor(Color(255, 255, 255, 255));
//	}
//
//	g->SetColorizeImages(false);
//
//	DrawAOInfo(g);
//
//
//	// Blip
//	if (mBlipInTank)
//	{
//		if (mHungerTimer < 500 && mState != STATE_DYING)
//			g->DrawImage(IMAGE_MISCITEMS, mX, mY, Rect(144, 0, 72, 72));
//	}
//	DrawFishName(g);
//}
//
//
//Drop* Beetlemuncher::FindNearestFood()
//{
//	int aDist = 10000;
//	Drop* ff = NULL;
//
//	/*for (int i = 0; i < mTheApp->mDrops.size(); i++)
//	{
//		Drop* ffsearch = mTheApp->mDrops[i];
//		if (ffsearch->mType == 5 && !ffsearch->mPickedUp)
//		{
//			int ax = (int)((double)ffsearch->mX - mX);
//			int ay = (int)((double)ffsearch->mY - mY);
//			int aNewDist = (int)sqrt((double)((ax * ax) + (ay * ay)));
//			if (aNewDist < aDist)
//			{
//				aDist = aNewDist;
//				ff = ffsearch;
//			}
//		}
//	}*/
//
//	return ff;
//}
//
//void Beetlemuncher::FishHungryBehavior()
//{
//	Drop* nearestFood = FindNearestFood();
//	if (mFishMovementChangeTimer >= 5 && nearestFood != NULL)
//	{
//		mFishMovementChangeTimer = 0;
//		if (mHungerTimer > 300)
//		{
//			// X
//			if (mX + 20.0 > nearestFood->mX + 20) {
//				if (mVX > -4.0) {
//					mVX -= 1.5;
//				}
//			}
//			else if (mX + 20.0 < nearestFood->mX + 20 && mVX < 4.0) {
//				mVX += 1.5;
//			}
//			// Y
//			if (mY + 16.0 > nearestFood->mY + 10) {
//				if (mVY > -3.0) {
//					mVY -= 1.5;
//				}
//			}
//			else if (mY + 16.0 < nearestFood->mY + 10 && mVY < 4.0) {
//				mVY += 1.5;
//			}
//
//			if (mXVelAbsolute < 5)
//				++mXVelAbsolute;
//		}
//		else // Starving
//		{
//			// X
//			if (mX + 20.0 > nearestFood->mX + 20) {
//				if (mVX > -5.0) {
//					mVX -= 1.5;
//				}
//			}
//			else if (mX + 16.0 < nearestFood->mX + 10 && mVX < 5.0) {
//				mVX += 1.5;
//			}
//			// Y
//			if (mY + 20.0 > nearestFood->mY + 20) {
//				if (mVY > -3.0) {
//					mVY -= 1.5;
//				}
//			}
//			else if (mY + 16.0 < nearestFood->mY + 10 && mVY < 4.0) {
//				mVY += 1.5;
//			}
//
//			if (mXVelAbsolute < 5)
//				++mXVelAbsolute;
//		}
//	}
//
//	// CHECK FOOD COLLISION
//	CheckFoodCollision();
//}
//
//void Beetlemuncher::CheckFoodCollision()
//{
//	if (mHungerTimer < 500)
//	{
//		FRect betlemunchrect = FRect(mX + 20, mY + 20, 40, 40);
//	//	for (int i = 0; i < mTheApp->mDrops.size(); i++)
//	//	{
//	//		Drop* drop = mTheApp->mDrops[i];
//	//		if (drop->mType != 5)
//	//			continue;
//	//		FRect droprect = FRect(drop->mX + 35, drop->mY + 35, 2, 2);
//	//		FRect dropAnimRect = FRect(drop->mX + 25, drop->mY + 25, 20, 30);
//	//		if (droprect.Intersects(betlemunchrect))
//	//		{
//	//			mHungerTimer += 900;
//	//			if (mHungerTimer > 1300)
//	//				mHungerTimer = 1300;
//
//	//			if (Rand(2))
//	//				mTheApp->PlaySample(SOUND_CHOMP);
//	//			else
//	//				mTheApp->PlaySample(SOUND_CHOMP2);
//	//			//PearlDrop();
//
//	//			mTheApp->mDrops.erase(mTheApp->mDrops.begin() + i);
//	//			--i;
//	//			break;
//	//		}
//	//		// Animation
//	//		if (betlemunchrect.Intersects(dropAnimRect) && mEatAnimTimer == 0)
//	//		{
//	//			if (mTurnAnimTimer > 5)
//	//			{
//	//				mEatAnimTimer = 9;
//	//				mTurnAnimTimer = 10;
//	//			}
//	//		}
//	//	}
//	}
//}
//
//bool Beetlemuncher::FishHungry()
//{
//	//if (!mTheApp->mAlienInTank)
//	//	--mHungerTimer;
//	//bool foodthere = false;
//	//for (int i = 0; i < mTheApp->mDrops.size(); i++)
//	//{
//	//	Drop* g = mTheApp->mDrops[i];
//	//	if (g->mType == 5)
//	//		foodthere = true;
//	//}
//
//	//if (mHungerTimer <= 0)
//	//{
//	//	if (mState != STATE_DYING)
//	//	{
//	//		mState = STATE_DYING;
//	//		mSwimmingAnimTimer = 0; // Used for death animation timing
//	//		SoundInstance* diesound = mTheApp->mSoundManager->GetSoundInstance(SOUND_DIE);
//	//		if (diesound != nullptr)
//	//		{
//	//			diesound->AdjustPitch(-10);
//	//			diesound->Play(false, true);
//	//		}
//	//	}
//	//	FishDeath();
//	//	return true;
//	//}
//	//else if (mHungerTimer < 500 && foodthere)
//	//{
//	//	FishHungryBehavior();
//	//	return true;
//	//}
//	return false;
//}
//
//
//void Beetlemuncher::PearlDrop()
//{
//	++mSpecialityTimer;
//	/*if (mSpecialityTimer >= mSpecialityValue)
//	{
//		mSpecialityTimer = 0;
//
//		Drop* pearl = new FishMoney(mTheApp, mX, mY, 0, 1.4, mXMin, mXMax, mYMin, mYMax, 9);
//		mTheApp->mDrops.push_back(pearl);
//	}*/
//}
//
//
//void Beetlemuncher::FishDeath()
//{
//	Guppy::FishDeath();
//}
//
//void Sexy::Beetlemuncher::Delete()
//{
//	mDisappearValue = 0;
//}
