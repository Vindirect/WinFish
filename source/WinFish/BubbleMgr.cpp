#include "BubbleMgr.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"

using namespace Sexy;
const double PI = 3.141590118408203;

const int BF_RED_TABLE1[30] = {
    125, 255, 135, 175, 195, 255, 255,  60, 255, 190,
    190, 145, 240, 255, 210, 175, 255,  65, 240, 240,
    255, 255, 175, 225, 130, 160, 205, 190, 240, 210
};

const int BF_GREEN_TABLE1[30] = {
    210, 240, 250, 235, 140, 255, 255,  60,  65,  60,
    190, 145, 145, 255, 215, 240, 180,  70, 240, 225,
    230,  40, 215, 175, 165, 230,  50, 140, 250, 250
};

const int BF_BLUE_TABLE1[30] = {
    255,  60, 245, 135,  90,  85, 255,  60,  65, 255,
    190, 175, 235,  40, 175, 175, 110, 160, 240, 125,
      0,   0, 110,  95, 145,  50, 180, 250, 240,  50
};

const int BF_RED_TABLE2[30] = {
     50,  45, 255,  50,  65, 250, 230,   0,  75, 255,
    250,  30, 110, 210, 110,  30, 210, 240,   0, 240,
      0, 255,  15,  65, 125, 145, 255, 250,  80, 110
};

const int BF_GREEN_TABLE2[30] = {
    110,  95,   0, 115, 120, 155, 130,   0,  75,   0,
    250,  25,  10,  10,  95,  80,  10, 240,   5,   5,
    125, 225,  30,  55,  15,   0, 145, 245, 250, 210
};

const int BF_BLUE_TABLE2[30] = {
    210, 195, 125, 210,  65,   0, 250,   0,  75, 125,
    250, 225, 225,   0, 210, 125,   0, 240, 130, 240,
      0,   0,  15, 190,  15, 210, 220, 175, 145,   0
};

BubbleMgr::BubbleMgr()
{
    mBubbleBounds = Rect(0, 0, 640, 480);
    mBubbleFishBounds = Rect(0, 0, 640, 480);
   
    mMaxFish = 0;
    mDefaultBubbleFishVY = 0.0f;

    mMaxBubbles = 10;
    mBubbleSpawnChance = 10;
    mFishSpawnChance = 10;

    mBubbleList.clear();

    mClipBubbleFish = true;
}

BubbleMgr::~BubbleMgr()
{
}

void BubbleMgr::Update()
{
    std::list<Bubble*>::iterator itParticle = mBubbleList.begin();
    while (itParticle != mBubbleList.end())
    {
        Bubble* aBubble = *itParticle;
        aBubble->Update();

        if (aBubble->mY < mBubbleBounds.mY)
        {
            itParticle = mBubbleList.erase(itParticle);
        }
        else
            ++itParticle;
    }

    if (mBubbleList.size() < mMaxBubbles)
        if ((Rand() % 100) < mBubbleSpawnChance)
            SpawnRandomBubble();

    BubbleFishList::iterator itFish = mFishList.begin();
    while (itFish != mFishList.end())
    {
        BubbleFish* aFish = &(*itFish);
        aFish->Update();

        bool shouldRemove = false;

        if (aFish->mVX == 0)
            shouldRemove = true;
        if (aFish->mX < (mBubbleFishBounds.mX - 80))
            shouldRemove = true;
        if (aFish->mX > (mBubbleFishBounds.mX + mBubbleFishBounds.mWidth))
            shouldRemove = true;

        if (shouldRemove)
        {
            itFish = mFishList.erase(itFish);
        }
        else
            ++itFish;
    }

    if (mFishList.size() < mMaxFish)
        if ((Rand() % 100) < mFishSpawnChance)
            SpawnRandomBubbleFish();
}

void BubbleMgr::Draw(Graphics* g)
{
    Graphics* g1 = new Graphics(*g);
    g1->ClipRect(mBubbleBounds);
    std::list<Bubble*>::iterator anIterator = mBubbleList.begin();
    while (anIterator != mBubbleList.end())
    {
        Bubble* aCurrentBubble = *anIterator;

        if (aCurrentBubble != NULL)
            aCurrentBubble->Draw(g);

        ++anIterator;
    }

    Graphics* g2 = new Graphics(*g);
    if (mClipBubbleFish)
        g2->ClipRect(mBubbleFishBounds);
    BubbleFishList::iterator itFish = mFishList.begin();
    while (itFish != mFishList.end())
    {
        BubbleFish* aFish = &(*itFish);

        if (aFish != NULL)
            aFish->Draw(g2);

        ++itFish;
    }
}

void BubbleMgr::SpawnBubble(int theX, int theY)
{
    Bubble* aBubble = new Bubble();

    mBubbleList.push_back(aBubble);

    aBubble->mX = theX;
    aBubble->mY = theY;
}

void BubbleMgr::SpawnRandomBubble()
{
    if (mBubbleBounds.mWidth <= 20)
        return;

    int aRandVal = Rand();
    SpawnBubble(aRandVal % (mBubbleBounds.mWidth-20) + mBubbleBounds.mX, mBubbleBounds.mHeight + mBubbleBounds.mY);
}

void Sexy::BubbleMgr::SpawnRandomBubbleFish()
{
    if (mBubbleFishBounds.mHeight <= 80)
        return;

    BubbleFish aFish = BubbleFish();

    mFishList.push_back(aFish);
    BubbleFish* aFishInList = &mFishList.back();

    int aRandVal = Rand();
    aFishInList->mY = aRandVal % (mBubbleFishBounds.mHeight - 80) + mBubbleFishBounds.mY;
    aFishInList->mVY = mDefaultBubbleFishVY;

    aRandVal = Rand();
    if (aRandVal % 2 == 0)
        aFishInList->mX = mBubbleFishBounds.mX - 80;
    else
    {
        aFishInList->mX = mBubbleFishBounds.mWidth + mBubbleFishBounds.mX;
        aFishInList->mVX *= -1;
    }
}

void Sexy::BubbleMgr::SetBubbleConfig(int theMaxBubbles, int theBubbleSpawnChance)
{
    mMaxBubbles = theMaxBubbles;
    mBubbleSpawnChance = theBubbleSpawnChance;
}

void Sexy::BubbleMgr::SetBubbleFishConfig(int theMaxBubbleFish, int theBubbleFishSpawnChance)
{
    mMaxFish = theMaxBubbleFish;
    mFishSpawnChance = theBubbleFishSpawnChance;
}

void Sexy::BubbleMgr::SetDefaultBubbleFishVY(float theDefaultVY)
{
    if (theDefaultVY == mDefaultBubbleFishVY)
        return;

    mDefaultBubbleFishVY = theDefaultVY;

    BubbleFishList::iterator itFish = mFishList.begin();
    while (itFish != mFishList.end())
    {
        BubbleFish* aFish = &(*itFish);
        aFish->mVY = theDefaultVY;
        ++itFish;
    }
}

void Sexy::BubbleMgr::UpdateALot()
{
    for (int i = 0; i < 500; i++)
        Update();
}

const float gBubbleParticleSpeed1 = 2.0f;
const float gBubbleParticleSpeed2 = 2.5f;   
const float gBubbleParticleSpeed3 = 3.0f;
const float gBubbleParticleSpeed4 = 3.2f;
const float gBubbleParticleSpeed5 = 2.3f;
const float gBubbleParticleSpeed6 = 2.8f;

Bubble::Bubble()
{
    unsigned int aRandVal = Rand();
    aRandVal = aRandVal & 0x80000003;
    if ((int)aRandVal < 0)
        aRandVal = (aRandVal - 1 | 0xFFFFFFFC) + 1;
    mType = aRandVal;

    if (mType == 3) {
        aRandVal = Rand();
        aRandVal = aRandVal & 0x80000003;
        if ((int)aRandVal < 0) 
            aRandVal = (aRandVal - 1 | 0xFFFFFFFC) + 1;
        mType = aRandVal;
    }

    aRandVal = Rand();
    int anOffsetY = (aRandVal % 6) + 400;
    mY = anOffsetY;

    aRandVal = Rand();
    int anOffsetX = (aRandVal % 22) + 150;
    mX = anOffsetX;

    mShakeOffset = 0;

    aRandVal = Rand();
    int aSpeedChoice = aRandVal % 6;

    switch (aSpeedChoice)
    {
    case 0:
        mVY = gBubbleParticleSpeed1;
        break;
    case 1:
        mVY = gBubbleParticleSpeed2;
        break;
    case 2:
        mVY = gBubbleParticleSpeed3;
        break;
    case 3:
        mVY = gBubbleParticleSpeed4;
        break;
    case 4:
        mVY = gBubbleParticleSpeed5;
        break;
    case 5:
        mVY = gBubbleParticleSpeed6;
        break;
    }
}

void Bubble::Draw(Graphics* g)
{
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    g->DrawImageCel(IMAGE_BUBBLES, mX + mShakeOffset, mY, mType);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Bubble::Update()
{
    mShakeOffset = Rand() % 2;
    mY -= mVY;
}

Sexy::BubbleFish::BubbleFish()
{
    mVX = 2.0;
    mUpdateCnt = 0;
    mTurnTimer = 0;
    mVY = 0.0;
    mX = 0;
    mY = 0;

    switch (Rand() % 6)
    {
    case 0:
        mVX = 1.3;
        break;
    case 1:
        mVX = 1.7;
        break;
    case 2:
        mVX = 2;
        break;
    case 3:
        mVX = 2.5;
        break;
    case 4:
        mVX = 3;
        break;
    case 5:
        mVX = 3.5;
        break;
    default:
        break;
    }
    int aRandVal = Rand();
    if (aRandVal % 10 < 5)
        mSize = 0;
    else
        mSize = (7 < aRandVal % 10) + 1;

    aRandVal = Rand();
    aRandVal %= 30;
    int aRed = BF_RED_TABLE1[aRandVal] + 5;
    if (aRed > 255) aRed = 255;
    int aGreen = BF_GREEN_TABLE1[aRandVal] + 5;
    if (aGreen > 255) aGreen = 255;
    int aBlue = BF_BLUE_TABLE1[aRandVal] + 5;
    if (aBlue > 255) aBlue = 255;
    mColor1 = Color(aRed, aGreen, aBlue);

    aRed = BF_RED_TABLE2[aRandVal] + 5;
    if (aRed > 255) aRed = 255;
    aGreen = BF_GREEN_TABLE2[aRandVal] + 5;
    if (aGreen > 255) aGreen = 255;
    aBlue = BF_BLUE_TABLE2[aRandVal] + 5;
    if (aBlue > 255) aBlue = 255;
    mColor2 = Color(aRed, aGreen, aBlue);
}

void Sexy::BubbleFish::Draw(Graphics* g)
{
    Rect aSrcRect = Rect(((mUpdateCnt / 2) % 10) * 80, mSize * 80, 80, 80);

    float aRad = ((mUpdateCnt * 5) * PI) / 180.0;
    float aSin = sinf(aRad) * 3.0;

    int anImageId = IMAGE_SIMSWIM1_ID;
    bool flip = mVX > 0.0;
    if (mTurnTimer > 0 && mTurnTimer <= 5)
    {
        aSrcRect.mX = (5 - mTurnTimer) * 160;
        flip = !flip;
        anImageId = IMAGE_SIMTURN1_ID;
    }

    Image* anImage = GetImageById(anImageId);
    g->DrawImageMirror(anImage, mX, mY + aSin, aSrcRect, flip);
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    g->SetColorizeImages(true);
    g->SetColor(mColor2);

    anImage = GetImageById(anImageId+1);
    g->DrawImageMirror(anImage, mX, mY + aSin, aSrcRect, flip);
    g->SetColor(mColor1);

    anImage = GetImageById(anImageId + 2);
    g->DrawImageMirror(anImage, mX, mY + aSin, aSrcRect, flip);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    g->SetColorizeImages(false);
}

void Sexy::BubbleFish::Update()
{
    mUpdateCnt++;
    mX += mVX;
    mY += mVY;
    if (mTurnTimer > 0)
        mTurnTimer--;
}
