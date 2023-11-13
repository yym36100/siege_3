#pragma once
#include "Bitmap2.h"
#include ".\resources\gimphead.h"

namespace siege{

	class CGmpBitmap :
		public CBitmap
	{
	public:
		gimphead *h;

		CGmpBitmap(void)
		{}
		CGmpBitmap(gimphead *hh):h(hh)		{		}


		


		virtual ~CGmpBitmap(void)
		{
		}
	};

}
