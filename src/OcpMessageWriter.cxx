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


// 3rd Party Headers


// GTK Headers


#include <oca/OcaTypes.hxx>
#include "Ocp1Header.hxx"
#include "Ocp1Parameters.hxx"

#include "OcpMessageWriter.hxx"



namespace oca
{
		void OcpMessageWriter::WriteHeaderToBuffer(const net::Ocp1Header& header, boost::asio::mutable_buffer& buffer)
		{
			// We cast the buffer to a pointer to the type we want, so that we
			// can write our data there.
			uint16_t* pv = boost::asio::buffer_cast<uint16_t*>(buffer);
			*pv = htons(header.protocolVersion);

			// Again, we get a new buffer (over the same raw data) that is
			// offset by the correct amount to give us the next field
			boost::asio::mutable_buffer bms = buffer+sizeof(uint16_t);
			uint32_t* ms = boost::asio::buffer_cast<uint32_t*>(bms);
			*ms = htonl(header.messageSize);

			boost::asio::mutable_buffer bmt = bms+sizeof(uint32_t);
			uint8_t* mt = boost::asio::buffer_cast<uint8_t*>(bmt);
			*mt = (uint8_t)header.messageType;

			boost::asio::mutable_buffer bmc = bmt+sizeof(uint8_t);
			uint16_t* mc = boost::asio::buffer_cast<uint16_t*>(bmc);
			*mc = htons(header.messageCount);
		}

		void OcpMessageWriter::WriteParametersToBuffer(const net::Ocp1Parameters& parameters, boost::asio::mutable_buffer& buffer)
		{
			OcaUint8* pc = boost::asio::buffer_cast<OcaUint8*>(buffer);
			*pc = parameters.parameterCount;

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			boost::asio::mutable_buffer paramBuffer = buffer + sizeof(OcaUint8);
			OcaUint8* destination = boost::asio::buffer_cast<OcaUint8*>(paramBuffer);
			memcpy(destination, &parameters.parameters[0], parameters.parameters.size());

		}
}
