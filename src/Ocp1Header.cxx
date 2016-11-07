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
#include <arpa/inet.h>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#include "Ocp1Header.hxx"


namespace oca
{
	namespace ocp
	{
		Ocp1Header::Ocp1Header()
			:	protocolVersion(0), messageSize(0), messageType(OcaCmd), messageCount(0)
		{

		}

		Ocp1Header::Ocp1Header(uint16_t protocolVersion, uint32_t messageSize, OcaMessageType messageType, uint16_t messageCount)
			:	protocolVersion(protocolVersion), messageSize(messageSize), messageType(messageType), messageCount(messageCount)
		{

		}

		Ocp1Header::Ocp1Header(const Ocp1Header& rhs)
			:	protocolVersion(rhs.protocolVersion), messageSize(rhs.messageSize), messageType(rhs.messageType), messageCount(rhs.messageCount)
		{

		}

		Ocp1Header& Ocp1Header::operator=(const Ocp1Header& rhs)
		{
			if (this != &rhs)
			{
				protocolVersion = rhs.protocolVersion;
				messageSize = rhs.messageSize;
				messageType = rhs.messageType;
				messageCount = rhs.messageCount;
			}

			return *this;
		}

		Ocp1Header::~Ocp1Header() {}
	}
}
