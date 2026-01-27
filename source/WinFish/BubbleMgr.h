#pragma once

#include "SexyAppFramework/Common.h"
#include <SexyAppFramework/Graphics.h>

namespace Sexy
{
    class Bubble
    {
    public:
        int mType;
        int mShakeOffset;
        float mX, mY;
        float mVY;

    public:
        Bubble();

        void            Draw(Graphics* g);
        void            Update();
    };

    class BubbleFish
    {
    public:
        int mSize;
        int mUpdateCnt;
        int mTurnTimer;
        float mX;
        float mY;
        float mVX;
        float mVY;
        Color mColor1;
        Color mColor2;
        int m0x3c;
        int m0x40;

    public:
        BubbleFish();

        void            Draw(Graphics* g);
        void            Update();
    };

    typedef std::list<BubbleFish> BubbleFishList;

	class BubbleMgr
	{
	public:
        Rect mBubbleBounds;
        Rect mBubbleFishBounds;
        int mMaxBubbles;
        int mBubbleSpawnChance;
        int m0x2c;
        int mMaxFish;
        int mFishSpawnChance;
        float mDefaultBubbleFishVY;
        bool mClipBubbleFish;
        std::list<Bubble*> mBubbleList;
        BubbleFishList mFishList;

    public:
        BubbleMgr();
        ~BubbleMgr();

        void                            Update();
        void                            Draw(Graphics* g);
        void                            SpawnBubble(int theX, int theY);
        void                            SpawnRandomBubble();
        void                            SpawnRandomBubbleFish();

        void					        SetBubbleConfig(int theMaxBubble, int theBubbleSpawnChance);
        void					        SetBubbleFishConfig(int theMaxBubbleFish, int theBubbleFishSpawnChance);
        void					        SetDefaultBubbleFishVY(float theDefaultVY);
        void					        UpdateALot();
	};
}

