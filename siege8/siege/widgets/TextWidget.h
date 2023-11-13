#pragma once
#include "Widget.h"
#include ".\resources\simplefont.h"
#include "..\Surface.h"

namespace siege
{
	namespace widgets
	{
		class CTextWidget :
			public CWidget
		{

			resources::CSimpleFont *sf;
			u8 text[256];
			
		public:
			u8 size;

			CTextWidget(CSurface *ss, u16 x, u16 y, u16 w, u16 h):
			  CWidget(x,y,w,h), size(1)
			  {
				  sf = new resources::CSimpleFont(ss,0xffffffff);
			  }
			  void SetText(u8 *t)
			  {
				  strcpy((char*)text,(char*)t);
			  }
			  virtual void Draw(void) 
			  {
				  if(Highlighted)
				  {
					  sf->Color = 0xffff0000;					  
				  }
				  else
				  {
					  sf->Color = 0xff808080;
				  }
				  sf->DrawTextB(rect.x,rect.y,text);
			  }

			  virtual ~CTextWidget(void)
			  {
			  }
			  void setsize(u16 s)
			  {
				  size = s;
				  sf->size = size;
			  }


			  virtual u16 OnEvent(Events *e)
			  {
				  if(HitTest(e))
				  {
					  Highlighted = true;
					  switch(e->e)
					  {
					  case Events::mmove:
						  break;
					  case Events::mpress:
						  return 1;
						  break;
					  }	
				  }
				  else
				  {
					  Highlighted = false;
				  }
				  return 0;
			  }


		};

	};
};