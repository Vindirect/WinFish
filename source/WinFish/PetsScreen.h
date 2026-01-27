#pragma once
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/DialogButton.h"
#include "WinFishApp.h"
#include "PetButtonWidget.h"

namespace Sexy
{
	class MemoryImage;
	class PetScreenOverlay : public Widget
	{
	public:
		PetsScreen* mScreen;

	public:
		virtual void			Draw(Graphics* g);

	};

	class PetsScreen : public Widget, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		DialogButton* mReturnButton;
		DialogButton* mMenuButton;
		PetScreenOverlay* mOverlay;
		PetButtonWidget* mPetButtons[GameObject::PET_END];
		MemoryImage* mBackgroundImage;
		bool m0x100[GameObject::PET_END];
		int m0x118;
		int m0x11c;
		int m0x120;
		int m0x124;
		int m0x128;
		int m0x12c;
		int m0x130;
		int m0x134;

	public:
		PetsScreen(WinFishApp* theApp);
		virtual ~PetsScreen();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Update();

		virtual void			DrawOverlay(Graphics* g);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
		virtual void			ButtonMouseEnter(int theId);
		virtual void			ButtonMouseLeave(int theId);

		void					DrawPetInfo(Graphics* g, int thePetId, int theAlpha);
	};
}

