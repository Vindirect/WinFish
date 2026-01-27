#include "Fish.h"
#include "WinFishApp.h"
#include "Board.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Res.h"

using namespace Sexy;

const int OSCAR_RED_TABLE1[12] =
{ 165, 40, 30, 255, 145, 40, 150, 200, 130, 220, 185, 40 };
const int OSCAR_GREEN_TABLE1[12] =
{ 210, 40, 30, 155, 210, 40, 125, 125, 190, 190, 100, 40 };
const int OSCAR_BLUE_TABLE1[12] =
{ 215, 40, 30, 0, 145, 40, 85, 215, 85, 140, 240, 40 };

const int OSCAR_RED_TABLE2[12] =
{ 75, 20, 40, 255, 20, 75, 160, 150, 245, 180, 105, 75 };
const int OSCAR_GREEN_TABLE2[12] =
{ 75, 70, 40, 65, 75, 70, 65, 80, 170, 170, 15, 75 };
const int OSCAR_BLUE_TABLE2[12] =
{ 75, 110, 40, 0, 120, 70, 25, 175, 0, 155, 245, 75 };

const int OSCAR_RED_TABLE3[12] =
{ 40, 50, 75, 250, 35, 255, 90, 170, 245, 140, 40, 255 };
const int OSCAR_GREEN_TABLE3[12] =
{ 125, 150, 75, 255, 150, 255, 70, 255, 245, 120, 40, 110 };
const int OSCAR_BLUE_TABLE3[12] =
{ 130, 225, 75, 0, 30, 255, 40, 255, 140, 80, 40, 0 };

const int FISH_RED_TABLE1[30] =
{ 125, 255, 135, 175, 195, 255, 255, 60,  255, 190,
    190, 145, 240, 255, 210, 175, 255, 65,  240, 240,
    255, 255, 175, 225, 130, 160, 205, 190, 240, 210 };
const int FISH_GREEN_TABLE1[30] =
{ 210, 240, 250, 235, 140, 255, 255, 60,  65,  60,
    190, 145, 145, 255, 215, 240, 180, 70,  240, 225,
    230, 40,  215, 175, 165, 230, 50,  140, 250, 250 };
const int FISH_BLUE_TABLE1[30] =
{ 255, 60,  245, 135, 90,  85,  255, 60,  65,  255,
    190, 175, 235, 40,  175, 175, 110, 160, 240, 125,
    0,   0,   110, 95,  145, 50,  180, 250, 240, 50 };

const int FISH_RED_TABLE2[30] =
{ 50,  45,  255, 50,  65,  250, 230, 0,   75,  255,
    250, 30,  110, 210, 110, 30,  210, 240, 0,   240,
    0,   255, 15,  65,  125, 145, 255, 250, 80,  110
}; 
const int FISH_GREEN_TABLE2[30] =
{ 110, 95, 0,   115, 120, 155, 130, 0,   75,  0,
    250, 25, 10,  10,  95,  80,  10,  240, 5,   5,
    125, 225, 30,  55,  15,  0,   145, 245, 250, 210 };
const int FISH_BLUE_TABLE2[30] =
{ 210, 195, 125, 210, 65,  0,   250, 0,   75,  125,
    250, 225, 225, 0,   210, 125, 0,   240, 130, 240,
    0,   0,   15,  190, 15,  210, 220, 175, 145, 0 };

//int Sexy::gWadsworthTimer;
//int Sexy::gWadsworthX;
//int Sexy::gWadsworthY;

Fish::Fish() : GameObject()
{
    mColors[0] = Color(0, 0, 0, 255);
    mColors[1] = Color(0, 0, 0, 255);
    mColors[2] = Color(0, 0, 0, 255);
    mUnusedVXWadsworthAddon = 0;
    mClip = false;
    mType = GameObject::TYPE_GUPPY;
    mWadsworthVXModCounter = 0;
    mUnusedTimer = 0;
    mVirtualFish = 0;
    mRainbowFishDeterminant = 0;
}

Fish::Fish(int theX, int theY) : GameObject()
{
    mColors[0] = Color(0, 0, 0, 255);
    mColors[1] = Color(0, 0, 0, 255);
    mColors[2] = Color(0, 0, 0, 255);
    Init(theX, theY);
    mSize = SIZE_SMALL;
}

Fish::Fish(int theX, int theY, int theSize, bool theVelDirection) : GameObject()
{
    mColors[0] = Color(0, 0, 0, 255);
    mColors[1] = Color(0, 0, 0, 255);
    mColors[2] = Color(0, 0, 0, 255);
    Init(theX, theY);
    mVX = theVelDirection ? 1 : -1;
    mPrevVX = theVelDirection ? 1 : -1;
    mSize = theSize;
}

Fish::~Fish()
{
}

void Fish::Update()
{
    if (mApp->mBoard == NULL || mApp->mBoard->mPause)
        return;
    Board* aBoard = mApp->mBoard;
    UpdateCounters();
    if (!gWadsworthTimer || !mIsGuppy || mSize > 1)
    {
        if (!Hungry())
        {
            bool anAliensInTank = aBoard->AliensInTank();
            if (!anAliensInTank || !aBoard->mPetsInTank[12] || !mIsGuppy)
            {
                if (mMovementState == 123)
                {
                    mVXAbs = (int)abs(mVX);
                    mMovementStateChangeTimer = 0;
                    if (mSpecialMovementStateChangeTimer > 30)
                    {
                        if (abs(mVX) > 2.0)
                            mVX *= 0.95;
                        if (abs(mVY) > 2.0)
                            mVY *= 0.95;
                    }
                    if (mSpecialMovementStateChangeTimer > 50)
                    {
                        mSpecialMovementStateChangeTimer = 0;
                        mMovementState = Rand() % 9 + 1;
                        int pr = mXMax - 20;
                        if (mXD > mXMax - 20 || mXD < mXMin + 20 || Rand() % 3 == 0)
                            mVX *= -1.0;
                    }
                }
                else if (mMovementState < 5)
                {
                    if (mMovementState == 0)
                    {
                        if (mBoughtTimer == 0)
                            mVY = 0.5;
                        if(mSpecialMovementStateChangeTimer > 39)
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
                            {
                                mVX++;
                            }
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
                }
                else
                {
                    if(mBoughtTimer == 0)
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
            else // 236 line decomp follow Gumbo
            {
                FishTypePet* aGumbo = NULL;
                for (int i = 0; i < aBoard->mFishTypePetList->size(); i++)
                {
                    FishTypePet* aPet = aBoard->mFishTypePetList->at(i);

                    if (aPet->mFishTypePetType == FishTypePet::PET_GUMBO)
                    {
                        aGumbo = aPet;
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
        }
    }
    else if (gWadsworthTimer && mIsGuppy && mSize < 2) // 358 line decomp wadsworth
    {
        if (mWadsworthVXModCounter == 0)
        {
            if (mVX < 0.0)
                mVX = -0.5;
            else
                mVX = 0.5;
        }
        if (mBoughtTimer == 0)
            mVY = 0.0;

        if (mXD > gWadsworthX + 50)
            mXD -= 5.0;
        else if (mXD < gWadsworthX - 50)
            mXD += 5.0;
        else if (mXD > gWadsworthX + 5)
            mXD -= 3.0;
        else if (mXD < gWadsworthX - 5)
            mXD += 3.0;

        if (mYD > gWadsworthY + 50)
            mYD -= 4.0;
        else if (mYD < gWadsworthY - 50)
            mYD += 4.0;
        else if (mYD > gWadsworthY + 5)
            mYD -= 3.0;
        else if (mYD < gWadsworthY - 5)
            mYD += 3.0;
    }

    mSpecialMovementStateChangeTimer++;
    mMovementStateChangeTimer++;
    if (mMovementStateChangeTimer > 20)
    {
        mMovementStateChangeTimer = 0;
        if (Rand() % 10 == 0)
            mMovementState = Rand() % 9 + 1;
    }

    if (!aBoard->AliensInTank())
        DropCoin();

    if (mBoughtTimer > 0)
    {
        mBoughtTimer--;
        mVY *= 0.9;
    }
    if (mWadsworthVXModCounter > 0)
    {
        mWadsworthVXModCounter--;
        mXD += mUnusedVXWadsworthAddon * 0.9;
        mUnusedVXWadsworthAddon *= 0.9;
    }
    if (mUnusedTimer > 0)
    {
        mUnusedTimer--;
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
    else
    {
        if (mBoughtTimer >= 31)
        {
            int aChance = 2;
            int aSpawnX;
            int aSpawnY;
            if (mBoughtTimer > 40)
                aChance = 1;

            if (Rand() % aChance == 0)
            {
                if (mSize == SIZE_SMALL)
                {
                    if (Rand() % 2)
                    {
                        aSpawnX = mX + 40 - Rand() % 30;
                        aSpawnY = mY + 40 - Rand() % 30;
                        aBoard->SpawnBubble(aSpawnX, aSpawnY);
                    }
                }
                else
                {
                    if (mType != GameObject::TYPE_ULTRA)
                    {
                        aBoard->SpawnBubble(mX + (55 - Rand() % 60), mY + (55 - Rand() % 60));
                        aSpawnX = mX + 45 - Rand() % 40;
                        aSpawnY = mY + 45 - Rand() % 40;
                        aBoard->SpawnBubble(aSpawnX, aSpawnY);
                    }
                    else
                    {
                        aBoard->SpawnBubble(mX + (125 - Rand() % 120), mY + (125 - Rand() % 120));
                        aBoard->SpawnBubble(mX + (105 - Rand() % 80), mY + (105 - Rand() % 80));
                        aSpawnX = mX + 105 - Rand() % 80;
                        aSpawnY = mY + 105 - Rand() % 80;
                        aBoard->SpawnBubble(aSpawnX, aSpawnY);
                    }
                }
            }
        }
    }

    if (mXD > mXMax - 5 && mVX > 0.1)
        mVX -= 0.1;
    if (mXD < 15 && mVX < -0.1)
        mVX += 0.1;

    FishUpdateAnimation();

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

void Fish::Draw(Graphics* g)
{
    UpdateFishSongMgr();

    if (gWadsworthTimer != 0 && mXD < gWadsworthX + 8 && mXD > gWadsworthX - 8 &&
        mYD < gWadsworthY + 8 && mYD > gWadsworthY - 8 && mIsGuppy && mSize < 2)
    {
        if (mShadowPtr)
            mShadowPtr->m0x168 = 0.0;
        return;
    }
    if(mShadowPtr)
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

    DrawFish(g, shouldFlip);

    if (mName.size() > 0)
        DrawName(g, false);
}

void Sexy::Fish::MouseDown(int x, int y, int theClickCount)
{
    if (theClickCount < 0)
    {
        mApp->mBoard->CheckMouseDown(mX + x, mY + y);
        return;
    }
    Board* aBoard = mApp->mBoard;

    if (aBoard->mAlienList->size() == 0 && aBoard->mBilaterusList->size() == 0 &&
        aBoard->mMissleList1->size() == 0 && mIsGuppy)
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

void Sexy::Fish::MouseUp(int x, int y, int theClickCount)
{
    GameObject::MouseUp(x, y, theClickCount);
    mApp->mBoard->m0x4ec = false;
    mApp->mBoard->m0x4ed = false;
}

int Sexy::Fish::SpecialReturnValue()
{
    if(mType != TYPE_BALL_FISH && mType != TYPE_BI_FISH)
        return 0;
    return 10;
}

int Sexy::Fish::GetShellPrice()
{
    int aVal = GameObject::GetShellPrice();
    if (mVirtualFish)
    {
        if (aVal < 0)
            aVal = mShellPrice / 2;

        if (aVal > 0)
        {
            if (mSize == TYPE_MEDIUM_GUPPY)
                aVal *= 2;
            else if (mSize == TYPE_BIG_GUPPY)
                aVal *= 3;
            else if (mSize == TYPE_CROWNED_GUPPY)
                aVal *= 5;
        }
    }
    return aVal;
}

void Sexy::Fish::Remove()
{
    RemoveFromGame(true);
}

void Sexy::Fish::SetPosition(double newX, double newY)
{
    mX = newX;
    mXD = newX;
    mY = newY;
    mYD = newY;
}

void Sexy::Fish::OnFoodAte(GameObject* obj)
{
    bool isHungry = IsHungryVisible();

    mApp->mBoard->PlaySlurpSound(mVoracious);
    Food* aFood = (Food*)obj;
    bool unkflag01 = aFood->mExoticFoodType == 2;
    Unk02(unkflag01);

    bool unkflag02 = true;
    bool doPart = true;
    if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
    {
        if (mVirtualTankId < 0 || unkflag01)
        {
            unkflag02 = false;
        }
        else // 41
        {
            doPart = false;

            unkflag02 = UpdateMentalState();
            if (mPreNamedTypeId == ROCKY)
                unkflag02 = false;
        }
    }

    if (doPart)
    { // 48
        if (unkflag01)
        {
            if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK) // 50
            {
                if (mHunger > 30)
                    mHunger = 30;

                mApp->mBoard->MakeNote(mX - 20 + mWidth / 2, mY - 5, 2, "YUK!");
            }
            else if (mSize < 3) // 71
            {
                RemoveFromGame(true);
                FishTypePet* aPet = (FishTypePet*) mApp->mBoard->SpawnPet(PET_NOSTRADAMUS, mX, mY, false, true);
                if (aPet)
                    aPet->m0x240 = 1;
                if (mApp->mBoard->HasAnyFish())
                    return;

                mApp->mBoard->SpawnGuppyBought();
                return;
            }
        }
    }

    if (!doPart || !unkflag01) // from 91 to 188
    {
        if (aFood->mFoodType == 0)
        {
            mHunger += 500;
            if (mBeginner) mHunger += 200;
            if (mHunger > 800) mHunger = 800;
            if (unkflag02) mFoodAte++;
        }
        else if (aFood->mFoodType == 1)
        {
            mHunger += 700;
            if (mHunger > 1000) mHunger = 1000;
            if (unkflag02) mFoodAte = mFoodAte + 1 + mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK;
        }
        else if (aFood->mFoodType == 3)
        {
            if (mSize == 0 || mSize == 1)
            {
                mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
                int aNumOfSmokes = mApp->mSeed->Next() % 3 + 2;
                for (int i = 0; i < aNumOfSmokes; i++)
                {
                    int aType = mApp->mSeed->Next() % 3 + 3;
                    int aY = mApp->mSeed->Next() % 30 - 10 + mY;
                    int aX = mApp->mSeed->Next() % 30 - 10 + mX;
                    mApp->mBoard->SpawnShot(aX, aY, aType);
                }

                if (mApp->mBoard->mTank == 2 && mApp->mBoard->mLevel < 3 && !mApp->mCurrentProfile->mFinishedGame)
                {
                    if (mApp->mBoard->m0x4b0[32])
                        mApp->mBoard->ShowText("Hint: Feed star potions to BIG guppies!", false, 33);
                    if (mApp->mBoard->m0x4b0[31])
                        mApp->mBoard->ShowText("Hint: Star potions are for big guppies only!", false, 32);
                    if (mApp->mBoard->m0x4b0[30])
                        mApp->mBoard->ShowText("Hint: Only certain fish can handle star potions!", false, 31);

                    mApp->mBoard->ShowText("Warning! Use star potions carefully!", false, 30);
                }

                mApp->mBoard->m0x49c++;
                Die(true);
            }
            else // 156
            {
                mHunger += 1100;
                if (aFood->mFoodType == 2)
                {
                    if (mHunger > 1400)
                        mHunger = 1400;
                    mSize = SIZE_STAR;
                    mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
                }
                else if(mHunger > 1400)
                    mHunger = 1400;
            }
        }
        else
        {
            if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
            {
                mHunger += 500;
                if (mHunger > 800) mHunger = 800;
            }
            else
            {
                mHunger += 1100;
                if (mHunger > 1400) mHunger = 1400;
            }
            if (unkflag02)
                mFoodAte = mFoodAte + 2 + mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK;
        }
    }

    if (mFoodNeededToGrow <= mFoodAte)
    {
        if (mSize < 2)
        {
            mSize++;
            if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
                FishOnGrow();

            mFoodAte = 0;
            mGrowthAnimationTimer = 10;
            mApp->mBoard->PlaySample(SOUND_GROW_ID, 3, 1.0);
        }
        if (mFoodNeededToGrow * ((mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK ? 7 : 0) + 8) <= mFoodAte
            && mSize > SIZE_MEDIUM && mSize != SIZE_CROWNED && !mHasSpecialColors)
        {
            mSize = SIZE_CROWNED;
            mApp->mBoard->PlaySample(SOUND_CROWNED_ID, 3, 1.0);
        }
    }

    UpdateHungerStateIfWasHungry(isHungry);
}

void Sexy::Fish::UpdateStoreAnimation()
{
    mUpdateCnt++;
    mStoreAnimationTimer++;
    if (mInvisible)
        m0x128++;
    if (mRainbowFish)
        UpdateRainbow();

    mStoreAnimationIndex = (mStoreAnimationTimer / 2) % 10;
}

void Sexy::Fish::DrawStoreAnimation(Graphics* g, int justification)
{
    int anXTrans = 0, anYTrans = 0;
    switch (justification)
    {
    case 0:
        anXTrans = 19;
        anYTrans = 20;
        break;
    case 1:
        anXTrans = 10;
        anYTrans = 17;
        break;
    case 2:
        anXTrans = 19;
        break;
    case 3:
        anXTrans = 5;
        break;
    case 4:
        anXTrans = -40;
        anYTrans = -5;
        break;
    }
    g->Translate(anXTrans, anYTrans);
    int aRow = mSize;
    if (mSize > TYPE_BIG_GUPPY)
    {
        if (mSize == TYPE_STAR_GUPPY)
            aRow = 2;
        else
        {
            aRow = 3;
            if (mSize != TYPE_CROWNED_GUPPY)
                aRow = justification;
        }
    }

    if (!mHasSpecialColors)
    {
        SetStoreColor(g, Color::White);
        g->DrawImage(IMAGE_SMALLSWIM, 0, 0, Rect(mStoreAnimationIndex * 80, aRow * 80, 80, 80));
    }
    else
    {
        Rect aSrcRect(mStoreAnimationIndex * 80, aRow * 80, 80, 80);
        SetStoreColor(g, Color::White);
        g->DrawImageMirror(GetImageById(IMAGE_SIMSWIM1_ID), 0, 0, aSrcRect, false);
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        SetStoreColor(g, mColors[1]);
        g->DrawImageMirror(GetImageById(IMAGE_SIMSWIM2_ID), 0, 0, aSrcRect, false);
        SetStoreColor(g, mColors[0]);
        g->DrawImageMirror(GetImageById(IMAGE_SIMSWIM3_ID), 0, 0, aSrcRect, false);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }
    g->SetColorizeImages(false);
    g->Translate(-anXTrans, -anYTrans);
}

void Sexy::Fish::Sync(DataSync* theSync)
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
    theSync->SyncLong(mFoodAte);
    theSync->SyncLong(mFoodNeededToGrow);
    theSync->SyncLong(mMovementState);
    theSync->SyncLong(mSpecialMovementStateChangeTimer);
    theSync->SyncLong(mMovementStateChangeTimer);
    theSync->SyncLong(mSwimFrameCounter);
    theSync->SyncLong(mVXAbs);
    theSync->SyncLong(mAnimationFrameIndexFish);
    theSync->SyncLong(mTurnAnimationTimer);
    theSync->SyncLong(mEatingAnimationTimer);
    theSync->SyncLong(mGrowthAnimationTimer);
    theSync->SyncBool(mHasSpecialColors);
    if(mHasSpecialColors)
    {
        theSync->SyncBool(mRainbowFish);
        theSync->SyncColor(mColors[0]);
        theSync->SyncColor(mColors[1]);
        theSync->SyncColor(mColors[2]);
        if (theSync->mReader && mRainbowFish)
            UpdateRainbow();
    }
    theSync->SyncLong(mBoughtTimer);
    theSync->SyncLong(mCoinDropTimer);
    theSync->SyncLong(mCoinDropT);
    theSync->SyncBool(mBeginner);
    theSync->SyncBool(mIsGuppy);
    if(mVirtualTankId > -1)
        theSync->SyncBool(mVirtualFish);
}

bool Fish::Hungry()
{
    UpdateHungerAnimCounter();
    if (!mApp->mBoard->AliensInTank())
        UpdateHungerCounter();
    if (!ShouldDie() || mBeginner)
    {
        if (mHunger == -1 && mBeginner)
        {
            if (mApp->mBoard->m0x4b0[23])
                mApp->mBoard->ShowText("Fish are hungry! Click to drop food!", true, 24);
            if (mApp->mBoard->m0x4b0[8])
                mApp->mBoard->ShowText("Fish are hungry! Click to drop food!", true, 23);

            mApp->mBoard->ShowText("Fish are hungry! Click to drop food!", true, 8);
            return false;
        }
        if (mHunger == -200 && mBeginner && !mApp->mBoard->m0x4b0[21])
        {
            mApp->mBoard->ShowText("Fish are REALLY hungry! Click to feed!", true, 21);
            mApp->mBoard->PlaySample(SOUND_AWOOGA_ID, 3, 1.0);
            return false;
        }
        if (mHunger == -400 && mBeginner)
        {
            mApp->mBoard->ShowText("Fish are about to die of hunger! Click on aquarium!", true, 22);
            return false;
        }
        if (mHunger < -499 && mBeginner)
        {
            Die();
            return false;
        }
        if (mHunger < 500)
        {
            if (!mApp->mBoard->AliensInTank() || 
                (!mApp->mBoard->mAlienList->empty() && mApp->mBoard->mAlienList->at(0)->mAlienType != Alien::ALIEN_GUS))
                return HungryBehavior();
            /*if (!mApp->mBoard->mAlienList.empty())
                if(mApp->mAlienList[0]->mAlienType != Alien::ALIEN_GUS)
                    return HungryBehavior();*/
        }
    }
    else
        Die();
    return false;
}

void Sexy::Fish::DrawFish(Graphics* g, bool mirror)
{
    int aRow = mSize;
    if (mSize > 2)
    {
        if (mSize == 3)
            aRow = 2;
        else
        {
            aRow = 3;
            if (mSize != 4)
                aRow = mirror;
        }
    }
    if (gZombieMode)
    {
        bool aHungr = IsHungryVisible();
        g->DrawImageMirror(IMAGE_SMALLDIE, 0, 0, Rect(aHungr ? 480 : 720, aRow * 80, 80, 80), mirror);
        if (mMisslePtr)
            DrawCrosshair(g, 0, 0);
        if (IsHungryBlipPointer(500))
            g->DrawImageCel(IMAGE_MISCITEMS, 0, -5, 2);
        return;
    }

    bool aHungr = IsHungryVisible();
    Image* anImg = GetImageToDraw(aHungr);
    Rect aSrcRect = Rect(mAnimationFrameIndexFish * 80, aRow * 80, 80, 80);
    Rect aDestRect = Rect(0, 0, 80, 80);
    if (mInvisible && mGrowthAnimationTimer == 0)
        if(DrawInvisibleEffect(g, anImg, aSrcRect, mirror))
             return;
    bool doMainDraw = false;
    if (mGrowthAnimationTimer < 1 && mSize != 3)
    {
        doMainDraw = true;
    }
    else
    {
        if (mSize != TYPE_STAR_GUPPY)
        {
            double aGrowthVal = 0;
            if (mGrowthAnimationTimer > 3)
                aGrowthVal = ((10 - mGrowthAnimationTimer) * 0.7) / 7.0 + 0.5;
            else
                aGrowthVal = (mGrowthAnimationTimer * 0.2) / 3 + 1.0;

            int aVal = (aGrowthVal - 1.0) * 40.0; // 80.0 * 0.5

            aDestRect.mX -= aVal;
            aDestRect.mY -= aVal;
            aDestRect.mWidth += aVal*2;
            aDestRect.mHeight += aVal*2;

            g->SetFastStretch(!mApp->Is3DAccelerated());
            doMainDraw = true;
        }
        else
        {
            SetColorHelper(g, Color(255, 255, 255, 155));
            g->DrawImageMirror(anImg, aDestRect, aSrcRect, mirror);
            if (!aHungr || mHungerAnimationTimer != 0)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                int anAlpha = 0;
                if (mHungerAnimationTimer == 0)
                    anAlpha = 200;
                else
                    anAlpha = ((5 - mHungerAnimationTimer) * 200) / 5;
                g->SetColor(Color(255, 255, 255, anAlpha));
                g->DrawImageMirror(anImg, aDestRect, aSrcRect, mirror);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
            }
        }
    }

    if (doMainDraw)
    {
        if (!mHasSpecialColors || aHungr)
        {
            SetColorHelper(g, Color::White);
            g->DrawImageMirror(anImg, aDestRect, aSrcRect, mirror);
        }
        else
        {
            int anImgId = IMAGE_SIMSWIM1_ID;
            if (mTurnAnimationTimer == 0)
            {
                if (mEatingAnimationTimer > 0 || mVoraciousScreamCounter > 100)
                    anImgId = IMAGE_SIMEAT1_ID;
            }
            else
                anImgId = IMAGE_SIMTURN1_ID;

            SetColorHelper(g, Color::White);
            Image* anImgToDraw = GetImageById(anImgId);
            g->DrawImageMirror(anImgToDraw, aDestRect, aSrcRect, mirror);

            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            SetColorHelper(g, mColors[1]);
            anImgToDraw = GetImageById(anImgId + 1);
            g->DrawImageMirror(anImgToDraw, aDestRect, aSrcRect, mirror);

            SetColorHelper(g, mColors[0]);
            anImgToDraw = GetImageById(anImgId + 2);
            g->DrawImageMirror(anImgToDraw, aDestRect, aSrcRect, mirror);
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        }
        g->SetColorizeImages(false);
        if (mHungerAnimationTimer != 0)
        {
            Image* anImgtoDraw = GetImageToDraw(true);
            Color aClr = Color(255, 255, 255, (mHungerAnimationTimer * 255) / 5);
            SetColorHelper(g, aClr);
            g->DrawImageMirror(anImgtoDraw, aDestRect, aSrcRect, mirror);
        }
    }
    g->SetColorizeImages(false);

    if (mMisslePtr)
        DrawCrosshair(g, 0, 0);
    if (IsHungryBlipPointer(500))
        g->DrawImageCel(IMAGE_MISCITEMS, 0, -5, 2);
}

bool Fish::HungryBehavior()
{
    GameObject* aNearestFood = FindNearestFood();

    int aCenterX = mXD + 40.0;
    int aCenterY = mYD + 40.0;

    if (mExoticDietFoodType != 0)
        aCenterY += 20;

    if (mSpecialMovementStateChangeTimer > 2)
    {
        if (!aNearestFood) 
            return false;
        mSpecialMovementStateChangeTimer = 0;
        int aFCX = aNearestFood->mX + aNearestFood->mWidth / 2;
        int aFCY = aNearestFood->mY + aNearestFood->mHeight / 2;
        if (mHunger < 301)
        {
            if (aCenterX > aFCX + 8) {
                if (mVX > -4.0) mVX -= 1.3;
            }
            else if (aCenterX < aFCX - 8) {
                if (mVX < 4.0) mVX += 1.3;
            }
            else if (aCenterX > aFCX + 4) {
                if (mVX > -4.0) mVX -= 0.2;
            }
            else if (aCenterX < aFCX - 4) {
                if (mVX < 4.0) mVX += 0.2;
            }
            else if (aCenterX > aFCX) {
                if (mVX > -4.0) mVX -= 0.05;
            }
            else if (aCenterX < aFCX) {
                if (mVX < 4.0) mVX += 0.05;
            }

            if (aCenterY > aFCY + 6) {
                if (mVY > -3.0) mVY -= 1.0; 
            }
            else if (aCenterY < aFCY - 6) {
                if (mVY < 4.0) mVY += 1.3; 
            }
            else if (aCenterY > aFCY) {
                if (mVY > -3.0) mVY -= 0.5; 
            }
            else if (aCenterY < aFCY) {
                if (mVY < 4.0) mVY += 0.7; 
            }
        }
        else
        {
            if (aCenterX > aFCX + 8) {
                if (mVX > -3.0) mVX -= 1.0;
            }
            else if (aCenterX < aFCX - 8) {
                if (mVX < 3.0) mVX += 1.0;
            }
            else if (aCenterX > aFCX + 4) {
                if (mVX > -3.0) mVX -= 0.1;
            }
            else if (aCenterX < aFCX - 4) {
                if (mVX < 3.0) mVX += 0.1;
            }
            else if (aCenterX > aFCX) {
                if (mVX > -3.0) mVX -= 0.05;
            }
            else if (aCenterX < aFCX) {
                if (mVX < 3.0) mVX += 0.05;
            }

            if (aCenterY > aFCY + 6) {
                if (mVY > -2.0) mVY -= 0.6;
            }
            else if (aCenterY < aFCY - 6) {
                if (mVY < 3.0) mVY += 1.0;
            }
            else if (aCenterY > aFCY) {
                if (mVY > -2.0) mVY -= 0.3;
            }
            else if (aCenterY < aFCY) {
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

GameObject* Fish::FindNearestFood()
{
    if (mExoticDietFoodType != 0)
        return FindNearestExoticFood(mX + mWidth/2, mY + 20 + mHeight/2);

    int aCenterX = mXD + 40.0;
    int aCenterY = mYD + 40.0;

    Board* aBoard = mApp->mBoard;
    int aDist = 100000000;
    Food* aRet = nullptr;
    for (int i = 0; i < aBoard->mFoodList->size(); i++)
    {
        Food* aFood = aBoard->mFoodList->at(i);
        
        if (CanEatFood(aFood))
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

void Fish::CollideWithFood()
{
    Board* aBoard = mApp->mBoard;
    int aCenterX = mXD + 40.0;
    int aCenterY = mYD + 40.0;
    if (mExoticDietFoodType == 0)
    {
        for (int i = 0; i < aBoard->mFoodList->size(); i++)
        {
            Food* aFood = aBoard->mFoodList->at(i);

            if (aCenterX > aFood->mX + 5 && aCenterX < aFood->mX + 35 &&
                aCenterY > aFood->mY && aCenterY < aFood->mY + 35) 
            {
                if (CanEatFood(aFood))
                {
                    OnFoodAte(aFood);
                    aFood->Remove();
                    if (mEatingAnimationTimer == 0)
                    {
                        ShowInvisibility();
                        mEatingAnimationTimer = 8;
                    }
                    return;
                }
            }
            
            if (mEatingAnimationTimer == 0) 
            {
                if (aCenterX > aFood->mX - 10 && aCenterX < aFood->mX + 50 &&
                    aCenterY > aFood->mY - 5 && aCenterY < aFood->mY + 40)
                {
                    if (CanEatFood(aFood)) 
                    {
                        ShowInvisibility();
                        mEatingAnimationTimer = 20;
                    }
                }
            }
        }
    }
    else
    {
        int aExoticFoodVal = ExoticFoodCollision(mX + mWidth / 2, mY + mHeight/2 + 20);
        if (mEatingAnimationTimer == 0)
        {
            if (aExoticFoodVal == 1)
            {
                ShowInvisibility();
                mEatingAnimationTimer = 8;
            }
            else if (aExoticFoodVal == 2)
            {
                ShowInvisibility();
                mEatingAnimationTimer = 20;
            }
        }
    }
}

void Fish::DropCoin()
{
    if (mSize < GameObject::TYPE_MEDIUM_GUPPY && mVirtualTankId < 0)
        return;

    mCoinDropTimer++;
    if (!CoinDropTimerPassed(mCoinDropTimer, gMerylActive ? 35 : mCoinDropT))
        return;

    mCoinDropTimer = 0;

    if (RelaxModeCanDrop())
    {
        int aCoinType = -999;
        if (mPreNamedTypeId == ROCKY)
            aCoinType = 7;
        else if(mPreNamedTypeId == SANTA)
        {
            if (mSize > GameObject::TYPE_MEDIUM_GUPPY)
                aCoinType = 7;
        }
        else
        {
            aCoinType = mSize;
            if (mSize == GameObject::TYPE_MEDIUM_GUPPY)
                aCoinType = Coin::COIN_SILVER_C;
            else if (mSize == GameObject::TYPE_BIG_GUPPY)
                aCoinType = Coin::COIN_GOLD_C;
            if (!mVirtualFish)
            {
                if (mSize < 1)
                    aCoinType = -999;
            }
            else if (mSize == GameObject::TYPE_MEDIUM_GUPPY)
                aCoinType = 4;
            else if (mSize == GameObject::TYPE_BIG_GUPPY)
                aCoinType = 6;
            else if (mSize == GameObject::TYPE_CROWNED_GUPPY)
                aCoinType = 7;
            else
                aCoinType = 2;
        }

        if (aCoinType != -999)
            mApp->mBoard->DropCoin(mX + 5, mY + 10, aCoinType, nullptr, -1.0, 0);
    }

    if (mApp->mBoard->IsFirstLevel())
    {
        if (mApp->mBoard->m0x4b0[25])
            mApp->mBoard->ShowText("Click on coins for extra money!", true, 26);
        else if(mApp->mBoard->m0x4b0[5])
            mApp->mBoard->ShowText("Click on coins for extra money!", true, 25);
        else
            mApp->mBoard->ShowText("Click on coins for extra money!", true, 5);
    }
}

void Fish::FishUpdateAnimation()
{
    if (mRainbowFish)
        UpdateRainbow();
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
            mAnimationFrameIndexFish = 9 - (mEatingAnimationTimer / 2);
        else 
        {
            if (mVXAbs < 2) 
                mSwimFrameCounter += 1;
            else 
                mSwimFrameCounter += 2;

            if (mSwimFrameCounter >= 20)
                mSwimFrameCounter = 0;
            mAnimationFrameIndexFish = mSwimFrameCounter / 2;
        }
    }
    else 
    {
        if (mTurnAnimationTimer > 0) 
            mAnimationFrameIndexFish = 9 - (mTurnAnimationTimer / 2);
        else
            mAnimationFrameIndexFish = 9 + (mTurnAnimationTimer / 2);
    }
    
    if (mGrowthAnimationTimer > 0)
        mGrowthAnimationTimer--;

    if (mPrevVX != mVX && mPrevVX != 0 && mVX != 0)
        mPrevVX = mVX;

    if (mVoraciousScreamCounter > 100 && mTurnAnimationTimer == 0)
        mAnimationFrameIndexFish = 4;

    if (mMisslePtr)
        UpdateCrosshairAnimation();
}

void Sexy::Fish::SetFishColors(int theRandIdx, bool rainbow)
{
    mHasSpecialColors = true;
    mRainbowFish = rainbow;
    if (mType == TYPE_OSCAR)
    {
        int aRandIndex = theRandIdx % 12;
        int aRed = OSCAR_RED_TABLE1[aRandIndex] + 5;
        if (aRed > 255) aRed = 255;
        int aGreen = OSCAR_GREEN_TABLE1[aRandIndex] + 5;
        if (aGreen > 255) aGreen = 255;
        int aBlue = OSCAR_GREEN_TABLE1[aRandIndex] + 5;
        if (aBlue > 255) aBlue = 255;
        mColors[0] = Color(aRed, aGreen, aBlue);

        aRed = OSCAR_RED_TABLE2[aRandIndex] + 5;
        if (aRed > 255) aRed = 255;
        aGreen = OSCAR_GREEN_TABLE2[aRandIndex] + 5;
        if (aGreen > 255) aGreen = 255;
        aBlue = OSCAR_GREEN_TABLE2[aRandIndex] + 5;
        if (aBlue > 255) aBlue = 255;
        mColors[1] = Color(aRed, aGreen, aBlue);

        aRed = OSCAR_RED_TABLE3[aRandIndex] + 5;
        if (aRed > 255) aRed = 255;
        aGreen = OSCAR_GREEN_TABLE3[aRandIndex] + 5;
        if (aGreen > 255) aGreen = 255;
        aBlue = OSCAR_GREEN_TABLE3[aRandIndex] + 5;
        if (aBlue > 255) aBlue = 255;
        mColors[2] = Color(aRed, aGreen, aBlue);
    }
    else
    {
        int aRandIndex = theRandIdx % 30;
        int aRed = FISH_RED_TABLE1[aRandIndex] + 5;
        if (aRed > 255) aRed = 255;
        int aGreen = FISH_GREEN_TABLE1[aRandIndex] + 5;
        if (aGreen > 255) aGreen = 255;
        int aBlue = FISH_BLUE_TABLE1[aRandIndex] + 5;
        if (aBlue > 255) aBlue = 255;
        mColors[0] = Color(aRed, aGreen, aBlue);

        aRed = FISH_RED_TABLE2[aRandIndex] + 5;
        if (aRed > 255) aRed = 255;
        aGreen = FISH_GREEN_TABLE2[aRandIndex] + 5;
        if (aGreen > 255) aGreen = 255;
        aBlue = FISH_BLUE_TABLE2[aRandIndex] + 5;
        if (aBlue > 255) aBlue = 255;
        mColors[1] = Color(aRed, aGreen, aBlue);
    }
}

void Fish::RemoveFromGame(bool aRemoveShadow)
{
    if (mMisslePtr)
        mMisslePtr->Remove();

    mApp->mBoard->mWidgetManager->RemoveWidget(this);
    mApp->SafeDeleteWidget(this);
    mApp->mBoard->RemoveGameObjectFromLists(this, false);
    if (aRemoveShadow && mShadowPtr)
        mShadowPtr->Remove();
    mApp->mBoard->mGuppiesDeadCount++;
}

void Fish::Die(bool flag)
{
    if (flag)
        mApp->mBoard->PlayDieSound(mType);

    RemoveFromGame(false);
    bool isFacingRight = (mVX >= 0.0);
    mApp->mBoard->SpawnDeadFish(mXD, mYD, mVX, mVY, mSpeedMod, mSize, isFacingRight, mShadowPtr);
}

void Fish::Init(int theX, int theY)
{
    mXD = theX;
    mYD = theY;
    mClip = false;
    mType = GameObject::TYPE_GUPPY;
    mVirtualFish = false;
    mX = theX;
    mY = theY;
    mVY = -0.5;
    mWidth = 80;
    mHeight = 80;
    mPrevVX = 1.0;
    if (Rand(2))
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
    int r = Rand();
    if (r % 3 == 0)
    {
        mSpeedMod = 2.0;
    }
    else if (r % 3 == 1)
    {
        mSpeedMod = 1.8;
    }
    else if (r % 3 == 2)
    {
        mSpeedMod = 1.6;
    }
    mFoodAte = 0;
    mHunger = Rand() % 200 + 400;
    if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
    {
        mFoodNeededToGrow = Rand() % 9 + 12;
    }
    else
    {
        mFoodNeededToGrow = Rand() % 3 + 4;
    }
    mSpecialMovementStateChangeTimer = 40;
    mMovementStateChangeTimer = 0;
    mSwimFrameCounter = 0;
    mVXAbs = 0;
    mAnimationFrameIndexFish = 0;
    mTurnAnimationTimer = 0;
    mEatingAnimationTimer = 0;
    mGrowthAnimationTimer = 0;
    mHasSpecialColors = false;
    mRainbowFish = false;
    mRainbowFishDeterminant = 0;
    mCoinDropTimer = 0;
    mMovementState = Rand() % 10;
    mCoinDropT = DetermineCoinDropT(Rand() % 200 + 150);
    mBeginner = false;
    mMouseVisible = true;
    if (mApp->mBoard->IsFirstLevel())
    {
        mBeginner = true;
        if (!mApp->mBoard->mAlienList->empty() || !mApp->mBoard->mBilaterusList->empty())
            mMouseVisible = false;
    }
    mBoughtTimer = 0;
    mWadsworthVXModCounter = 0;
    mUnusedTimer = 0;
    mUnusedVXWadsworthAddon = 0;
    mIsGuppy = 1;
}

void Fish::UpdateRainbow()
{
    if (mRainbowFishDeterminant == 0)
        mRainbowFishDeterminant = Rand() % 255 + 1;

    int aValue = mUpdateCnt + mRainbowFishDeterminant;
    int aHue = abs(aValue);
    
    Color aColor = Color(mApp->HSLToRGB(aHue % 255, 200, 128));
    mColors[0] = aColor;
    aColor = Color(mApp->HSLToRGB((aHue + 60) % 255, 200, 128));
    mColors[1] = aColor;
    aColor = Color(mApp->HSLToRGB((aHue + 120) % 255, 200, 128));
    mColors[2] = aColor;
}

bool Fish::CanEatFood(Food* theFood)
{
    if (theFood->mPickedUp != 0 || theFood->mCantEatTimer != 0)
        return false;
    if (theFood->mExoticFoodType != 2)
        return (theFood->mExoticFoodType == 0);
    else
    {
        if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
            return true;
        if (mSize <= TYPE_BIG_GUPPY && mHunger < 108)
            return true;
        return false;
    }
}

Image* Sexy::Fish::GetImageToDraw(bool theFlag)
{
    if (mTurnAnimationTimer != 0)
        return GetImageById(IMAGE_SMALLTURN_ID + theFlag);
    if(mEatingAnimationTimer < 1 && mVoraciousScreamCounter < 101)
        return GetImageById(theFlag ? IMAGE_HUNGRYSWIM_ID : IMAGE_SMALLSWIM_ID);

    return GetImageById(IMAGE_HUNGRYEAT_ID + (4*(!theFlag)));
}

void Sexy::Fish::ClickedBehavior(int theX, int theY)
{
    if (mTurnAnimationTimer != 0 || mEatingAnimationTimer != 0 || mSpecialMovementStateChangeTimer < 3)
        return;

    if (Rand() % 75 > 100) // Old code :)
        return;

    int aCenterX = theX - (mX + 40);
    int aCenterY = theY - (mY + 40);

    if (aCenterX * aCenterX + aCenterY * aCenterY > 8099)
        return;

    mMovementState = 123;
    mSpecialMovementStateChangeTimer = 0;
    mMovementStateChangeTimer = 0;

    int aDirX;
    int aDirY;

    if (mVX >= 0.0 || aCenterX + 20 < theX)
    {
        if (mVX <= 0.0 || mWidth - 20 + aCenterX >= theX)
        {
            aDirX = -1;
            if (mVX <= 0.0)
                aDirX = 1;
        }
        else
            aDirX = -1;
    }
    else
        aDirX = 1;

    aDirY = aCenterY < 0 ? 1 : -1;

    if (Rand() % 100 < 26)
        aDirY = -aDirY;

    mVX = (Rand() % 2 == 0) ? 4.0 : 6.0;
    mVY = (Rand() % 2 == 0) ? 1.0 : 3.0;

    mVX = mVX * aDirX;
    mVY = mVY * aDirY;

    bool isTurning = (mPrevVX < 0.0 && mVX > 0.0) || (mPrevVX > 0.0 && mVX < 0.0);
    if (isTurning) 
        mTurnAnimationTimer = (mVX > 0.0) ? -20 : 20;

    mPrevVX = mVX;
}

void Sexy::Fish::FishOnGrow()
{
    Board* aBoard = mApp->mBoard;
    int aTank = aBoard->mTank;
    int aLevel = aBoard->mLevel;
    int aFinishedGame = mApp->mCurrentProfile->mFinishedGame;
    if (mSize == 1)
    {
        if (!aBoard->mSlotUnlocked[Board::SLOT_GUPPY])
        {
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_GUPPY, true);
            if (aTank == 1 && aLevel == 1 && !aFinishedGame)
            {
                aBoard->m0x4b0[2] = true;
                if (aBoard->mMessageWidget->m0xd0 < 1)
                {
                    aBoard->ShowText("Your fish has grown! Good work!", false, 9);
                    return;
                }
            }
        }
    }
    else if (mSize == 2) // 33
    {
        if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_EGG, true);
        if (aTank == 1)
        {
            if (aLevel == 1)
            {
                if (!aBoard->mSlotUnlocked[Board::SLOT_EGG])
                {
                    aBoard->MakeAndUnlockMenuButton(Board::SLOT_EGG, true);
                    aBoard->m0x4b0[2] = false;
                    aBoard->m0x4b0[40] = true;
                    if (!aFinishedGame && aBoard->mMessageWidget->m0xd0 < 1)
                    {
                        aBoard->ShowText("Buy 3 egg pieces to complete level!", false, 7);
                        return;
                    }
                }
            }
            else if (aLevel == 2)
            {
                if (!aBoard->mSlotUnlocked[Board::SLOT_FOODLVL])
                {
                    aBoard->m0x4b0[15] = true;
                    if (!aFinishedGame)
                        aBoard->ShowText("Upgrade Food Quality to make food more nourishing!", false, 11);
                    aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLVL, true);
                    return;
                }
            }
            else
            { //53
                aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLVL, true);
                aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLIMIT, true);
                aBoard->MakeAndUnlockMenuButton(Board::SLOT_OSCAR, true);
            }
        }
        else if (aTank == 2)
        {
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLVL, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLIMIT, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_POTION, true);
            if (aLevel == 1)
            {
                aBoard->MakeAndUnlockMenuButton(Board::SLOT_EGG, true);
                return;
            }
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_STARCATCHER, true);
            return;
        }
        else if (aTank == 3)
        {
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLVL, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLIMIT, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_GRUBBER, true);
            return;
        }
        else if (aTank == 4)
        {
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLVL, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_FOODLIMIT, true);
            aBoard->MakeAndUnlockMenuButton(Board::SLOT_OSCAR, true);
            return;
        }
    }
}

bool Sexy::Fish::WadsworthActive()
{
    if (gWadsworthTimer != 0 && mType == TYPE_GUPPY)
        return mSize < SIZE_LARGE;
    return false;
}


