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
#ifndef __OCP1HEADER_HXX__
#define __OCP1HEADER_HXX__

// C++ Standard Headers


// C Standard Headers
#include <stdint.h>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#define OCP1_HEADER_SIZE 9

namespace oca
{
	namespace net
	{
		enum OcaMessageType
		{
			OcaCmd = 0,
			OcaCmdRrq = 1,
			OcaNtf = 2,
			OcaRsp = 3
		};

		struct Ocp1Header
		{

			Ocp1Header();
			Ocp1Header(uint16_t protocolVersion, uint32_t messageSize, OcaMessageType messageType, uint16_t messageCount);
			Ocp1Header(const Ocp1Header& rhs);

			Ocp1Header& operator=(const Ocp1Header& rhs);

			static const Ocp1Header FromBuffer(boost::asio::const_buffer& buffer);	// TODO: should the parameter be const? I don't think so, but I don't know for sure.
			static void FromBuffer(boost::asio::const_buffer& buffer, Ocp1Header& header);
			void WriteToBuffer(boost::asio::mutable_buffer& buffer) const;

			~Ocp1Header();

			uint16_t protocolVersion;
			uint32_t messageSize;
			OcaMessageType messageType;
			uint16_t messageCount;
		};
	}
}


#endif // __OCP1HEADER_HXX__
