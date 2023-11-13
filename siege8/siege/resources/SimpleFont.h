#pragma once
#include "..\cdef.h"
#include "..\Surface.h"

namespace siege
{
	namespace resources
	{
		class CSimpleFont
		{		
		public:
			CSurface *s;
			u32 Color;
			u8 size;

			static u8 const TABLE5[];
			CSimpleFont(CSurface *ss,u32 cc):s(ss),Color(cc),size(1){}
			~CSimpleFont(void){}

			void DrawChar( u16 x, u16 y, u8 c) const;
			void DrawText(u16 x, u16 y, u8 const *t) const;
			void DrawTextB(u16 x, u16 y, u8 const *t) const;

		};
	}
}