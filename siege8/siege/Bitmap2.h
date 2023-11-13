#pragma once
#include "obj.h"

namespace siege{

	class CBitmap :
		public CObj
	{
	public:
		u16 w,h;
		u32 *data;

		CBitmap(void)
		{
		}
	public:

		virtual ~CBitmap(void)
		{
		}
	};

}
