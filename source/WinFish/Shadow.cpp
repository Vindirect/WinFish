#include "Shadow.h"
#include "WinFishApp.h"
#include "SexyAppFramework/WidgetManager.h"
#include "Board.h"
#include "Res.h"

Sexy::Shadow::Shadow()
{
	mObjectPtr = nullptr;
	mClip = false;
	mType = TYPE_SHADOW;
}

Sexy::Shadow::Shadow(int theSize, GameObject* theObject)
{
	mObjectPtr = theObject;
	mType = TYPE_SHADOW;
	if (theObject == nullptr)
		m0x160 = 0;
	else
	{
		theObject->mShadowPtr = this;
		m0x160 = (theObject->mY - 50) / 2;
	}
	m0x168 = 1.0;
	mX = -200;
	mY = mApp->mSeed->Next() % 5 + 410;
	mWidth = 80;
	mHeight = 40;
	mMouseVisible = false;
	mClip = false;
	mShadowSize = theSize;
}

void Sexy::Shadow::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	GameObject::UpdateCounters();
	if (mObjectPtr == nullptr)
		return;

	m0x160 = (mObjectPtr->mY - 50) / 2;
	int aX = mObjectPtr->mX;
	if (mShadowSize == 2)
		aX += 40;
	mX = aX;
	if (mShadowSize == 0 || mShadowSize == 2)
	{
		int aY = 370 - mObjectPtr->mY;
		if (aY < 0)
			aY = 0;
		mY = 415 - (aY*30) / 370;
	}
}

void Sexy::Shadow::Draw(Graphics* g)
{
	GameObject::UpdateFishSongMgr();
	g->SetColorizeImages(true);
	if (mObjectPtr->mInvisible && mObjectPtr->mType >= TYPE_PENTA && mObjectPtr->mType <= TYPE_GRUBBER)
	{
		g->SetColorizeImages(false);
		return;
	}

	int anAlpha = m0x160;
	if (mShadowSize == 2)
		anAlpha = anAlpha * 2 * m0x168;
	if (anAlpha < 0) anAlpha = 0;
	if (anAlpha > 255) anAlpha = 255;
	g->SetColor(Color(gUnkColor01.mRed, gUnkColor01.mGreen, gUnkColor01.mBlue, anAlpha));
	if (mShadowSize == 1)
	{
		g->DrawImage(IMAGE_SHADOW, 0, 0);
		g->SetColorizeImages(false);
		return;
	}

	int anImgWidth = IMAGE_SHADOW->mWidth;
	int anImgHeight = IMAGE_SHADOW->mHeight;
	Rect aDestRect(0, 0, anImgWidth, anImgHeight);
	Rect aSrcRect(0, 0, anImgWidth, anImgHeight);

	if (mObjectPtr != nullptr && mApp->Is3DAccelerated())
	{
		double aVal = (mY - mObjectPtr->mY) / 100.0;
		if (aVal < 0.9f)
			aVal = 0.9f;
		else if (aVal > 1.3f)
			aVal = 1.3f;

		if (mShadowSize == 2)
			aVal += 0.4;
		float aTransform = aVal - 1.0;

		int aDeltaWidth = (int)(IMAGE_SHADOW->mWidth * aTransform);
		int aDeltaHeight = (int)(IMAGE_SHADOW->mHeight * aTransform);

		aDestRect.mX -= aDeltaWidth;
		aDestRect.mY -= aDeltaHeight;
		aDestRect.mWidth += aDeltaWidth * 2;
		aDestRect.mHeight += aDeltaHeight * 2;
	}
	g->SetFastStretch(false);
	g->DrawImageMirror(IMAGE_SHADOW, aDestRect, aSrcRect, false);
	g->SetColorizeImages(false);
}

void Sexy::Shadow::VFT74()
{
	if (mObjectPtr != nullptr)
	{
		mObjectPtr->mShadowPtr = nullptr;
		mObjectPtr = nullptr;
	}
}

void Sexy::Shadow::Remove()
{
	if (mObjectPtr != nullptr)
	{
		mObjectPtr->mShadowPtr = nullptr;
		mObjectPtr = nullptr;
	}
	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
}

void Sexy::Shadow::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncLong(mShadowSize);
	theSync->SyncLong(m0x160);
	theSync->SyncDouble(m0x168);
	theSync->SyncPointer((void**) &mObjectPtr);
}
