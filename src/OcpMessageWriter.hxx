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
#ifndef __OCP1MESSAGEWRITER_HXX__
#define __OCP1MESSAGEWRITER_HXX__

// C++ Standard Headers
#include <vector>

// C Standard Headers


// Boost Headers
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#include "Ocp1Header.hxx"
#include "Ocp1Parameters.hxx"
#include "Ocp1Command.hxx"
#include "Ocp1Response.hxx"
#include "Ocp1EventData.hxx"
#include "Ocp1NtfParams.hxx"
#include "Ocp1Notification.hxx"

namespace oca
{

		class OcpMessageWriter : public boost::enable_shared_from_this<OcpMessageWriter>
		{
		public:

			static void WriteHeaderToBuffer(const ocp::Ocp1Header& header, uint8_t* buffer);

			static void WriteCommandListToBuffer(oca::net::CommandList const& commands, uint8_t* buffer);
			static void WriteCommandToBuffer(const net::Ocp1Command& command, uint8_t* buffer);
			static void WriteMethodIdToBuffer(const OcaMethodId& id, uint8_t* buffer);
			static void WriteParametersToBuffer(const net::Ocp1Parameters& parameters, uint8_t* buffer);

			static void WriteResponseListToBuffer(oca::net::ResponseList const& responses, uint8_t* buffer);
			static void WriteResponseToBuffer(const net::Ocp1Response& response, uint8_t* buffer);

			static void WriteEventIdToBuffer(const OcaEventId& id, uint8_t* buffer);
			static void WriteEventToBuffer(const OcaEvent& event, uint8_t* buffer);
			static void WriteEventDataToBuffer(const net::Ocp1EventData& data, uint8_t* buffer);

			static void WriteNtfParamsToBuffer(const net::Ocp1NtfParams& params, uint8_t* buffer);
			static void WriteNotificationToBuffer(const net::Ocp1Notification& notification, uint8_t* buffer);
			static void WriteNotificationListToBuffer(const net::NotificationList& notifications, uint8_t* buffer);

			static OcaUint32 ComputeCommandDataSize(net::Ocp1Command& command);
			static OcaUint32 ComputeCommandListDataSize(net::CommandList& commands);

			static OcaUint32 ComputeResponseDataSize(net::Ocp1Response& response);
			static OcaUint32 ComputeResponseListDataSize(net::ResponseList& responses);

			static OcaUint32 ComputeNotificationDataSize(net::Ocp1Notification& notification);
			static OcaUint32 ComputeNotificationListDataSize(net::NotificationList& notifications);


		};
}

#endif // __OCP1MESSAGEWRITER_HXX__
