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

namespace oca
{

	namespace net
	{
		IOcpSession::pointer OcpSession::Create(boost::asio::io_service &ioService)
		{
			return pointer(new OcpSession(ioService));
		}

		OcpSession::OcpSession(boost::asio::io_service &ioService)
			: stashedHeader(boost::shared_ptr<Ocp1Header>()), id(nextId++), socket(ioService), identifier((uint64_t)this)
		{
			memset(&dataBuffer[0], 0, OCP1_DATA_BUFFER_SIZE);
		}


		OcpSession::~OcpSession()
		{

		}



		void OcpSession::Start()
		{
			readSyncValue();

		}

		void OcpSession::readSyncValue()
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], 1),
				boost::bind(
					&OcpSession::syncValueRead,
					shared_from_this(),
					boost::asio::placeholders::error,
				 	boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void OcpSession::syncValueRead(const boost::system::error_code& error, size_t bytesTransferred)
		{

			this->SyncValueReceived(&dataBuffer[0],
				error,
				bytesTransferred);



		}

		void OcpSession::SyncValueReceived(uint8_t* bufferData,
			const boost::system::error_code& error,
			size_t bytesTransferred)
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
					readOcp1Header();
				}
				else
				{
					sessionClosed();
				}
			}
		}

		void OcpSession::readOcp1Header()
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], OCP1_HEADER_SIZE),
				boost::bind(
					&OcpSession::ocp1HeaderRead,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void OcpSession::ocp1HeaderRead(const boost::system::error_code& error, size_t bytesTransferred)
		{
			this->Ocp1HeaderReceived(
				&dataBuffer[0],
				identifier,
				error,
				bytesTransferred);
		}

		void OcpSession::Ocp1HeaderReceived(uint8_t* bufferData,
			uint64_t connectionIdentifier,
			const boost::system::error_code& error,
			size_t bytesTransferred)
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
			readOcp1Data(header.messageSize - OCP1_HEADER_SIZE);

		}

		void OcpSession::readOcp1Data(uint32_t dataSize)
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], (size_t)dataSize),
				boost::bind(
					&OcpSession::ocp1DataRead,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void OcpSession::ocp1DataRead(const boost::system::error_code& error, size_t bytesTransferred)
		{
			this->Ocp1DataReceived(&dataBuffer[0], identifier, error, bytesTransferred);

			// Now that we've read all the way through the message, we go wait
			// for the next one.
			readSyncValue();
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

		boost::system::error_code OcpSession::Send(boost::asio::const_buffer& buffer, size_t bytesToTransfer)
		{
			boost::system::error_code error;
			boost::asio::write(socket, boost::asio::buffer(buffer, bytesToTransfer), error);
			return error;
		}

		boost::asio::ip::tcp::socket& OcpSession::GetSocket()
		{
			return socket;
		}

	}



}
