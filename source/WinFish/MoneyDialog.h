#pragma once

#include "SexyAppFramework/Dialog.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/EditWidget.h"

namespace Sexy
{
	class WinFishApp;
	class MoneyDialog : public Dialog, public ButtonListener
	{
	public:
		WinFishApp* mApp;
		int mEnableButtonsTimer;

	public:
		MoneyDialog(WinFishApp* theApp, Image* theComponentImage, Image* theButtonComponentImage,
			int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);
		virtual ~MoneyDialog();

		virtual void			Update();

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		virtual void			DisableButtons(int theEnableButtonsTimer);		//[76]
		virtual void			WidgetClicked(int theId, bool theFlag);			//[77]

		void					DrawEditWidgetBox(Graphics* g, EditWidget* theWidget);
	};
}

