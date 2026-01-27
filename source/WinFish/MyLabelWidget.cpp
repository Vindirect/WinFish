#include "MyLabelWidget.h"
#include "Res.h"
#include "SexyAppFramework/Font.h"

using namespace Sexy;

Sexy::MyLabelWidget::MyLabelWidget()
{
	mLabel = "";
	mLabelColor = Color(0x6e, 0xfa, 0x6e, 0xff);
	mAlignment = 0;
	mLabelFont = FONT_CONTINUUMBOLD12OUTLINE;
}

Sexy::MyLabelWidget::~MyLabelWidget()
{
}

void Sexy::MyLabelWidget::Draw(Graphics* g)
{
	g->SetColor(mLabelColor);
	g->SetFont(mLabelFont);

	int aX, aWidth;
	if (mAlignment == 0)
		aX = 0;
	else if (mAlignment == 1)
	{
		aWidth = mLabelFont->StringWidth(mLabel);
		aX = (mWidth - aWidth) / 2;
	}
	else
	{
		aWidth = mLabelFont->StringWidth(mLabel);
		aX = mWidth - aWidth;
	}

	g->DrawString(mLabel, aX, mLabelFont->GetAscent() + (mHeight - mLabelFont->GetHeight())/2);
}

void Sexy::MyLabelWidget::SetLabel(SexyString theLabel)
{
	mLabel = theLabel;
	MarkDirty();
}
