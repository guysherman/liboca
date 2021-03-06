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
#include "OcaBasicTypeWriter.hxx"
#include "Ocp1Header.hxx"
#include "Ocp1Parameters.hxx"
#include "Ocp1Command.hxx"
#include "OcpMessageWriter.hxx"





namespace oca
{
		void OcpMessageWriter::WriteHeaderToBuffer(const ocp::Ocp1Header& header, uint8_t* buffer)
		{
			// We cast the buffer to a pointer to the type we want, so that we
			// can write our data there.
			uint16_t* pv = reinterpret_cast<uint16_t*>(buffer);
			*pv = htons(header.protocolVersion);

			// Again, we get a new buffer (over the same raw data) that is
			// offset by the correct amount to give us the next field
			uint8_t* bms = buffer+sizeof(uint16_t);
			uint32_t* ms = reinterpret_cast<uint32_t*>(bms);
			*ms = htonl(header.messageSize);

			uint8_t* bmt = bms+sizeof(uint32_t);
			uint8_t* mt = reinterpret_cast<uint8_t*>(bmt);
			*mt = (uint8_t)header.messageType;

			uint8_t* bmc = bmt+sizeof(uint8_t);
			uint16_t* mc = reinterpret_cast<uint16_t*>(bmc);
			*mc = htons(header.messageCount);
		}

		void OcpMessageWriter::WriteParametersToBuffer(const net::Ocp1Parameters& parameters, uint8_t* buffer)
		{
			OcaUint8* pc = reinterpret_cast<OcaUint8*>(buffer);
			*pc = parameters.parameterCount;

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* paramBuffer = buffer + sizeof(OcaUint8);
			OcaUint8* destination = reinterpret_cast<OcaUint8*>(paramBuffer);
			memcpy(destination, &parameters.parameters[0], parameters.parameters.size());

		}

		void OcpMessageWriter::WriteMethodIdToBuffer(const OcaMethodId& id, uint8_t* buffer)
		{
			OcaUint16* tl = reinterpret_cast<OcaUint16*>(buffer);
			*tl = htons(id.treeLevel);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* methodIndexBuffer = buffer + sizeof(OcaUint16);
			OcaUint16* mi = reinterpret_cast<OcaUint16*>(methodIndexBuffer);
			*mi = htons(id.methodIndex);
		}

		void OcpMessageWriter::WriteCommandListToBuffer(oca::net::CommandList const& commands, uint8_t* buffer)
		{
			uint8_t* message = buffer;
			for (oca::net::CommandList::const_iterator it = commands.begin(); it != commands.end(); ++it)
			{
				// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
				oca::net::Ocp1Command cmd = *it;
				WriteCommandToBuffer(cmd, message);
				message = message + cmd.commandSize;
			}
		}

		void OcpMessageWriter::WriteCommandToBuffer(const net::Ocp1Command& command, uint8_t* buffer)
		{
			OcaUint32* cs = reinterpret_cast<OcaUint32*>(buffer);
			*cs = htonl(command.commandSize);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* handleBuffer = buffer + sizeof(OcaUint32);
			OcaUint32* hdl = reinterpret_cast<OcaUint32*>(handleBuffer);
			*hdl = htonl(command.handle);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* oNoBuffer = handleBuffer + sizeof(OcaUint32);
			OcaUint32* oNo = reinterpret_cast<OcaUint32*>(oNoBuffer);
			*oNo = htonl(command.targetONo);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* methodBuffer = oNoBuffer + sizeof(OcaONo);
			WriteMethodIdToBuffer(command.methodId, methodBuffer);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* paramBuffer = methodBuffer + sizeof(OcaMethodId);
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

		void OcpMessageWriter::WriteResponseListToBuffer(oca::net::ResponseList const& responses, uint8_t* buffer)
		{
			uint8_t* message = buffer;
			for (oca::net::ResponseList::const_iterator it = responses.begin(); it != responses.end(); ++it)
			{
				oca::net::Ocp1Response resp = *it;
				WriteResponseToBuffer(resp, message);
				message = message + resp.responseSize;
			}
		}

		void OcpMessageWriter::WriteResponseToBuffer(const net::Ocp1Response& response, uint8_t* buffer)
		{
			OcaUint32* rs = reinterpret_cast<OcaUint32*>(buffer);
			*rs = htonl(response.responseSize);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* handleBuf = buffer + sizeof(OcaUint32);
			OcaUint32* handle = reinterpret_cast<OcaUint32*>(handleBuf);
			*handle = htonl(response.handle);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* statusCodeBuf = handleBuf + sizeof(OcaUint32);
			OcaUint8* statusCode = reinterpret_cast<OcaUint8*>(statusCodeBuf);
			*statusCode = response.statusCode;

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* paramsBuf = statusCodeBuf + sizeof(OcaUint8);
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

		void OcpMessageWriter::WriteEventIdToBuffer(const OcaEventId &id, uint8_t* buffer)
		{
			OcaUint16* tl = reinterpret_cast<OcaUint16*>(buffer);
			*tl = htons(id.treeLevel);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* indexBuf = buffer + sizeof(OcaUint16);
			OcaUint16* ei = reinterpret_cast<OcaUint16*>(indexBuf);
			*ei = htons(id.eventIndex);
		}

		void OcpMessageWriter::WriteEventToBuffer(const OcaEvent& event, uint8_t* buffer)
		{
			OcaONo* emitter = reinterpret_cast<OcaONo*>(buffer);
			*emitter = htonl(event.emitterONo);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* idBuffer = buffer + sizeof(OcaONo);
			WriteEventIdToBuffer(event.eventId, idBuffer);
		}

		void OcpMessageWriter::WriteEventDataToBuffer(const net::Ocp1EventData& data, uint8_t* buffer)
		{
			WriteEventToBuffer(data.event, buffer);

			// TODO: there's a bit of a security issue here: we trust that the buffer has enough space #security
			uint8_t* paramsBuffer = buffer + sizeof(OcaEvent);
			OcaUint8* destination = reinterpret_cast<OcaUint8*>(paramsBuffer);
			memcpy(destination, &data.eventParameters[0], data.eventParameters.size());
		}

		void OcpMessageWriter::WriteNtfParamsToBuffer(const net::Ocp1NtfParams& params, uint8_t* buffer)
		{
			uint8_t* temp = buffer;
			OcaBasicTypeWriter::WriteUint8ToBuffer(params.parameterCount, &temp);
			OcaBasicTypeWriter::WriteBlobToBuffer(params.context, &temp);
			WriteEventDataToBuffer(params.eventData, temp);
		}

		void OcpMessageWriter::WriteNotificationToBuffer(const net::Ocp1Notification& notification, uint8_t* buffer)
		{
			uint8_t* temp = buffer;
			OcaBasicTypeWriter::WriteUint32ToBuffer(notification.notificationSize, &temp);
			OcaBasicTypeWriter::WriteUint32ToBuffer(notification.targetONo, &temp);
			WriteMethodIdToBuffer(notification.methodId, temp);
			temp = temp + sizeof(OcaMethodId);

			WriteNtfParamsToBuffer(notification.parameters, temp);
		}

		void OcpMessageWriter::WriteNotificationListToBuffer(const net::NotificationList& notifications, uint8_t* buffer)
		{
			uint8_t* message = buffer;
			for (net::NotificationList::const_iterator it = notifications.begin(); it != notifications.end(); ++it)
			{
				const net::Ocp1Notification& notification = *it;
				WriteNotificationToBuffer(notification, message);
				message = message + notification.notificationSize;
			}
		}

		OcaUint32 OcpMessageWriter::ComputeNotificationDataSize(net::Ocp1Notification& notification)
		{
			size_t parameterBytes = notification.parameters.eventData.eventParameters.size();
			size_t eventDataSize = parameterBytes + sizeof(OcaEvent);
			size_t contextSize = notification.parameters.context.size() + sizeof(OcaUint16);
			size_t ntfParamsSize = eventDataSize + contextSize + sizeof(OcaUint8);

			size_t notificationHeaderSize = sizeof(OcaUint32) + sizeof(OcaONo) + sizeof(OcaMethodId);
			size_t notificationSize = notificationHeaderSize + ntfParamsSize;

			notification.notificationSize = notificationSize;
			return notificationSize;
		}

		OcaUint32 OcpMessageWriter::ComputeNotificationListDataSize(net::NotificationList& notifications)
		{
			size_t notificationListDataSize = 0;
			for (net::NotificationList::iterator it = notifications.begin(); it != notifications.end(); ++it)
			{
				notificationListDataSize += ComputeNotificationDataSize(*it);
			}


			return notificationListDataSize;
		}

}
