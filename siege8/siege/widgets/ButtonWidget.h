#pragma once
#include "Widget.h"
#include "..\Surface.h"

namespace siege
{
	namespace widgets
	{
		class CButtonWidget :
			public CWidget
		{
			CSurface *surf;
			resources::CSimpleFont *sf;
			u8 text[256];
		public:
			u32 handle;


			CButtonWidget(CSurface *ss, u16 x, u16 y, u16 w, u16 h):
			surf(ss),CWidget(x,y,w,h)
			{
				sf = new resources::CSimpleFont(ss,0xff000000);
			}
			void SetText(u8 *t)
			{
				strcpy((char*)text,(char*)t);
			}
			void Draw()
			{
				if(Highlighted)
				{
					sf->Color = 0xff000000;

					surf->FillRect(rect,0xfff0f0f0);
				}
				else
				{
					sf->Color = 0xff000000;
					surf->FillRect(rect,0xffd0d0d0);
				}
				surf->DrawRect(rect,0xff606060);
				sf->DrawText(rect.x+(rect.w - 6*strlen((char*)text))/2,rect.y+(rect.h-6)/2,text);
			}

			virtual ~CButtonWidget(void)
			{
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
						if(handle)  SendMessage((HWND)handle,0x1122,command,0);
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