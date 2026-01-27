#pragma once

#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "WinFishApp.h"

namespace Sexy
{
	class PetButtonWidget : public ButtonWidget
	{
	public:
		Rect mRect;
		bool m0x130;
		WinFishApp* mApp;
		int m0x138;

	public:
		PetButtonWidget(int theImageId, int theId, ButtonListener* theListener);
		virtual ~PetButtonWidget();

		virtual void			Draw(Graphics* g);
		virtual void			MouseDown(int x, int y, int theClickCount);
		virtual void			MouseUp(int x, int y, int theClickCount);

		void					Setup(bool flag);
	};
}

