/*
  Copyright (C) 2015 Guy Sherman, Shermann Innovations Limited

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#ifndef __OCP1NTFPARAMS_HXX__
#define __OCP1NTFPARAMS_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers


// 3rd Party Headers


// GTK Headers


// Our Headers
#include <oca/OcaTypes.hxx>
#include "Ocp1EventData.hxx"

namespace oca
{
	namespace net
	{
		struct Ocp1NtfParams
		{
			Ocp1NtfParams();
			Ocp1NtfParams(const Ocp1NtfParams& rhs);
			Ocp1NtfParams& operator=(const Ocp1NtfParams& rhs);
			~Ocp1NtfParams();

			OcaUint8		parameterCount;
			OcaBlob			context;
			Ocp1EventData	eventData;
		};
	}
}


#endif // __OCP1NTFPARAMS_HXX__
