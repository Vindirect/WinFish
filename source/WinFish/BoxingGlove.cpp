#include "BoxingGlove.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"

Sexy::BoxingGlove::BoxingGlove(FishTypePet* theWalter, bool right)
{
	mWalter = theWalter;
	mX = -200;
	mY = -200;
	mMouseVisible = false;
	m0x164 = 0;
	mWidth = 160;
	mHeight = 80;
	m0x15c = 30;
	m0x160 = right;
}

void Sexy::BoxingGlove::Update()
{
	Board* aBoard = mApp->mBoard;
	if (aBoard == nullptr || aBoard->mPause)
		return;

	if (mWalter != nullptr)
	{
		if (mWalter->mEatingAnimationTimer > 35)
			return;
		if (!m0x160)
			mX = mWalter->mX - 60;
		else
			mX = mWalter->mX - 20;
		mY = mWalter->mY;
	}

	m0x15c--;
	if (m0x15c > 0)
	{
		if (m0x164 > 0)
			m0x164--;

		bool hit = false;
		if (m0x15c > 12)
		{
			int anXHitPos = 60;
			if (m0x15c > 20)
				anXHitPos = 30;
			if (m0x15c > 25)
				anXHitPos = 0;
			if (!m0x160)
				anXHitPos = mX - anXHitPos + 80;	
			else
				anXHitPos = mX + anXHitPos + 80;
			int anYHitPos = mY + 40;

			for (GameObjectSet::iterator it = aBoard->mGameObjectSet.begin(); it != aBoard->mGameObjectSet.end(); ++it)
			{
				GameObject* anObj = *it;

				if (anObj != mWalter && anObj->mShown)
				{
					switch (anObj->mType)
					{
					case TYPE_GUPPY:
					case TYPE_OSCAR:
					case TYPE_ULTRA:
					case TYPE_GEKKO:
					case TYPE_SYLVESTER_FISH:
					case TYPE_BALL_FISH:
					case TYPE_BI_FISH:
						if (HandleFishHit(anObj, anXHitPos, anYHitPos, m0x160 * 2 - 1, 5))
							hit = true;
						break;
					case TYPE_FISH_TYPE_PET:
					{
						FishTypePet* aPet = (FishTypePet*)anObj;
						if (aPet->mFishTypePetType != PET_PREGO)
						{
							int aVal = -1;
							switch (aPet->mFishTypePetType)
							{
							case PET_VERT:
							case PET_SHRAPNEL:
							case PET_AMP:
								aVal = 5;
								break;
							case PET_NOSTRADAMUS:
								aVal = 10;
							}
							if (HandleFishHit(anObj, anXHitPos, anYHitPos, m0x160 * 2 - 1, 5))
								hit = true;
						}
						break;
					}
					}
				}
			}

			if (hit && m0x164 == 0)
			{
				mApp->mBoard->PlayPunchSound(3);
				m0x164 = 5;
			}
		}
	}
}

void Sexy::BoxingGlove::Draw(Graphics* g)
{
	int aSrcCol = abs(m0x15c - 15);
	Rect aSrcRect = Rect(9 - aSrcCol, 160, 80, 80);
	if (aSrcRect.mX > 0)
	{
		if (aSrcRect.mX < 6)
			aSrcRect.mX = 6;
		aSrcRect.mX *= 80;
		g->DrawImageMirror(IMAGE_WALTER, (m0x160 ? 83 : -3), 2, aSrcRect, m0x160);
	}

	if (!m0x160)
		g->DrawImageMirror(IMAGE_WALTER, aSrcCol * 6 - 20, 0, Rect(400, 160, 80, 80), false);
	else
		g->DrawImageMirror(IMAGE_WALTER, 100 - aSrcCol * 6, 0, Rect(400, 160, 80, 80), true);
}

bool Sexy::BoxingGlove::HandleFishHit(GameObject* theObj, int x, int y, int dir, int unk)
{
	Fish* aFish = (Fish*)theObj;
	FishTypePet* aPet = nullptr;
	if(aFish->mType == TYPE_FISH_TYPE_PET)
		aPet = (FishTypePet*)theObj;

	if (aFish->mWadsworthVXModCounter < 1 && (aFish->mType != TYPE_FISH_TYPE_PET || !aPet->m0x230 || aFish->mUpdateCnt > 29))
	{
		int aCX = aFish->mWidth / 2 + aFish->mX;
		int aCY = aFish->mHeight / 2 + aFish->mY;

		if (x - 40 < aCX && x + 40 > aCX && y - 40 < aCY && y + 40 > aCY)
		{
			if (unk > 0)
				aFish->mCoinDropTimer = aFish->mCoinDropT - unk;
			aFish->mWadsworthVXModCounter = 50;
			aFish->mUnusedTimer = 180;
			if (aFish->mSongId != -1)
				mApp->mBoard->mFishSongMgr->StopFishSong(aFish->mSongId);
			aFish->ShowInvisibility();
			if (Rand() % 2 == 0)
				aFish->mUnusedVXWadsworthAddon = dir * 15;
			else
				aFish->mUnusedVXWadsworthAddon = dir * 10;
			return true;
		}
	}
	return false;
}