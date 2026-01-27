#include "Coin.h"
#include "WinFishApp.h"
#include "Board.h"
#include "Res.h"
#include "BilaterusHead.h"
#include "OtherTypePet.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::Coin::Coin()
{
	mClip = false;
	m0x190 = nullptr;
	mType = TYPE_COIN;
}

Sexy::Coin::Coin(int theX, int theY, int theType, OtherTypePet* unk, double theVel)
{
	mXD = theX;
	mType = TYPE_COIN;
	mClip = false;
	m0x1a0 = 0;
	mYD = theY;
	m0x17c = 0;
	mComboCount = 1;
	m0x190 = unk;
	mCoinType = theType;
	mX = theX;
	mY = theY;
	mWidth = 72;
	mHeight = 72;
	if (theType == COIN_NOTE)
	{
		mMouseVisible = false;
		mDoFinger = false;
	}
	else
		mDoFinger = true;

	m0x19c = 0;
	mVY = 1.5;
	m0x168 = false;
	m0x198 = false;
	mAnimationTimer = 0;
	m0x170 = 0;
	mDisappearTimer = 0;
	mAnimationFrame = 0;
	if (theVel != -1.0)
		mVY = theVel;
}

void Sexy::Coin::Update()
{
	if (!mApp->mBoard || mApp->mBoard->mPause)
		return;

	UpdateCounters();
	mAnimationTimer++;
	m0x170++;
	if (mAnimationTimer > 79)
		mAnimationTimer = 0;
	if (mCoinType == COIN_SHRAPNEL_BOMB)
	{
		mAnimationFrame = (mAnimationTimer / 2) % 5;
	}
	else
	{
		if (!IsShell() ||
			(mCoinType != 1 && mCoinType != 2 && mCoinType != 4 && mCoinType != 5 && mCoinType != 6
				&& mCoinType != 7 && mCoinType != 11 && mCoinType != 12 && mCoinType != 13 && mCoinType != 14))
		{
			int aVal = mAnimationTimer;
			if (mApp->mBoard->mPetsInTank[10] == 0)
				aVal /= 2;
			else
				aVal /= 4;

			mAnimationFrame = aVal % 10;
		}
		else
			mAnimationFrame = (mAnimationTimer / ((mApp->mBoard->mPetsInTank[10] != 0) * 2 + 2)) % 20;
	}

	if (mDisappearTimer != 0)
	{
		if (!m0x198)
		{
			mDisappearTimer--;
			if (mDisappearTimer != 0)
				return;
			Remove();
			return;
		}
		mDisappearTimer = 0;
	}

	if (!m0x198)
	{
		if (mCoinType == COIN_NIKOPEARL) // 87
		{
			m0x19c++;
			if (m0x19c < 217)
				return;
			Remove();
			return;
		}


		if (mCoinType == COIN_NOTE) // 96
		{
			m0x19c++;
			int aVal = m0x1a0 != 0 ? -50 : 50;
			if ((m0x1a0 < 3 || m0x19c < 51) && m0x19c < 101 && aVal <= mYD)
			{
				mYD -= 1.0;
				Move(mXD, mYD);
				return;
			}
			Remove();
			return;
		} // 111

		if ((mCoinType != 18 && mCoinType != 5 && (mCoinType < 8 || mCoinType > 14)) || m0x168)
		{
			if (mApp->mBoard->mPetsInTank[10] == 0)
			{
				if (!mApp->mBoard->mIsBonusRound)
					mYD += 1.5;
				else
					mYD += mVY;
			}
			else
				mYD += 0.8;

			if (mYD > 370.0)
			{
				mYD = 370.0;
				if (mApp->mBoard->mIsBonusRound)
					mDisappearTimer = 5;
				m0x19c++;

				int unkval = 0;
				if (mApp->mBoard->mPetsInTank[10] == 0)
				{
					if (!mApp->mRelaxMode)
						unkval = 20 + (mApp->mBoard->IsFirstLevel() ? 130 : 0);
					else
						unkval = 108;
				}
				else
					unkval = 200;
				if (m0x19c >= unkval)
				{
					if (mCoinType == COIN_SHRAPNEL_BOMB)
					{
						Remove();
						return;
					}
					mDisappearTimer = 5;
				}
			} // 157

			Move(mXD, mYD);
			if (mCoinType == COIN_SHRAPNEL_BOMB && m0x170 > 30)
			{
				for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
				{
					Fish* aFish = mApp->mBoard->mFishList->at(i);

					if (aFish->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aFish->mX + 70 && mXD + 36.0 > aFish->mX + 10
							&& mYD + 36.0 < aFish->mY + 70 && mYD + 36.0 > aFish->mY + 10)
						{
							aFish->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mOscarList->size(); i++)
				{
					Fish* aFish = mApp->mBoard->mOscarList->at(i);

					if (aFish->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aFish->mX + 70 && mXD + 36.0 > aFish->mX + 10
							&& mYD + 36.0 < aFish->mY + 70 && mYD + 36.0 > aFish->mY + 10)
						{
							aFish->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mUltraList->size(); i++)
				{
					Fish* aFish = mApp->mBoard->mUltraList->at(i);

					if (aFish->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aFish->mX + 150 && mXD + 36.0 > aFish->mX + 10
							&& mYD + 36.0 < aFish->mY + 150 && mYD + 36.0 > aFish->mY + 10)
						{
							aFish->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mGekkoList->size(); i++)
				{
					Fish* aFish = mApp->mBoard->mGekkoList->at(i);

					if (aFish->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aFish->mX + 150 && mXD + 36.0 > aFish->mX + 10
							&& mYD + 36.0 < aFish->mY + 150 && mYD + 36.0 > aFish->mY + 10)
						{
							aFish->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mPentaList->size(); i++) // 444
				{
					Penta* aPenta = mApp->mBoard->mPentaList->at(i);

					if (aPenta->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aPenta->mX + 150 && mXD + 36.0 > aPenta->mX + 10
							&& mYD + 36.0 < aPenta->mY + 150 && mYD + 36.0 > aPenta->mY + 10)
						{
							aPenta->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mGrubberList->size(); i++) // 497
				{
					Grubber* aGrubber = mApp->mBoard->mGrubberList->at(i);

					if (aGrubber->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aGrubber->mX + 150 && mXD + 36.0 > aGrubber->mX + 10
							&& mYD + 36.0 < aGrubber->mY + 150 && mYD + 36.0 > aGrubber->mY + 10)
						{
							aGrubber->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}

				for (int i = 0; i < mApp->mBoard->mBreederList->size(); i++) // 497
				{
					Breeder* aFish = mApp->mBoard->mBreederList->at(i);

					if (aFish->mVirtualTankId < 0)
					{
						if (mXD + 36.0 < aFish->mX + 150 && mXD + 36.0 > aFish->mX + 10
							&& mYD + 36.0 < aFish->mY + 150 && mYD + 36.0 > aFish->mY + 10)
						{
							aFish->Die(true);
							Remove();
							int aShotType = mApp->mSeed->Next() % 3 + 3;
							mApp->mBoard->SpawnShot(mXD, mYD, aShotType);
							return;
						}
					}
				}
			}
			return;
		}

		if (mYD < 120.0)
		{
			m0x168 = true;
		} // 309
		else
		{
			if (mYD >= 150)
				mYD -= 8.0;
			else if (mYD >= 135)
				mYD -= 2.5;
			else if (mYD >= 130)
				mYD -= 1.5;
			else if (mYD >= 125)
				mYD -= 1.0;
			else
				mYD -= 0.5;
		}
	}
	else // 332
	{
		if (!mApp->mBoard->mIsBonusRound)
		{
			if (mXD < 550.0)
				mXD = (550.0 - mXD) / 7.0 + mXD;
			else if (mXD > 550.0)
				mXD = mXD - (mXD - 550.0) / 7.0;

			if (mYD < 30.0)
				mYD = (30.0 - mYD) / 7.0 + mYD;
			else if (mYD > 30.0)
				mYD = mYD - (mYD - 30.0) / 7.0;

			if (mY < 40)
			{
				Remove();
				ReceiveMoney();
				return;
			}
		}
		else
		{ // 358
			m0x17c++;
			int aXDist = m0x180 - (260 + 30);
			int aYDist = m0x184 - (265 + 50);
			int aDist = aXDist * aXDist + aYDist * aYDist;
			int aVal = aDist < 22501 ? 5 : 15;
			if (aVal <= m0x17c)
			{
				Remove();
				ReceiveMoney();
				return;
			}
			int aSomeDist = aVal - m0x17c;
			mXD = (aSomeDist * m0x180 + (260 + 30) * m0x17c) / aVal;
			// 375 iVar3 = iVar3 % iVar9; ?
			mYD = (aSomeDist * m0x184 + (265 + 50) * m0x17c) / aVal;
		}
	}

	Move(mXD, mYD);
}

void Sexy::Coin::Draw(Graphics* g)
{
	GameObject::UpdateFishSongMgr();

	int aVal = m0x19c % 8;
	if (aVal > 7 && mCoinType != 6 && mCoinType != 7 && !m0x198)
		return;

	if (mDisappearTimer != 0)
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, mDisappearTimer * 255 / 5));
	}

	bool isShell = Coin::IsShell();
	Image* aCoinImg = IMAGE_SHELLS;
	if (mCoinType == COIN_SILVER_C || mCoinType == SHELL_SILVER)
	{
		int aX = 20;
		int aY = 20;
		if (!isShell)
		{
			aCoinImg = IMAGE_MONEY;
			aX = 0;
			aY = 0;
		}
		g->DrawImageCel(aCoinImg, aX, aY, mAnimationFrame, 0);
		g->SetColorizeImages(false);
		return;
	}
	else if (mCoinType == COIN_GOLD_C || mCoinType == SHELL_GOLD)
	{
		int aX = 20;
		int aY = 20;
		if (!isShell)
		{
			aCoinImg = IMAGE_MONEY;
			aX = 0;
			aY = 0;
		}
		g->DrawImageCel(aCoinImg, aX, aY, mAnimationFrame, 1);
		g->SetColorizeImages(false);
		return;
	}
	else if (mCoinType == 5 || mCoinType == 12 || mCoinType == COIN_DIAMOND || mCoinType == 11)
	{
		int aRow = 3;
		int aX = 0;
		int aY = 0;
		if (isShell)
		{
			aRow = 2;
			aX = 20;
			aY = 20;
		}
		else
			aCoinImg = IMAGE_MONEY;
		g->DrawImageCel(aCoinImg, aX, aY, mAnimationFrame, aRow);
		g->SetColorizeImages(false);
		return;
	}
	else if (mCoinType == COIN_PEARL || mCoinType == 13)
	{
		if (!isShell)
		{
			g->DrawImage(IMAGE_PEARL, 0,0);
			g->SetColorizeImages(false);
			return;
		}

		g->DrawImageCel(aCoinImg, 20, 20, mAnimationFrame, 3);
		g->SetColorizeImages(false);
		return;
	}
	else
	{
		if (mCoinType == 7 || mCoinType == 14)
		{
			if (isShell)
			{
				g->DrawImageCel(IMAGE_MONEYBAG, 18, 18, 0, 0);
				g->SetColorizeImages(false);
				return;
			}

			g->DrawImageCel(IMAGE_MONEY, 0, 0, mAnimationFrame, 4);
			g->SetColorizeImages(false);
			return;
		}
		else // 120
		{
			if (mCoinType == 18)
			{
				g->DrawImage(IMAGE_PEANUT, 0, 0);
				g->SetColorizeImages(false);
				return;
			}

			if (mCoinType == COIN_STAR || mCoinType == SHELL_STAR)
			{
				g->DrawImageCel(IMAGE_MONEY, 0, 0, mAnimationFrame, 2);
				g->SetColorizeImages(false);
				return;
			}

			if (mCoinType != COIN_NIKOPEARL || !m0x198)
			{
				if (mCoinType == COIN_NOTE)
				{
					if (m0x1a0 < 2)
					{
						g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 1);
						g->SetColorizeImages(false);
						return;
					}
					Color aCol = Color::White;
					if (m0x1a0 < 3)
					{
						g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
					}
					else
					{
						g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
						switch (m0x1a0)
						{
						case 3:
							aCol = Color(0xaaaaaa);
							break;
						case 4:
							aCol = Color(0xfddc41);
							break;
						case 5:
							aCol = Color(0x29e7ff);
							break;
						case 6:
							aCol = Color(0x88ff88);
							break;
						case 7:
							aCol = Color(0xffff00);
							break;
						default:
							aCol = Color(0xffff00);
						}
						if (m0x19c > 0)
						{
							aCol.mAlpha = ((50 - m0x19c) * 255) / 50;
							if (aCol.mAlpha < 1)
								return;
						}
					}
					g->SetColor(aCol);
					g->DrawString(m0x1a4, 5, g->GetFont()->GetHeight());
					g->SetColorizeImages(false);
					return;
				}
				if (mCoinType == COIN_SHRAPNEL_BOMB)
				{
					g->DrawImageCel(IMAGE_MISCITEMS, 0, 0, 0);
					g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
					g->DrawImageCel(IMAGE_SPARKS, 10, -10, mAnimationFrame);
					g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
					return;
				}
				g->SetColorizeImages(false);
				return;
			}

			g->DrawImage(IMAGE_PEARL, 0, 0);
			g->SetColorizeImages(false);
			return;
		}
	}

	g->DrawImageCel(IMAGE_MONEY, 0, 0, mAnimationFrame, 2);
	g->SetColorizeImages(false);
}

void Sexy::Coin::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount < 0)
	{
		mApp->mBoard->CheckMouseDown(mX + x, mY + y);
		return;
	}

	if (mCoinType == 18 && mUpdateCnt < 12)
		return;

	if (mCoinType == COIN_NOTE || m0x198)
		return;
	Board* aBoard = mApp->mBoard;
	bool aFlag = false;
	if (aBoard->AliensInTank() || aBoard->mMissleList1->size() > 0)
	{
		for (int i = 0; i < aBoard->mMissleList1->size(); i++)
		{
			if (aBoard->mMissleList1->at(i)->Contains(mX + x, mY + y))
			{
				aFlag = true;
				break;
			}
		}
		for (int i = 0; i < aBoard->mAlienList->size(); i++)
		{
			if (aBoard->mAlienList->at(i)->Contains(mX + x, mY + y))
			{
				aFlag = true;
				break;
			}
		}
		for (int i = 0; i < aBoard->mBilaterusList->size(); i++)
		{
			if (aBoard->mBilaterusList->at(i)->mActiveHead && aBoard->mBilaterusList->at(i)->mActiveHead->Contains(mX + x, mY + y))
			{
				aFlag = true;
				break;
			}
		}

		if (!gUnkBool04 && aFlag)
		{
			gUnkBool04 = true;
			aBoard->MouseDown(x, y, 1);
			aFlag = true;
			gUnkBool04 = false;
		}
		else
		{
			aFlag = false;
		}
	}

	m0x17c = 1;

	m0x180 = mXD;
	m0x184 = mYD;
	if (mCoinType == COIN_NIKOPEARL)
	{
		m0x198 = true;
		for (int i = 0; i < aBoard->mOtherTypePetList->size(); i++)
		{
			OtherTypePet* aPet = aBoard->mOtherTypePetList->at(i);
			if (m0x190 != 0 && m0x190 == aPet)
				aPet->m0x1c0 = true;
		}
		aBoard->PlaySample(SOUND_PEARL_ID, 3, 1.0);
	}

	mMouseVisible = false;

	if (mCoinType < 15 || mCoinType == COIN_SHRAPNEL_BOMB || mCoinType == 18)
	{
		m0x198 = true;
		if (!aBoard->mIsBonusRound || !aBoard->Unk09(this))
		{
			if (mCoinType == COIN_DIAMOND || mCoinType == 5 || mCoinType == 11 || mCoinType == 12)
				aBoard->PlaySample(SOUND_DIAMOND_ID, 3, 1.0);
			else
			{
				if (mCoinType == 18)
					aBoard->PlayChompSound(false);
				else
				{
					if (mCoinType == COIN_PEARL || mCoinType == 13)
						aBoard->PlaySample(SOUND_PEARL_ID, 3, 1.0);
					else
					{
						if (mCoinType == 7 || mCoinType == 14)
						{
							aBoard->PlaySample(IsShell() ? SOUND_BONUSCOLLECT_ID : SOUND_TREASURE_ID, 3, 1.0);
						}
						else
						{
							aBoard->PlayPointsSound();
						}
					}
				}
			}
		}
	}
}

void Sexy::Coin::Remove()
{
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mCoinType == COIN_SHRAPNEL_BOMB && !m0x198)
	{
		mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
		int aNumOfSmokes = mApp->mSeed->Next() % 3 + 2;
		for (int i = 0; i < aNumOfSmokes; i++)
		{
			int aType = mApp->mSeed->Next() % 3 + 3;
			int aRandX = (mApp->mSeed->Next() % 30 - 10) + mX;
			int aRandY = (mApp->mSeed->Next() % 30 - 10) + mY;
			mApp->mBoard->SpawnShot(aRandX, aRandY, aType);
		}
	}
}

void Sexy::Coin::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncDouble(mXD);
	theSync->SyncDouble(mYD);
	theSync->SyncBool(m0x168);
	theSync->SyncLong(mAnimationTimer);
	theSync->SyncLong(m0x170);
	theSync->SyncLong(mAnimationFrame);
	theSync->SyncLong(mDisappearTimer);
	theSync->SyncLong(m0x17c);
	if (m0x17c > 0)
	{
		theSync->SyncLong(m0x180);
		theSync->SyncLong(m0x184);
	}
	theSync->SyncDouble(mVY);
	theSync->SyncLong(mCoinType);
	theSync->SyncBool(m0x198);
	theSync->SyncLong(m0x19c);
	theSync->SyncLong(mComboCount);
	theSync->SyncLong(m0x1a0);
	if (m0x1a0 > 1)
		theSync->SyncString(m0x1a4);
	theSync->SyncPointer((void**) &m0x190);
}

bool Sexy::Coin::IsShell()
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK || mApp->mBoard->mIsBonusRound)
		return true;
	return false;
}

int Sexy::Coin::GetValue()
{
	if (mCoinType == COIN_SILVER_C || mCoinType == SHELL_SILVER)
	{
		if (IsShell())
			return mComboCount;
		return 15;
	}
	if (mCoinType == COIN_GOLD_C || mCoinType == SHELL_GOLD)
	{
		if (IsShell())
			return mComboCount * 2;
		return 35;
	}
	if (mCoinType == COIN_STAR || mCoinType == SHELL_STAR)
	{
		if (IsShell())
			return mComboCount * 3;
		return 40;
	}
	if (mCoinType == COIN_DIAMOND || mCoinType == 11 || mCoinType == 5 || mCoinType == 12)
	{
		if (IsShell())
			return mComboCount * 5;
		return 200;
	}
	if (mCoinType == COIN_PEARL || mCoinType == 13)
	{
		if (IsShell())
			return mComboCount * 10;
		return 500;
	}
	if (mCoinType == 7 || mCoinType == 14)
	{
		if (IsShell())
			return mComboCount * 20;
		return 2000;
	}
	if (mCoinType == COIN_NIKOPEARL)
	{
		if (IsShell())
			return mComboCount * 10;
		return 250;
	}
	if (mCoinType == COIN_SHRAPNEL_BOMB)
	{
		if (IsShell())
			return mComboCount * 5;
		return 150;
	}
	if (mCoinType == 18)
	{
		if (IsShell())
			return mComboCount;
		return 15;
	}
	return 0;
}

void Sexy::Coin::ReceiveMoney()
{
	mApp->mBoard->mShouldSave = true;
	mApp->mBoard->Unk07(GetValue());
}

void Sexy::Coin::PetCollected()
{
	ReceiveMoney();
	if (mCoinType == COIN_DIAMOND || mCoinType == 5 || mCoinType == 11 || mCoinType == 12)
		mApp->mBoard->PlaySample(SOUND_DIAMOND_ID, 3, 1.0);
	else if (mCoinType == COIN_PEARL || mCoinType == 13)
		mApp->mBoard->PlaySample(SOUND_PEARL_ID, 3, 1.0);
	else if (mCoinType == 7 || mCoinType == 14)
		mApp->mBoard->PlaySample(IsShell() ? SOUND_BONUSCOLLECT_ID : SOUND_TREASURE_ID, 3, 1.0);
	else
		mApp->mBoard->PlayPointsSound();
}
