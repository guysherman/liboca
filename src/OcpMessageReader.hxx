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
  #ifndef __OCPMESSAGEREADER_HXX__
  #define __OCPMESSAGEREADER_HXX__

    // C++ Standard Headers
    #include <map>
    #include <vector>

    // C Standard Headers


    // Boost Headers
	#include <boost/system/error_code.hpp>
    #include <boost/function.hpp>
	#include <boost/bind.hpp>
    #include <boost/shared_ptr.hpp>
	#include <boost/enable_shared_from_this.hpp>

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
		class OcpMessageReader : public boost::enable_shared_from_this<OcpMessageReader>
		{
		public:
			typedef boost::shared_ptr<OcpMessageReader> pointer;

            OcpMessageReader();
            virtual ~OcpMessageReader();

            static const ocp::Ocp1Header HeaderFromBuffer(const uint8_t* buffer);	// TODO: should the parameter be const? I don't think so, but I don't know for sure.
			static void HeaderFromBuffer(const uint8_t* buffer, ocp::Ocp1Header& header);

            static void CommandListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, std::vector<net::Ocp1Command>& commands);
            static void CommandFromBuffer(const uint8_t* buffer, net::Ocp1Command& cmd);
            static void MethodIdFromBuffer(const uint8_t* buffer, OcaMethodId& methodId);
            static void ParametersFromBuffer(const uint8_t* buffer, size_t remainingCommandBytes, net::Ocp1Parameters& parameters);

            static void ResponseListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, std::vector<net::Ocp1Response>& responses);
            static void ResponseFromBuffer(const uint8_t* buffer, net::Ocp1Response& resp);

            static void EventIdFromBuffer(const uint8_t* buffer, OcaEventId& eventId);
            static void EventFromBuffer(const uint8_t* buffer, OcaEvent& event);
            static void EventDataFromBuffer(const uint8_t* buffer, size_t remainingBytes, net::Ocp1EventData& data);

            static void NtfParamsFromBuffer(const uint8_t* buffer, size_t remainingBytes, net::Ocp1NtfParams& params);
            static void NotificationFromBuffer(const uint8_t* buffer, net::Ocp1Notification& notification);
            static void NotificationListFromBuffer(const uint8_t* buffer, ocp::Ocp1Header header, net::NotificationList& notifications);

        private:

		};
	}

  #endif // __OCPMESSAGEREADER_HXX__
