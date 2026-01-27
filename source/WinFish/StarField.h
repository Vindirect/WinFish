#pragma once

#include "SexyAppFramework/Common.h"

namespace Sexy
{
	class DDImage;
	class Graphics;

	struct Star
	{
		float mX;
		float mY;
		float mVX;
		float mVY;
		ulong mColorValue;
	};

	typedef std::list<Star> StarList;

	class StarField
	{
	public:
		StarList mStarList;
		int mMaxStars;
		DDImage* mNebulaImage;

	public:
		StarField();
		virtual ~StarField();

		void				Init(int theMaxStars);
		void				AddStar(int theX, int theY);
		void				Update();
		void				Draw(Graphics* g, bool flag);
	};
}

