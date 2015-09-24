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
#include "Ocp1Command.hxx"
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

		void OcpMessageWriter::WriteMethodIdToBuffer(const OcaMethodId& id, boost::asio::mutable_buffer& buffer)
		{
			OcaUint16* tl = boost::asio::buffer_cast<OcaUint16*>(buffer);
			*tl = htons(id.treeLevel);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			boost::asio::mutable_buffer methodIndexBuffer = buffer + sizeof(OcaUint16);
			OcaUint16* mi = boost::asio::buffer_cast<OcaUint16*>(methodIndexBuffer);
			*mi = htons(id.methodIndex);
		}

		void OcpMessageWriter::WriteCommandListToBuffer(oca::net::CommandList const& commands, boost::asio::mutable_buffer& buffer)
		{
			boost::asio::mutable_buffer message = buffer;
			for (oca::net::CommandList::const_iterator it = commands.begin(); it != commands.end(); ++it)
			{
				oca::net::Ocp1Command cmd = *it;
				WriteCommandToBuffer(cmd, message);
				message = message + cmd.commandSize;
			}
		}

		void OcpMessageWriter::WriteCommandToBuffer(const net::Ocp1Command& command, boost::asio::mutable_buffer& buffer)
		{
			OcaUint32* cs = boost::asio::buffer_cast<OcaUint32*>(buffer);
			*cs = htonl(command.commandSize);

			boost::asio::mutable_buffer handleBuffer = buffer + sizeof(OcaUint32);
			OcaUint32* hdl = boost::asio::buffer_cast<OcaUint32*>(handleBuffer);
			*hdl = htonl(command.handle);

			boost::asio::mutable_buffer oNoBuffer = handleBuffer + sizeof(OcaUint32);
			OcaUint32* oNo = boost::asio::buffer_cast<OcaUint32*>(oNoBuffer);
			*oNo = htonl(command.targetONo);

			boost::asio::mutable_buffer methodBuffer = oNoBuffer + sizeof(OcaONo);
			WriteMethodIdToBuffer(command.methodId, methodBuffer);

			boost::asio::mutable_buffer paramBuffer = methodBuffer + sizeof(OcaMethodId);
			WriteParametersToBuffer(command.parameters, paramBuffer);
		}

		OcaUint32 OcpMessageWriter::ComputeCommandDataSize(net::Ocp1Command& command)
		{
			OcaUint32 paramsSizeBytes = command.parameters.parameters.size() + sizeof(OcaUint8);
			OcaUint32 commandSize = paramsSizeBytes + (2*sizeof(OcaUint32) + sizeof(OcaONo) + sizeof(OcaMethodId));
			command.commandSize = commandSize;
			return commandSize;
		}

		OcaUint32 OcpMessageWriter::ComputeCommandListDataSize(net::CommandList& commands)
		{
			OcaUint32 commandListDataSize = 0;
			for (net::CommandList::iterator it = commands.begin(); it != commands.end(); ++it)
			{
				commandListDataSize += ComputeCommandDataSize(*it);
			}


			return commandListDataSize;
		}

		void OcpMessageWriter::WriteResponseListToBuffer(oca::net::ResponseList const& responses, boost::asio::mutable_buffer& buffer)
		{
			boost::asio::mutable_buffer message = buffer;
			for (oca::net::ResponseList::const_iterator it = responses.begin(); it != responses.end(); ++it)
			{
				oca::net::Ocp1Response resp = *it;
				WriteResponseToBuffer(resp, message);
				message = message + resp.responseSize;
			}
		}

		void OcpMessageWriter::WriteResponseToBuffer(const net::Ocp1Response& response, boost::asio::mutable_buffer& buffer)
		{
			OcaUint32* rs = boost::asio::buffer_cast<OcaUint32*>(buffer);
			*rs = htonl(response.responseSize);

			boost::asio::mutable_buffer handleBuf = buffer + sizeof(OcaUint32);
			OcaUint32* handle = boost::asio::buffer_cast<OcaUint32*>(handleBuf);
			*handle = htonl(response.handle);

			boost::asio::mutable_buffer statusCodeBuf = handleBuf + sizeof(OcaUint32);
			OcaUint8* statusCode = boost::asio::buffer_cast<OcaUint8*>(statusCodeBuf);
			*statusCode = response.statusCode;

			boost::asio::mutable_buffer paramsBuf = statusCodeBuf + sizeof(OcaUint8);
			WriteParametersToBuffer(response.parameters, paramsBuf);
		}

		OcaUint32 OcpMessageWriter::ComputeResponseDataSize(net::Ocp1Response& response)
		{
			OcaUint32 paramsSizeBytes = response.parameters.parameters.size() + sizeof(OcaUint8);
			OcaUint32 responseSize = paramsSizeBytes + (2*sizeof(OcaUint32) + sizeof(OcaStatus));
			response.responseSize = responseSize;
			return responseSize;
		}

		OcaUint32 OcpMessageWriter::ComputeResponseListDataSize(net::ResponseList& responses)
		{
			OcaUint32 responseListDataSize = 0;
			for (net::ResponseList::iterator it = responses.begin(); it != responses.end(); ++it)
			{
				responseListDataSize += ComputeResponseDataSize(*it);
			}

			return responseListDataSize;
		}

		void OcpMessageWriter::WriteEventIdToBuffer(const OcaEventId &id, boost::asio::mutable_buffer &buffer)
		{
			OcaUint16* tl = boost::asio::buffer_cast<OcaUint16*>(buffer);
			*tl = htons(id.treeLevel);

			boost::asio::mutable_buffer indexBuf = buffer + sizeof(OcaUint16);
			OcaUint16* ei = boost::asio::buffer_cast<OcaUint16*>(indexBuf);
			*ei = htons(id.eventIndex);
		}
}
