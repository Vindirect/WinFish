#pragma once
#include "WinFishApp.h"
#include "MoneyDialog.h"
namespace Sexy
{
	struct FishFeedButton
	{
		Rect mButtonRect;
		int mId;
		bool mShown;
	};

	class FoodDialog : public MoneyDialog
	{
	public:
		int m0x158;
		int m0x15c;
		FishFeedButton mButtons[8];

	public:
		FoodDialog(WinFishApp* theApp);
		virtual ~FoodDialog();

		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
		virtual void			MouseLeave();
		virtual void			MouseMove(int x, int y);
		virtual void			MouseDown(int x, int y, int theClickCount);
		virtual void			MouseUp(int x, int y, int theClickCount);


		int						MouseAtHandle(int x, int y);
	};
}

