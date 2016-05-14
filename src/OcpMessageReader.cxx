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
#include "Ocp1Response.hxx"
#include "Ocp1EventData.hxx"
#include "OcaBasicTypeReader.hxx"

namespace oca
{
	OcpMessageReader::OcpMessageReader()
	{

	}

	OcpMessageReader::~OcpMessageReader()
	{

	}


	const ocp::Ocp1Header OcpMessageReader::HeaderFromBuffer(const uint8_t* buffer)
	{
		ocp::Ocp1Header header;

		HeaderFromBuffer(buffer, header);

		return header;
	}

	void OcpMessageReader::HeaderFromBuffer(const uint8_t* buffer, ocp::Ocp1Header& header)
	{
		// Cast the buffer to the type we want, and dereference the pointer
		// so that we can read the data.
		header.protocolVersion = ntohs(* reinterpret_cast<const uint16_t*>(buffer));

		// Create a new buffer offset from the previous one so that we can
		// do the same as above but for the next field
		const uint8_t* ms = buffer+sizeof(uint16_t);
		header.messageSize = ntohl(* reinterpret_cast<const uint32_t*>(ms));

		const uint8_t* mt = ms+sizeof(uint32_t);
		header.messageType = static_cast<ocp::OcaMessageType>(* mt);

		const uint8_t* mc = mt+sizeof(uint8_t);
		header.messageCount = ntohs(* reinterpret_cast<const uint16_t*>(mc));
	}

	void OcpMessageReader::ParametersFromBuffer(const uint8_t* buffer, size_t remainingCommandBytes, net::Ocp1Parameters& parameters)
	{
		parameters.parameterCount = *buffer;

		size_t parameterBufferBytes = (remainingCommandBytes - sizeof(uint8_t));
		const uint8_t* paramBuffer = buffer+sizeof(uint8_t);
		//const uint8_t* params = boost::asio::buffer_cast<const uint8_t*>(paramBuffer);

		OcaBasicTypeReader::BufferToUint8Vector(&paramBuffer, parameterBufferBytes, parameters.parameters);

	}



	// TODO: move this to OcpBasicTypeReader and make it implicitly advance buffer #refactor
	void OcpMessageReader::MethodIdFromBuffer(const uint8_t* buffer, OcaMethodId& methodId)
	{
		methodId.treeLevel = ntohs(* reinterpret_cast<const OcaUint16*>(buffer));

		const uint8_t* mi = buffer + sizeof(OcaUint16);
		methodId.methodIndex = ntohs(* reinterpret_cast<const OcaUint16*>(mi));
	}

	void OcpMessageReader::CommandFromBuffer(const uint8_t* buffer, net::Ocp1Command& cmd)
	{
		cmd.commandSize = ntohl(* reinterpret_cast<const OcaUint32*>(buffer));

		const uint8_t* handleBuf = buffer + sizeof(OcaUint32);
		cmd.handle = ntohl(* reinterpret_cast<const OcaUint32*>(handleBuf));

		const uint8_t* targetONoBuffer = handleBuf + sizeof(OcaUint32);
		cmd.targetONo = ntohl(* reinterpret_cast<const OcaONo*>(targetONoBuffer));

		const uint8_t* methodIdBuffer = targetONoBuffer + sizeof(OcaONo);
		MethodIdFromBuffer(methodIdBuffer, cmd.methodId);

		size_t remainingBytes = cmd.commandSize - (2*sizeof(OcaUint32) + sizeof(OcaONo) + sizeof(OcaMethodId));
		const uint8_t* parametersBuffer = methodIdBuffer + sizeof(OcaMethodId);

		ParametersFromBuffer(parametersBuffer, remainingBytes, cmd.parameters);
	}

	void OcpMessageReader::CommandListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, std::vector<net::Ocp1Command>& commands)
	{
	 	const uint8_t* message = buffer;

		for (uint16_t i = 0; i < header.messageCount; ++i )
		{
			net::Ocp1Command cmd;
			CommandFromBuffer(message, cmd);
			commands.push_back(cmd);
			message = message + cmd.commandSize;
		}
	}

	void OcpMessageReader::ResponseListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, std::vector<net::Ocp1Response>& responses)
	{
		const uint8_t* message = buffer;
		for (uint16_t i = 0; i < header.messageCount; ++i)
		{
			net::Ocp1Response resp;
			ResponseFromBuffer(message, resp);
			responses.push_back(resp);
			message = message + resp.responseSize;
		}
	}

	void OcpMessageReader::ResponseFromBuffer(const uint8_t* buffer, net::Ocp1Response& resp)
	{
		resp.responseSize = ntohl(* reinterpret_cast<const OcaUint32*>(buffer));;

		const uint8_t* handleBuf =  buffer + sizeof(OcaUint32);
		resp.handle = ntohl(* reinterpret_cast<const OcaUint32*>(handleBuf));;

		const uint8_t* statusCodeBuf =  handleBuf + sizeof(OcaUint32);
		resp.statusCode = (* reinterpret_cast<const OcaStatus*>(statusCodeBuf));

		const uint8_t* paramsBuf =  statusCodeBuf + sizeof(OcaStatus);
		size_t remainingBytes = resp.responseSize - (2*sizeof(OcaUint32) + sizeof(OcaStatus));
		ParametersFromBuffer(paramsBuf, remainingBytes, resp.parameters);
	}

	void OcpMessageReader::EventIdFromBuffer(const uint8_t* buffer, OcaEventId& eventId)
	{
		eventId.treeLevel = ntohs(* reinterpret_cast<const OcaUint16*>(buffer));;

		const uint8_t* indexBuf =  buffer + sizeof(OcaUint16);
		eventId.eventIndex = ntohs(* reinterpret_cast<const OcaUint16*>(indexBuf));;
	}

	void OcpMessageReader::EventFromBuffer(const uint8_t* buffer, OcaEvent& event)
	{
		event.emitterONo = ntohl(* reinterpret_cast<const OcaONo*>(buffer));;

		const uint8_t* idBuffer =  buffer + sizeof(OcaONo);
		EventIdFromBuffer(idBuffer, event.eventId);
	}

	void OcpMessageReader::EventDataFromBuffer(const uint8_t* buffer, size_t remainingBytes, oca::net::Ocp1EventData& data)
	{
		EventFromBuffer(buffer, data.event);

		const uint8_t* paramsBuffer =  buffer + sizeof(OcaEvent);
		size_t parameterBytes = remainingBytes - sizeof(OcaEvent);
		OcaBasicTypeReader::BufferToUint8Vector(&paramsBuffer, parameterBytes, data.eventParameters);

	}

	void OcpMessageReader::NtfParamsFromBuffer(const uint8_t* buffer, size_t remainingBytes, net::Ocp1NtfParams& params)
	{
		const uint8_t* temp =  buffer;
		params.parameterCount = OcaBasicTypeReader::Uint8FromBuffer(&temp);
		OcaBasicTypeReader::BlobFromBuffer(&temp, params.context);
		size_t remainingEventDataBytes = remainingBytes - sizeof(OcaUint8) - (params.context.size() * sizeof(OcaUint8)) - sizeof(OcaUint16);
		EventDataFromBuffer(temp, remainingEventDataBytes, params.eventData);
	}

	void OcpMessageReader::NotificationFromBuffer(const uint8_t* buffer, net::Ocp1Notification& notification)
	{
		const uint8_t* temp =  buffer;
		notification.notificationSize = oca::OcaBasicTypeReader::Uint32FromBuffer(&temp);
		notification.targetONo = oca::OcaBasicTypeReader::Uint32FromBuffer(&temp);
		MethodIdFromBuffer(temp, notification.methodId);
		temp = temp + sizeof(OcaMethodId);
		size_t parameterChunkBytes = notification.notificationSize - sizeof(OcaUint32) - sizeof(OcaONo) - sizeof(OcaMethodId);
		NtfParamsFromBuffer(temp, parameterChunkBytes, notification.parameters);
	}

	void OcpMessageReader::NotificationListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, net::NotificationList& notifications)
	{
		const uint8_t* message =  buffer;
		for (OcaUint16 i = 0; i < header.messageCount; ++i)
		{
			net::Ocp1Notification notification;
			NotificationFromBuffer(message, notification);
			notifications.push_back(notification);
			message = message + notification.notificationSize;
		}
	}

}
