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

// C++ Standard Headers

// C Standard Headers


// Boost Headers


// 3rd Party Headers


// GTK Headers

#include <oca/OcaTypes.hxx>
#include "Ocp1Parameters.hxx"
#include "Ocp1Response.hxx"


namespace oca
{
	namespace net
	{
		Ocp1Response::Ocp1Response()
			: responseSize(0), handle(0), statusCode(OK)
		{

		}


		Ocp1Response::Ocp1Response(const Ocp1Response& rhs)
			: responseSize(rhs.responseSize), handle(rhs.handle), statusCode(rhs.statusCode), parameters(rhs.parameters)
		{

		}

		Ocp1Response& Ocp1Response::operator=(const Ocp1Response& rhs)
		{
			responseSize = rhs.responseSize;
			handle = rhs.handle;
			statusCode = rhs.statusCode;
			parameters = rhs.parameters;

			return *this;
		}

		Ocp1Response::~Ocp1Response()
		{

		}
	}
}
