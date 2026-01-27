#pragma once

#include "SexyAppFramework/Dialog.h"

namespace Sexy
{
	class Image;
	class UpdateCheckDialog : public Dialog, public ButtonListener
	{
	public:
		bool mCheckFinished;
		Image* mWaitBarImage;
		int mWaitBarXOffset;
		int mYOffset;

	public:
		UpdateCheckDialog(Image* theComponentImage, Image* theButtonComponentImage, Image* theWaitImage, int theId);
		virtual ~UpdateCheckDialog();

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual int				GetPreferredHeight(int theWidth);
	};
}

