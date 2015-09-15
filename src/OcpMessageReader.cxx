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
#include <map>
#include <vector>

// C Standard Headers
#include <arpa/inet.h>

// Boost Headers

// 3rd Party Headers


// GTK Headers

#include <oca/OcaTypes.hxx>
#include "OcpMessageReader.hxx"
#include "Ocp1Header.hxx"

namespace oca
{
	OcpMessageReader::OcpMessageReader()
	{

	}

	OcpMessageReader::~OcpMessageReader()
	{

	}


	const net::Ocp1Header OcpMessageReader::HeaderFromBuffer(boost::asio::const_buffer& buffer)
	{
		net::Ocp1Header header;

		HeaderFromBuffer(buffer, header);

		return header;
	}

	void OcpMessageReader::HeaderFromBuffer(boost::asio::const_buffer& buffer, net::Ocp1Header& header)
	{
		// Cast the buffer to the type we want, and dereference the pointer
		// so that we can read the data.
		header.protocolVersion = ntohs(*(boost::asio::buffer_cast<const uint16_t*>(buffer)));

		// Create a new buffer offset from the previous one so that we can
		// do the same as above but for the next field
		boost::asio::const_buffer ms = buffer+sizeof(uint16_t);
		header.messageSize = ntohl(*(boost::asio::buffer_cast<const uint32_t*>(ms)));

		boost::asio::const_buffer mt = ms+sizeof(uint32_t);
		header.messageType = (net::OcaMessageType) *(boost::asio::buffer_cast<const uint8_t*>(mt));

		boost::asio::const_buffer mc = mt+sizeof(uint8_t);
		header.messageCount = ntohs(*(boost::asio::buffer_cast<const uint16_t*>(mc)));
	}

	void OcpMessageReader::ParametersFromBuffer(boost::asio::const_buffer& buffer, size_t remainingCommandBytes, net::Ocp1Parameters& parameters)
	{
		parameters.parameterCount = *(boost::asio::buffer_cast<const uint8_t*>(buffer));

		size_t parameterBufferBytes = (remainingCommandBytes - sizeof(uint8_t));
		boost::asio::const_buffer paramBuffer = buffer+sizeof(uint8_t);
		const uint8_t* params = boost::asio::buffer_cast<const uint8_t*>(paramBuffer);

		assert (parameters.parameters.size() == 0);
		// We ask the vector to grow in one hit, so we can memcpy straight after into it
		// We reserve then resize, rather than just resizing, so that we only get one
		// allocation, rather than log(n) allocations;
		// We resize so that the size()  value is up to date, and so that the
		// memory gets initialized to 0.
		parameters.parameters.reserve(parameterBufferBytes);
		parameters.parameters.resize(parameterBufferBytes, 0);
		// TODO: security issue: we trust that the buffer actually has the right number of bytes #security
		memcpy(&parameters.parameters[0], params, parameterBufferBytes);

	}

	void OcpMessageReader::MethodIdFromBuffer(boost::asio::const_buffer& buffer, OcaMethodId& methodId)
	{
		methodId.treeLevel = ntohs(*(boost::asio::buffer_cast<const OcaUint16*>(buffer)));

		boost::asio::const_buffer mi = buffer + sizeof(OcaUint16);
		methodId.methodIndex = ntohs(*(boost::asio::buffer_cast<const OcaUint16*>(mi)));
	}

	void OcpMessageReader::CommandFromBuffer(boost::asio::const_buffer& buffer, net::Ocp1Command& cmd)
	{
		cmd.commandSize = ntohl(*(boost::asio::buffer_cast<const OcaUint32*>(buffer)));

		boost::asio::const_buffer handleBuf = buffer + sizeof(OcaUint32);
		cmd.handle = ntohl(*(boost::asio::buffer_cast<const OcaUint32*>(handleBuf)));

		boost::asio::const_buffer targetONoBuffer = handleBuf + sizeof(OcaUint32);
		cmd.targetONo = ntohl(*(boost::asio::buffer_cast<const OcaONo*>(targetONoBuffer)));

		boost::asio::const_buffer methodIdBuffer = targetONoBuffer + sizeof(OcaONo);
		MethodIdFromBuffer(methodIdBuffer, cmd.methodId);

		size_t remainingBytes = cmd.commandSize - (2*sizeof(OcaUint32) + sizeof(OcaONo) + sizeof(OcaMethodId));
		boost::asio::const_buffer parametersBuffer = methodIdBuffer + sizeof(OcaMethodId);

		ParametersFromBuffer(parametersBuffer, remainingBytes, cmd.parameters);
	}

	void OcpMessageReader::CommandListFromBuffer(boost::asio::const_buffer& buffer, net::Ocp1Header header, std::vector<net::Ocp1Command>& commands)
	{
		boost::asio::const_buffer message = buffer;

		for (uint16_t i = 0; i < header.messageCount; ++i )
		{
			net::Ocp1Command cmd;
			CommandFromBuffer(message, cmd);
			commands.push_back(cmd);
			message = message + cmd.commandSize;
		}
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




		oca::net::Ocp1Header header = HeaderFromBuffer(headerBuffer);

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
