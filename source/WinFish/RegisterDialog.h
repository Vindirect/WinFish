#pragma once

#include "MoneyDialog.h"
#include "SexyAppFramework/EditListener.h"

namespace Sexy
{
	class WinFishApp;
	class DialogButton;
	class EditWidget;
	class RegisterDialog : public MoneyDialog, public ButtonListener, public EditListener
	{
	public:
		EditWidget* mNameEditWidget;
		EditWidget* mCodeEditWidget;
		DialogButton* mGetCodeButton;

	public:
		RegisterDialog(WinFishApp* theApp);
		virtual ~RegisterDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int				GetPreferredHeight(int theWidth);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		virtual void			EditWidgetText(int theId, const SexyString& theString);
	};
}

