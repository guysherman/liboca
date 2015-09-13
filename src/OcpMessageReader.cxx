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

#include "OcpMessageReader.hxx"

namespace oca
{
	OcpMessageReader::OcpMessageReader()
	{

	}

	OcpMessageReader::~OcpMessageReader()
	{

	}


	void OcpMessageReader::SyncValueReceived(uint8_t* bufferData,
		const boost::system::error_code& error,
		size_t bytesTransferred,
		boost::function<void(void)> getHeader )
	{
		if (error.value() != boost::system::errc::success)
		{
			// TODO: consider an exception here
			return;
		}


		if (bytesTransferred != 1)
		{
			// TODO: consider an exception here
			return;
		}


		// TODO: logging


		if (bufferData != 0)
		{
			if (bufferData[0] == 0x3B)
			{
				getHeader();
			}
		}
	}

	void OcpMessageReader::Ocp1HeaderReceived(uint8_t* bufferData,
		uint64_t connectionIdentifier,
		const boost::system::error_code& error,
		size_t bytesTransferred,
		boost::function<void(uint32_t)> getData)
	{
		boost::asio::const_buffer headerBuffer(bufferData, bytesTransferred);



		// By convetion we should be removing the per connection header object once we're done
		// so if this assert throws we've either forgotten, or we've got a concurrency
		// issue.
		ConnectionStateMap::iterator it = perConnectionState.find(connectionIdentifier);
		assert(it == perConnectionState.end());




		oca::net::Ocp1Header header = oca::net::Ocp1Header::FromBuffer(headerBuffer);

		perConnectionState.insert(ConnectionStateMap::value_type(connectionIdentifier, header));

		// The messageSize property includes the header, but we've
		// already got it so we ask for fewer bytes.
		getData(header.messageSize - OCP1_HEADER_SIZE);

	}

	void OcpMessageReader::Ocp1DataReceived(uint8_t* bufferData,
		uint64_t connectionIdentifier,
		const boost::system::error_code& error,
		size_t bytesTransferred)
	{
		// Don't really have anything to do here yet...



		// ...but before we go, lets clean up
		ConnectionStateMap::iterator it = perConnectionState.find(connectionIdentifier);
		if (it != perConnectionState.end())
		{
			perConnectionState.erase(it);
		}
		return;
	}

}
