#pragma once
#include "Widget.h"
#include ".\resources\simplefont.h"
#include "..\Surface.h"
#include "..\gBitmap.h"
#include ".\resources\gimphead.h"

namespace siege
{
	namespace widgets
	{
		class CBitmapWidget :
			public CWidget
		{

			resources::CSimpleFont *sf;
			CGmpBitmap *bmp;

			u8 text[256];
		public:

			CBitmapWidget(CSurface *ss, u16 x, u16 y, u16 w, u16 h):
			  CWidget(x,y,w,h)
			  {
				  bmp = NULL;
				  
			  }
			  void SetPicture(gimphead *h)
			  {
				  bmp = new CGmpBitmap(h);

			  }

			  virtual void Draw(void) const
			  {
				  
			  }


			  virtual ~CBitmapWidget(void)
			  {
			  }


			  virtual u16 OnEvent(Events *e)
			  {
				  return 0;

			  }


		};

	};
};