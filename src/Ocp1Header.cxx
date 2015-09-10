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
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#include "Ocp1Header.hxx"


namespace oca
{
	namespace net
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

		const Ocp1Header Ocp1Header::FromBuffer(boost::asio::const_buffer& buffer)
		{
			Ocp1Header header;

			FromBuffer(buffer, header);

			return header;
		}

		void Ocp1Header::FromBuffer(boost::asio::const_buffer& buffer, Ocp1Header& header)
		{
			// Cast the buffer to the type we want, and dereference the pointer
			// so that we can read the data.
			header.protocolVersion = *(boost::asio::buffer_cast<const uint16_t*>(buffer));

			// Create a new buffer offset from the previous one so that we can
			// do the same as above but for the next field
			boost::asio::const_buffer ms = buffer+sizeof(uint16_t);
			header.messageSize = *(boost::asio::buffer_cast<const uint32_t*>(ms));

			boost::asio::const_buffer mt = ms+sizeof(uint32_t);
			header.messageType = (OcaMessageType) *(boost::asio::buffer_cast<const uint8_t*>(mt));

			boost::asio::const_buffer mc = mt+sizeof(uint8_t);
			header.messageCount = *(boost::asio::buffer_cast<const uint16_t*>(mc));
		}

		void Ocp1Header::WriteToBuffer(boost::asio::mutable_buffer& buffer) const
		{
			// We cast the buffer to a pointer to the type we want, so that we
			// can write our data there.
			uint16_t* pv = boost::asio::buffer_cast<uint16_t*>(buffer);
			*pv = protocolVersion;

			// Again, we get a new buffer (over the same raw data) that is
			// offset by the correct amount to give us the next field
			boost::asio::mutable_buffer bms = buffer+sizeof(uint16_t);
			uint32_t* ms = boost::asio::buffer_cast<uint32_t*>(bms);
			*ms = messageSize;

			boost::asio::mutable_buffer bmt = bms+sizeof(uint32_t);
			uint8_t* mt = boost::asio::buffer_cast<uint8_t*>(bmt);
			*mt = (uint8_t)messageType;

			boost::asio::mutable_buffer bmc = bmt+sizeof(uint8_t);
			uint16_t* mc = boost::asio::buffer_cast<uint16_t*>(bmc);
			*mc = messageCount;


		}

		Ocp1Header::~Ocp1Header() {}
	}
}
