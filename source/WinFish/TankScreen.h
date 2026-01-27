#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/DialogButton.h"
#include "WinFishApp.h"

namespace Sexy
{
	class TankScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		DialogButton* mMenuButton;
		DialogButton* mStoriesButton;
		DialogButton* mTankButtons[4];
		int m0xa8[8];

	public:
		TankScreen(WinFishApp* theApp);
		virtual ~TankScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		void					DrawBolts(Graphics* g, int xOffset, int yOffset, bool drawRight, bool drawBottom);
	};
}