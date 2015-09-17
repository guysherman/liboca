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
#ifndef __OCP1RESPONSE_HXX__
#define __OCP1RESPONSE_HXX__

// C++ Standard Headers
#include <vector>

// C Standard Headers


// Boost Headers


// 3rd Party Headers


// GTK Headers


#include <oca/OcaTypes.hxx>
#include "Ocp1Parameters.hxx"

namespace oca
{
	namespace net
	{
		class Ocp1Response
		{
		public:
			Ocp1Response();
			Ocp1Response(const Ocp1Response&);
			Ocp1Response& operator=(const Ocp1Response&);
			~Ocp1Response();

			OcaUint32			responseSize;
			OcaUint32			handle;
			OcaStatus			statusCode;
			Ocp1Parameters		parameters;

		};

		typedef std::vector<Ocp1Response> ResponseList;
	}
}


#endif // __OCP1RESPONSE_HXX__
