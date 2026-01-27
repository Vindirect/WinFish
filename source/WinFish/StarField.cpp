#include "StarField.h"
#include "WinFishApp.h"
#include "Board.h"
#include "SexyAppFramework/DDImage.h"

using namespace Sexy;

Sexy::StarField::StarField()
{
	mStarList.clear();
	mMaxStars = 0;
	mNebulaImage = 0;
}

Sexy::StarField::~StarField()
{
	if (mNebulaImage)
		delete mNebulaImage;
}

void Sexy::StarField::Init(int theMaxStars)
{
	mMaxStars = theMaxStars;
	if (mNebulaImage == nullptr)
		mNebulaImage = gSexyApp->GetImage("images/nebula1");

	mStarList.clear();
	if (theMaxStars > 0)
	{
		for (int i = 0; i < theMaxStars; ++i)
		{
			int x = Rand() % 640;
			int y = Rand() % 480;

			AddStar(x, y);
		}
	}
}

void Sexy::StarField::AddStar(int theX, int theY)
{
	mStarList.emplace_back();

	Star& newStar = mStarList.back();

	newStar.mX = theX;
	newStar.mY = theY;
	newStar.mVY = 0;

	int aVal = rand();
	int aVal2 = aVal / 3;
	aVal %= 3;
	if (aVal == 0)
	{
		newStar.mColorValue = 0x404040;
		newStar.mVX = -0.6f;
	}
	else if (aVal == 1)
	{
		newStar.mColorValue = 0x909090;
		newStar.mVX = -1.8f;
	}
	else if (aVal == 2)
	{
		newStar.mColorValue = 0xFFFFFF;
		newStar.mVX = -2.7f;
	}
}

void Sexy::StarField::Update()
{
	StarList::iterator it = mStarList.begin();
	while (it != mStarList.end())
	{
		Star& currentStar = *it;
		currentStar.mX += currentStar.mVX;
		currentStar.mY += currentStar.mVY;

		if (currentStar.mX < 0.0f)
			it = mStarList.erase(it);
		else
			++it;
	}

	for (int i = mStarList.size(); i < mMaxStars; i++)
	{
		AddStar(640, Rand() % 480);
	}
}

void Sexy::StarField::Draw(Graphics* g, bool flag)
{
	Board* aBoard = ((WinFishApp*)gSexyApp)->mBoard;
	if (!mNebulaImage)
	{
		g->SetColor(Color::Black);
		g->FillRect(0, 0, 640, 480);
	}
	else
	{
		int aImgWdth = mNebulaImage->mWidth;
		int aX = (aImgWdth - aBoard->mGameUpdateCnt / 2 % aImgWdth)-1;
		g->DrawImage(mNebulaImage, aX - aImgWdth, 0);
		g->DrawImage(mNebulaImage, aX, 0);
		g->DrawImage(mNebulaImage, aX + aImgWdth, 0);
		if (flag)
		{
			aBoard->Unk06(g, mNebulaImage, aX - aImgWdth, 0, 8.0);
			aBoard->Unk06(g, mNebulaImage, aX, 0, 8.0);
			aBoard->Unk06(g, mNebulaImage, aX + aImgWdth, 0, 8.0);
		}
	}
	StarList::iterator it;
	for (it = mStarList.begin(); it != mStarList.end(); ++it)
	{
		g->SetColor(Color(it->mColorValue));
		g->FillRect(it->mX, it->mY, 1, 1);
	}
}
