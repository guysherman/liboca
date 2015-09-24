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

namespace oca
{

		class OcpMessageWriter : public boost::enable_shared_from_this<OcpMessageWriter>
		{
		public:

			static void WriteHeaderToBuffer(const net::Ocp1Header& header, boost::asio::mutable_buffer& buffer);

			static void WriteCommandListToBuffer(oca::net::CommandList const& commands, boost::asio::mutable_buffer& buffer);
			static void WriteCommandToBuffer(const net::Ocp1Command& command, boost::asio::mutable_buffer& buffer);
			static void WriteMethodIdToBuffer(const OcaMethodId& id, boost::asio::mutable_buffer& buffer);
			static void WriteParametersToBuffer(const net::Ocp1Parameters& parameters, boost::asio::mutable_buffer& buffer);

			static void WriteResponseListToBuffer(oca::net::ResponseList const& responses, boost::asio::mutable_buffer& buffer);
			static void WriteResponseToBuffer(const net::Ocp1Response& response, boost::asio::mutable_buffer& buffer);

			static void WriteEventIdToBuffer(const OcaEventId& id, boost::asio::mutable_buffer& buffer);

			static OcaUint32 ComputeCommandDataSize(net::Ocp1Command& command);
			static OcaUint32 ComputeCommandListDataSize(net::CommandList& commands);

			static OcaUint32 ComputeResponseDataSize(net::Ocp1Response& response);
			static OcaUint32 ComputeResponseListDataSize(net::ResponseList& responses);




		};
}

#endif // __OCP1MESSAGEWRITER_HXX__
