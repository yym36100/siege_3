#pragma once
#include "Obj.h"
#include "..\Events.h"

namespace siege
{
	namespace widgets
	{
		class CWidget :
			public CObj
		{					
		public:			
			u16 command;
			stRect rect;
			bool Focused, Highlighted;



			CWidget( u16 x, u16 y, u16 w, u16 h):rect(x,y,w,h),Highlighted(false) {}
			virtual ~CWidget(void){}
			virtual void Draw(void){}

			void DrawAll(void)
			{
				CWidget *w = this;
				do{
					w->Draw();
					w=(siege::widgets::CWidget*)w->next;
				}while(w);
			}

			volatile bool HitTest(Events *e) const
			{
				return (e->x>=rect.x)&&(e->y>=rect.y)&&(e->x<rect.x+rect.w)&&(e->y<rect.y+rect.h);
			}

			virtual u16 OnEvent(Events *e)
			{
				return 0;
			}


		};

	};
};