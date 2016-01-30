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


// Our Headers

#include <oca/OcaTypes.hxx>

#include "OcpSession.hxx"
#include "Ocp1Header.hxx"
#include "OcpMessageReader.hxx"
#include "ITcpConnection.hxx"

namespace oca
{

	namespace net
	{

		OcpSession::OcpSession()
			: stashedHeader(boost::shared_ptr<Ocp1Header>()), id(nextId++), tcpConnection(ITcpConnection::pointer())
		{

		}

		OcpSession::~OcpSession()
		{

		}

		void OcpSession::SetTcpConnection(ITcpConnection::pointer connection)
		{
			tcpConnection = connection;
			tcpConnection->SetOcpSession(shared_from_this());
		}

		void OcpSession::SyncValueReceived(uint8_t* bufferData,
			const boost::system::error_code& error,
			size_t bytesTransferred,
			boost::function<void(void)> getHeader )
		{
			if (error.value() != boost::system::errc::success)
			{
				// TODO: consider an exception here
				sessionClosed();
				return;
			}


			if (bytesTransferred != 1)
			{
				// TODO: consider an exception here
				sessionClosed();
				return;
			}


			// TODO: logging


			if (bufferData != 0)
			{
				if (bufferData[0] == 0x3B)
				{
					getHeader();
				}
				else
				{
					sessionClosed();
				}
			}
		}

		void OcpSession::Ocp1HeaderReceived(uint8_t* bufferData,
			uint64_t connectionIdentifier,
			const boost::system::error_code& error,
			size_t bytesTransferred,
			boost::function<void(uint32_t)> getData)
		{
			boost::asio::const_buffer headerBuffer(bufferData, bytesTransferred);



			// By convention we should be removing the per connection header object once we're done
			// so if this assert throws we've either forgotten, or we've got a concurrency
			// issue.
			assert(stashedHeader == NULL);


			oca::net::Ocp1Header header = OcpMessageReader::HeaderFromBuffer(headerBuffer);

			*stashedHeader = header;

			// The messageSize property includes the header, but we've
			// already got it so we ask for fewer bytes.
			getData(header.messageSize - OCP1_HEADER_SIZE);

		}

		void OcpSession::Ocp1DataReceived(uint8_t* bufferData,
			uint64_t connectionIdentifier,
			const boost::system::error_code& error,
			size_t bytesTransferred)
		{
			// assert(stashedHeader != NULL);
			// switch (stashedHeader->messageType)
			// {
			// 	case OcaMessageType::OcaKeepAlive:
			// 		processKeepAliveMessage(bufferData, stashedHeader);
			// 		break;
			// }


			// ...but before we go, lets clean up
			stashedHeader = boost::shared_ptr<oca::net::Ocp1Header>();
			return;
		}

		void OcpSession::AddSessionClosedHandler(SessionEventHandler handler)
		{
			// TODO: think about finding some way to not add the same handler twice
			sessionClosedHandlers.push_back(handler);
		}

		void OcpSession::sessionClosed()
		{
			for (std::vector<IOcpSession::SessionEventHandler>::iterator it = sessionClosedHandlers.begin(); it != sessionClosedHandlers.end(); ++it)
			{
				(*it)(shared_from_this());
			}

			sessionClosedHandlers.clear();
		}

		int OcpSession::GetId()
		{
			return id;
		}

		int OcpSession::nextId = 0;
	}


}
