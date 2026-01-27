#pragma once
#include "SexyAppFramework/ButtonWidget.h"
namespace Sexy
{
	class GameObject;
	class WinFishApp;

	class FishButtonWidget : public ButtonWidget
	{
	public:
		Rect m0x120;
		Rect m0x130;
		Rect m0x140;
		bool m0x150;
		WinFishApp* mApp;
		GameObject* mObject;

	public:
		FishButtonWidget(GameObject* theObject, int theId, ButtonListener* theButtonListener);
		virtual ~FishButtonWidget();

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		void					SwitchDrawRects(bool toggle);
	};
}

