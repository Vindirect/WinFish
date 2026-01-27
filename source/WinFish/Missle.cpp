#include "Missle.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Board.h"
#include "Bilaterus.h"
#include "BilaterusHead.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Res.h"

Sexy::Missle::Missle()
{
    mTarget = 0;
    m0x1a8 = 0;
    mClip = false;
    mType = TYPE_MISSLE;
}

Sexy::Missle::Missle(int x, int y, GameObject* theTarget, int theType)
{
    mXD = x;
    mType = TYPE_MISSLE;
    mYD = y;
    mClip = false;
    mMissleType = theType;
    mTarget = theTarget;
    mX = mXD;
    mY = mYD;
    mVX = 0;
    mVY = 0;
    mWidth = 80;
    mHeight = 80;
    m0x194 = 1;
    m0x198 = 1;
    m0x1a8 = 0;
    m0x1ac = 10;
    m0x17c = 15;
    m0x180 = 0;
    m0x1a0 = Rand() % 4;
    mMouseVisible = false;
    if (mMissleType == MISSLE_BALL)
    {
        mSpeedMod = 0.4;
        mWidth = 50;
        mHeight = 50;
    }
    else
        mSpeedMod = 0.8;
    m0x190 = false;
    bool flag = IsTargetless();
    if (flag)
    {
        int aRandVal = mApp->mSeed->Next() % 6;
        if (aRandVal == 0)
            mVX = -6;
        else if (aRandVal == 1)
            mVX = -5;
        else if (aRandVal == 2)
            mVX = -4;
        else if (aRandVal == 3)
            mVX = 4;
        else if (aRandVal == 4)
            mVX = 5;
        else if (aRandVal == 5)
            mVX = 6;

        aRandVal = mApp->mSeed->Next() % 6;
        if (aRandVal == 0)
            mVY = -6;
        else if (aRandVal == 1)
            mVY = -5;
        else if (aRandVal == 2)
            mVY = -4;
        else if (aRandVal == 3)
            mVY = 4;
        else if (aRandVal == 4)
            mVY = 4;
        else if (aRandVal == 5)
            mVY = 4;

        m0x190 = true;
    }
}

void Sexy::Missle::Update()
{
    if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
        return;

    UpdateCounters();
    if (m0x1a8 > 0)
    {
        m0x1a8++;
        if (m0x1a8 >= m0x1ac)
        {
            Remove();
            return;
        }
    }

    if (ChaseTarget()) return;
    
    if (mMissleType == MISSLE_CLASSIC || (mMissleType == MISSLE_ENERGYBALL && !m0x190))
    {
        if (mXD > 550.0)
            mXD = 550.0;
        if (mXD < 10.0)
            mXD = 10.0;
        if (mYD > 370.0)
            mYD = 370.0;
        if (mYD < 95.0)
            mYD = 95.0;
    }
    else
    {
        if ((mMissleType == MISSLE_ENERGYBALL && m0x190) || (mMissleType == MISSLE_BALL || IsTargetless()))
        {
            if (mXD > 580.0 || mXD < -20.0 || mYD > 380.0 || mYD < 45.0)
            {
                Remove();
                return;
            }
        }
    }

    if (mMissleType == MISSLE_BALL && m0x190)
        mVY += 0.2;

    mXD += mVX / mSpeedMod;
    mYD += mVY / mSpeedMod;
    if (m0x17c > 0)
        m0x17c--;
    Move(mXD, mYD);

    if (mMissleType == MISSLE_CLASSIC && mApp->mSeed->Next() % 3 == 0)
    {
        mApp->mBoard->SpawnShot(mX + mVX * -20 + 17 + mApp->mSeed->Next() % 5, mY + mVY * -16 + 17 + mApp->mSeed->Next() % 5, 8);
    }
    UpdateAnimations();
    MarkDirty();
}

void Sexy::Missle::Draw(Graphics* g)
{
    UpdateFishSongMgr();
    bool doMirror = (mMissleType == MISSLE_ENERGYBALL || mMissleType == MISSLE_BALL || IsTargetless());
    DrawHelper(g, doMirror);
}

void Sexy::Missle::VFT74()
{
    if (mTarget)
    {
        mTarget->mMisslePtr = nullptr;
        mTarget = nullptr;
    }
    if (!IsTargetless())
        mApp->mBoard->Unk12();
}

void Sexy::Missle::Remove()
{
    if (mTarget != nullptr)
    {
        mTarget->mMisslePtr = nullptr;
        mTarget = nullptr;
    }
    mApp->mBoard->mWidgetManager->RemoveWidget(this);
    mApp->SafeDeleteWidget(this);
    mApp->mBoard->RemoveGameObjectFromLists(this, false);
    if (!IsTargetless())
        mApp->mBoard->Unk12();
    
    if (mMissleType != MISSLE_BALL)
    {
        if (mMissleType == MISSLE_ENERGYBALL)
            mApp->mBoard->PlaySample(SOUND_EXPLOSION4_ID, 3, 1.0);
        mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
        for (int i = mApp->mSeed->Next() % 3 + 2; i > 0; i--)
        {
            mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 30 - 10 + mX, mApp->mSeed->Next() % 30 - 10 + mY, mApp->mSeed->Next() % 3 + 3);
        }
    }
}

void Sexy::Missle::OnFoodAte(GameObject* obj)
{
    obj->mMisslePtr = nullptr;
    if (obj->mType == TYPE_PENTA)
        ((Penta*)obj)->Die(true);
    else if (obj->mType == TYPE_GRUBBER)
        ((Grubber*)obj)->Die(true);
    else if (obj->mType == TYPE_GUPPY || obj->mType == TYPE_OSCAR || obj->mType == TYPE_ULTRA || obj->mType == TYPE_GEKKO)
        ((Fish*)obj)->Die(true);
    else if (obj->mType == TYPE_BREEDER)
        ((Breeder*)obj)->Die(true);
    else if (obj->mType == TYPE_OTHER_TYPE_PET)
        ((OtherTypePet*)obj)->Remove();
    else if (obj->mType == TYPE_FISH_TYPE_PET)
        ((FishTypePet*)obj)->Remove();
    Remove();
}

void Sexy::Missle::Sync(DataSync* theSync)
{
    GameObject::Sync(theSync);
    theSync->SyncDouble(mXD);
    theSync->SyncDouble(mYD);
    theSync->SyncDouble(mVX);
    theSync->SyncDouble(mVY);
    theSync->SyncLong(m0x178);
    theSync->SyncLong(m0x17c);
    theSync->SyncLong(m0x180);
    theSync->SyncDouble(mSpeedMod);
    theSync->SyncBool(m0x190);
    theSync->SyncLong(m0x194);
    theSync->SyncLong(m0x198);
    theSync->SyncLong(mMissleType);
    theSync->SyncLong(m0x1a0);
    theSync->SyncLong(m0x1a8);
    if (m0x1a8 > 0)
    {
        theSync->SyncLong(m0x1ac);
        theSync->SyncLong(m0x1b0);
        theSync->SyncLong(m0x1b4);
    }
    theSync->SyncPointer((void**) &mTarget);
}

bool Sexy::Missle::IsTargetless()
{
    if (mMissleType == MISSLE_BONE || mMissleType == MISSLE_HEAD1 || mMissleType == MISSLE_HEAD2)
        return true;
    return false;
}

void Sexy::Missle::UpdateAnimations()
{
    if (mMissleType == MISSLE_ENERGYBALL)
        m0x194 = (m0x194 + 1) % 5;
    else if (mMissleType == MISSLE_BALL)
    {
        if (!m0x190)
        {
            if (mVX < 0.0)
                m0x180 = (m0x180 + 2) % 20;
            else
            {
                m0x180 = (m0x180 - 2);
                if (m0x180 < 0) m0x180 += 20;
            }
        }
        else
        {
            if (mVX < 0.0)
                m0x180 = (m0x180 + 1) % 20;
            else
            {
                m0x180 = (m0x180 - 1);
                if (m0x180 < 0) m0x180 += 20;
            }
        }

        m0x194 = m0x180 / 2;
    }
    else
    {
        if (IsTargetless())
        {
            m0x194 = (m0x194 + 1) % 10;
            if (m0x194 == 0)
                m0x198++;
        }
        else
        { // 31
            if (mVX > -0.8 && mVX < 0.8)
            {
                if (mVY <= 0.0)
                    m0x194 = 4;
                else
                    m0x194 = 12;
            }
            else if (mVX <= 0.0)
            {
                if (mVY <= -2.5)        m0x194 = 4;
                else if(mVY > 2.5)      m0x194 = 12;
                else if (mVY > 2.0)     m0x194 = 11;
                else if (mVY < -2.0)    m0x194 = 5;
                else if (mVY > 1.5)     m0x194 = 10;
                else if (mVY < -1.5)    m0x194 = 6;
                else if (mVY > 1.0)     m0x194 = 9;
                else if (mVY < -1.0)    m0x194 = 7;
                else                    m0x194 = 8;
            }
            else
            {
                if (mVY <= -2.5)        m0x194 = 4;
                else if (mVY > 2.5)     m0x194 = 12;
                else if (mVY > 2.0)     m0x194 = 13;
                else if (mVY < -2.0)    m0x194 = 3;
                else if (mVY > 1.5)     m0x194 = 14;
                else if (mVY < -1.5)    m0x194 = 2;
                else if (mVY > 1.0)     m0x194 = 15;
                else if (mVY < -1.0)    m0x194 = 1;
                else                    m0x194 = 0;
            }
        }
    }
}

void Sexy::Missle::DrawHelper(Graphics* g, bool flag)
{
    if (mMissleType == MISSLE_ENERGYBALL)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        Color aCol;
        if (!m0x190)
            aCol = Color(100, 100, 255, 55);
        else
            aCol = Color(175, 175, 50, 255);
        g->SetColor(aCol);
        g->DrawImageCel(IMAGE_ENERGYBALL, 0, 0, m0x194);
        g->SetColor(Color(100, 100, 255, 255));
        g->DrawImageCel(IMAGE_ENERGYBALL, 0, 0, 5);
        g->SetColorizeImages(false);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }
    else if (mMissleType == MISSLE_BALL)
    {
        g->DrawImageCel(IMAGE_BALLS, 0, 0, m0x194, m0x1a0);
    }
    else if (mMissleType == MISSLE_BONE)
    {
        g->DrawImage(IMAGE_BILATERUS, 0, 0, Rect(m0x194 * 80, 480, 80, 80));
    }
    else if (mMissleType == MISSLE_HEAD1 || mMissleType == MISSLE_HEAD2)
    {
        g->DrawImageMirror(IMAGE_BILATERUS, 0, 0, Rect(m0x194 * 80, (mMissleType == MISSLE_HEAD2 ? 1 : 4) * 80, 80, 80), (m0x198 % 2) == 0);
    }
    else
    {
        g->DrawImage(IMAGE_MISSILE, 0, 0, Rect(m0x194 * 80, 0, 80, 80));
    }
    if (m0x1a8 > 0)
    {
        int aTargetX = mX + mWidth / 2;
        int aTargetY = mY + mHeight / 2;

        int anInterpolatedX = InterpolateInt(m0x1b0, aTargetX, m0x1a8, m0x1ac, false);
        int anInterpolatedY = InterpolateInt(m0x1b4, aTargetY, m0x1a8, m0x1ac, false);

        int aDiffX = aTargetX - anInterpolatedX;
        int aDiffY = aTargetY - anInterpolatedY;

        float aVec[3] = { aDiffX, aDiffY , 0};
        NormalizeVector3(aVec);

        int anOffX = (int)(aVec[1] * 3.0f);
        int anOffY = (int)(-aVec[0] * 3.0f);

        float aProjX = anInterpolatedX + (aVec[0] * 50.0f);
        float aProjY = anInterpolatedY + (aVec[1] * 50.0f);

        float clampedX = aProjX; 
        float clampedY = aProjY;

        bool useTarget = false;

        if ((aVec[1] >= 0 && (float)aTargetY < aProjY) || (aVec[1] < 0 && (float)aTargetY > aProjY)) 
        {
            if ((aVec[0] >= 0 && (float)aTargetX < aProjX) || (aVec[0] < 0 && (float)aTargetX > aProjX)) 
            {
                useTarget = true;
            }
        }

        if (useTarget) {
            clampedX = (float)aTargetX;
            clampedY = (float)aTargetY;
        }

        int screenBaseX = (int)(clampedX - g->mTransX);
        int screenBaseY = (int)(clampedY - g->mTransY);

        int screenX = screenBaseX - anOffX;
        int screenY = screenBaseY;
        // TODO This calculation is not entirely correct, in real game it looks different.
        g->DrawImageCel(IMAGE_BALLS, screenX - 25, screenY - 25, m0x1a8 % 10, m0x1a0);
    }
}

bool Sexy::Missle::ChaseTarget()
{
    if ((mTarget == nullptr || mTarget->mMisslePtr == nullptr) && mMissleType != MISSLE_BALL)
    {
        if (!IsTargetless())
        {
            Remove();
            return true;
        }
        if (mTarget != nullptr)
            return CheckCollision();
        return false;
    }

    if (m0x190)
    {
        if (mTarget != nullptr)
            return CheckCollision();
        return false;
    }

    int aHalfWidth = 0;
    if (mMissleType == MISSLE_BALL)
    {
        if (mApp->mBoard->mBilaterusList->empty() && !mApp->mBoard->AliensInTank())
        {
            if (mTarget != nullptr)
                return CheckCollision();
            return false;
        }

        aHalfWidth = 25;
        if (!mApp->mBoard->mBilaterusList->empty())
            aHalfWidth = 40;
    }
    else
        aHalfWidth = 40;

    if (mTarget->mType == TYPE_ULTRA || (mMissleType == MISSLE_BALL && mApp->mBoard->mBilaterusList->empty()))
    {
        int aTX = mTarget->mX;
        double aMisCX = mXD + aHalfWidth;
        if (aTX + 200 < aMisCX)
        {
            if (mVX > -1.8)
                mVX -= 0.2;
        }
        else if (aTX - 40 > aMisCX)
        {
            if (mVX < 1.8)
                mVX += 0.2;
        }
        else if (aTX + 110 < aMisCX)
        {
            if (mVX > -1.4)
                mVX -= 0.2;
        }
        else if (aTX + 50 > aMisCX)
        {
            if (mVX < 1.4)
                mVX += 0.2;
        }
        else if (aTX + 80 < aMisCX)
        {
            if (mVX > -0.6)
                mVX -= 0.2;
        }
        else if (aTX + 80 > aMisCX)
        {
            if (mVX < 0.6)
                mVX += 0.2;
        }

        int aTY = mTarget->mY;
        double aMisCY = mYD + aHalfWidth;
        if (aTY + 200 < aMisCY)
        {
            if (mVY > -1.8)
                mVY -= 0.2;
        }
        else if (aTY - 40 > aMisCY)
        {
            if (mVY < 1.8)
                mVY += 0.2;
        }
        else if (aTY + 110 < aMisCY)
        {
            if (mVY > -1.4)
                mVY -= 0.2;
        }
        else if (aTY + 50 > aMisCY)
        {
            if (mVY < 1.4)
                mVY += 0.2;
        }
        else if (aTY + 80 > aMisCY)
        {
            if (mVY < 0.2)
                mVY += 0.1;
        }
        else if (aTY + 80 < aMisCY)
        {
            if (mVY > -0.2)
                mVY -= 0.1;
        }
    }
    else
    {
        // 156
        int aTX = mTarget->mX;
        double aMisCX = mXD + 40.0;
        if (aTX + 80 < aMisCX)
        {
            if (mVX > -1.8)
                mVX -= 0.2;
        }
        else if (aTX > aMisCX)
        {
            if (mVX < 1.8)
                mVX += 0.2;
        }
        else if (aTX + 60 < aMisCX)
        {
            if (mVX > -1.4)
                mVX -= 0.2;
        }
        else if (aTX + 20 > aMisCX)
        {
            if (mVX < 1.4)
                mVX += 0.2;
        }
        else if (aTX + 40 > aMisCX)
        {
            if (mVX < 0.6)
                mVX += 0.2;
        }
        else if(aTX + 40 < aMisCX)
        {
            if (mVX > -0.6)
                mVX -= 0.2;
        }

        int aTY = mTarget->mY;
        double aMisCY = mYD + 40.0;
        if (aTY + 80 < aMisCY)
        {
            if (mVY > -1.8)
                mVY -= 0.2;
        }
        else if (aTY > aMisCY)
        {
            if (mVY < 1.8)
                mVY += 0.2;
        }
        else if (aTY + 60 < aMisCY)
        {
            if (mVY > -1.4)
                mVY -= 0.2;
        }
        else if (aTY + 20 > aMisCY)
        {
            if (mVY < 1.4)
                mVY += 0.2;
        }
        else if (aTY + 40 > aMisCY)
        {
            if (mVY < 0.2)
                mVY += 0.1;
        }
        else if (aTY + 40 < aMisCY)
        {
            if (mVY > -0.2)
                mVY -= 0.1;
        }
    }
    

    if (mTarget != nullptr)
        return CheckCollision();
    return false;
}

bool Sexy::Missle::CheckCollision()
{
    if (mMissleType == MISSLE_ENERGYBALL && m0x190 && !mApp->mRelaxMode)
    {
        for (int i = 0; i < mApp->mBoard->mAlienList->size(); i++)
        {
            Alien* anAlien = mApp->mBoard->mAlienList->at(i);

            if (mXD + 40.0 < anAlien->mX + 140 && mXD + 40.0 > anAlien->mX + 30
                && mYD + 40.0 < anAlien->mY + 150 && mYD + 40.0 > anAlien->mY + 10 && !anAlien->mIsPsychosquidHealing)
            {
                anAlien->mHealth -= 30.0;
                anAlien->mHitFlashTimer = 10;
                if (anAlien->mHealth <= 0.0)
                    anAlien->Remove(true);

                Remove();
                return true;
            }
        }

        for (int i = 0; i < mApp->mBoard->mBilaterusList->size(); i++)
        {
            Bilaterus* aBil = mApp->mBoard->mBilaterusList->at(i);

            if (mXD + 25.0 < aBil->mActiveHead->mX + 70 && mXD + 25.0 > aBil->mActiveHead->mX + 10
                && mYD + 25.0 < aBil->mActiveHead->mY + 70 && mYD + 25.0 > aBil->mActiveHead->mY + 10)
            {
                aBil->mActiveHead->m0x1d8 -= 30.0;
                aBil->mActiveHead->m0x1d0 = 10;
                if (aBil->mActiveHead->m0x1d8 <= 0.0)
                    aBil->mActiveHead->Die(true);

                Remove();
                return true;
            }
        }
        Alien* aCyrax = mApp->mBoard->mCyraxPtr;
        if (aCyrax)
        {
            if (mXD + 40.0 < aCyrax->mX + 140 && mXD + 40.0 > aCyrax->mX + 30 &&
                mYD + 40.0 < aCyrax->mY + 150 && mYD + 40.0 > aCyrax->mY + 10)
            {
                aCyrax->mHealth -= 30.0;
                aCyrax->mHitFlashTimer = 10;
                if (aCyrax->mHealth <= 0.0)
                    aCyrax->Remove(true);
                Remove();
                return true;
            }

            for (int i = 0; i < mApp->mBoard->mFishTypePetList->size(); i++)
            {
                FishTypePet* aPet = mApp->mBoard->mFishTypePetList->at(i);
                if (mXD + 40.0 < aPet->mX + 70 && mXD + 40.0 > aPet->mX + 10 &&
                    mYD + 40.0 < aPet->mY + 70 && mYD + 40.0 > aPet->mY + 10)
                {
                    aPet->Remove();
                    mApp->mBoard->PlayDieSound(-1);
                    break;
                }
            }
            for (int i = 0; i < mApp->mBoard->mOtherTypePetList->size(); i++)
            {
                OtherTypePet* aPet = mApp->mBoard->mOtherTypePetList->at(i);
                if (mXD + 40.0 < aPet->mX + 70 && mXD + 40.0 > aPet->mX + 10 &&
                    mYD + 40.0 < aPet->mY + 70 && mYD + 40.0 > aPet->mY + 10)
                {
                    aPet->Remove();
                    mApp->mBoard->PlayDieSound(-1);
                    return false;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mFishList->size(); i++)
        {
            Fish* aFish = mApp->mBoard->mFishList->at(i);

            if (aFish->mVirtualTankId < 0)
            {
                if (gWadsworthTimer != 0 && aFish->mSize < SIZE_LARGE)
                    continue;

                if (mXD + 40.0 < aFish->mX + 70 && mXD + 40.0 > aFish->mX + 10 &&
                    mYD + 40.0 < aFish->mY + 70 && mYD + 40.0 > aFish->mY + 10)
                { 
                    aFish->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mOscarList->size(); i++)
        {
            Oscar* aFish = mApp->mBoard->mOscarList->at(i);

            if (aFish->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < aFish->mX + 70 && mXD + 40.0 > aFish->mX + 10 &&
                    mYD + 40.0 < aFish->mY + 70 && mYD + 40.0 > aFish->mY + 10)
                {
                    aFish->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mPentaList->size(); i++)
        {
            Penta* aPenta = mApp->mBoard->mPentaList->at(i);

            if (aPenta->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < aPenta->mX + 70 && mXD + 40.0 > aPenta->mX + 10 &&
                    mYD + 40.0 < aPenta->mY + 70 && mYD + 40.0 > aPenta->mY + 10)
                {
                    aPenta->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mUltraList->size(); i++)
        {
            Ultra* anUltra = mApp->mBoard->mUltraList->at(i);

            if (anUltra->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < anUltra->mX + 150 && mXD + 40.0 > anUltra->mX + 10 &&
                    mYD + 40.0 < anUltra->mY + 150 && mYD + 40.0 > anUltra->mY + 10)
                {
                    anUltra->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mGrubberList->size(); i++)
        {
            Grubber* aGrubber = mApp->mBoard->mGrubberList->at(i);

            if (aGrubber->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < aGrubber->mX + 70 && mXD + 40.0 > aGrubber->mX + 10 &&
                    mYD + 40.0 < aGrubber->mY + 70 && mYD + 40.0 > aGrubber->mY + 10)
                {
                    aGrubber->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mGekkoList->size(); i++)
        {
            Gekko* aGekko = mApp->mBoard->mGekkoList->at(i);

            if (aGekko->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < aGekko->mX + 70 && mXD + 40.0 > aGekko->mX + 10 &&
                    mYD + 40.0 < aGekko->mY + 70 && mYD + 40.0 > aGekko->mY + 10)
                {
                    aGekko->Die(true);
                    break;
                }
            }
        }

        for (int i = 0; i < mApp->mBoard->mBreederList->size(); i++)
        {
            Breeder* aBreeder = mApp->mBoard->mBreederList->at(i);

            if (aBreeder->mVirtualTankId < 0)
            {
                if (mXD + 40.0 < aBreeder->mX + 70 && mXD + 40.0 > aBreeder->mX + 10 &&
                    mYD + 40.0 < aBreeder->mY + 70 && mYD + 40.0 > aBreeder->mY + 10)
                {
                    aBreeder->Die(true);
                    break;
                }
            }
        }
        return false;
    }

    if (mMissleType == MISSLE_BALL)
    { //307
        if (!m0x190)
        {
            bool hit = false;
            double aCX = mXD + 25.0;
            double aCY = mYD + 25.0;
            if (mTarget->mType == TYPE_BILATERUS)
            {
                if(aCX < mTarget->mX + 70 && aCX > mTarget->mX + 10 && 
                    aCY < mTarget->mY + 70 && aCY > mTarget->mY + 10)
                    hit = true;
            }
            else
            {
                if (aCX < mTarget->mX + 140 && aCX > mTarget->mX + 20 &&
                    aCY < mTarget->mY + 140 && aCY > mTarget->mY + 20)
                    hit = true;
            }

            if (hit)
            {
                GameObject* aTar = mTarget;
                m0x190 = true;
                mTarget->mMisslePtr = nullptr;
                mTarget = nullptr;

                int aDamage = 55;
                if (m0x1a0 < 4)
                {
                    mApp->mBoard->PlaySample(SOUND_SFX_ID, 3, 1.0);
                    mSpeedMod = 1.5;
                    aDamage = 15;
                }
                else
                {
                    mApp->mBoard->PlayPunchSound(3);
                    mSpeedMod = 0.6;
                }

                mVX = -mVX;
                mVY = -mVY;
                if (aTar->mType == TYPE_BILATERUS)
                {
                    Bilaterus* aBil = (Bilaterus*)aTar;
                    if (aBil->mActiveHead)
                    {
                        aBil->mActiveHead->m0x1d8 -= aDamage;
                        if (aBil->mActiveHead->m0x1d8 <= 0.0)
                        {
                            aBil->mActiveHead->Die(true);
                            return false;
                        }
                    }
                }
                else if (aTar->mType == TYPE_ALIEN)
                {
                    Alien* anAlien = (Alien*)aTar;
                    if (!anAlien->mIsPsychosquidHealing)
                    {
                        anAlien->mHealth -= aDamage;
                        if (anAlien->mHealth <= 0.0)
                        {
                            anAlien->Remove(true);
                            return false;
                        }
                    }
                }
                return false;
            }
        }
    } // 403
    else if (!IsTargetless()) // 407
    {
        bool hit = false;
        if (mTarget->mType == TYPE_ULTRA)
        {
            if (mXD + 40.0 < mTarget->mX + 140 && mXD + 40.0 > mTarget->mX + 8 &&
                mYD + 40.0 < mTarget->mY + 140 && mYD + 40.0 > mTarget->mY + 4)
                hit = true;
        }
        else
        {
            if (mXD + 40.0 < mTarget->mX + 70 && mXD + 40.0 > mTarget->mX + 10 &&
                mYD + 40.0 < mTarget->mY + 70 && mYD + 40.0 > mTarget->mY + 10)
                hit = true;
        }
        if (hit)
        {
            OnFoodAte(mTarget);
            return true;
        }
    }
    return false;
}

bool Sexy::Missle::Shot(int x, int y)
{
    if (mMissleType == MISSLE_ENERGYBALL)
    {
        if (x > mXD + 10.0 && x < mXD + 70.0 && y > mYD + 10.0 && y < mYD + 70.0)
        {
            if (m0x17c < 1)
            {
                mVX = ((x - mXD) - 40.0) / -5.0;
                mVY = ((y - mYD) - 40.0) / -5.0;
                if (mVX < 1.0 && mVX >= 0.0)
                    mVX = 1.0;
                if (mVX > -1.0 && mVX <= 0.0)
                    mVX = -1.0;
                if (mVY < 1.0 && mVY >= 0.0)
                    mVY = 1.0;
                if (mVY > -1.0 && mVY <= 0.0)
                    mVY = -1.0;
                m0x190 = 1;
            }
            mApp->mBoard->SpawnShot(x - 40, y - 40, 2);
            return true;
        }
    }
    else if (mMissleType == MISSLE_CLASSIC)
    {
        double aCX = mXD + 40.0;
        double aCY = mYD + 40.0;
        if (x > aCX - 30.0 && x < aCX + 30.0 && y > aCY - 30.0 && y < aCY + 30.0)
        {
            if (m0x17c < 1)
            {
                Remove();
                mApp->mBoard->SpawnShot(x - 40, y - 40, 2);
                return true;
            }
        }
    }
    return false;
}
