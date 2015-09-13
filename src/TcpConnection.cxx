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
#include <boost/bind/protect.hpp>


// 3rd Party Headers

// GTK Headers

#include "TcpConnection.hxx"
#include "OcpMessageReader.hxx"
#include "Ocp1Header.hxx"

namespace oca
{
	namespace net
	{
		TcpConnection::pointer TcpConnection::Create(boost::asio::io_service &ioService, OcpMessageReader::pointer processor)
		{
			return pointer(new TcpConnection(ioService, processor));
		}

		void TcpConnection::Start()
		{
			readSyncValue();

		}

		TcpConnection::TcpConnection(boost::asio::io_service &ioService, OcpMessageReader::pointer processor)
			:	socket(ioService), processor(processor), identifier((uint64_t)this)
		{
			memset(&dataBuffer[0], 0, OCP1_DATA_BUFFER_SIZE);
		}

		TcpConnection::~TcpConnection()
		{

		}

		boost::asio::ip::tcp::socket& TcpConnection::GetSocket()
		{
			return socket;
		}

		void TcpConnection::readSyncValue()
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], 1),
				boost::bind(
					&TcpConnection::syncValueRead,
					shared_from_this(),
					boost::asio::placeholders::error,
				 	boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void TcpConnection::syncValueRead(const boost::system::error_code& error, size_t bytesTransferred)
		{
				processor->SyncValueReceived(&dataBuffer[0],
					error,
					bytesTransferred,
					boost::bind(&TcpConnection::readOcp1Header, shared_from_this())
				);

		}


		void TcpConnection::readOcp1Header()
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], OCP1_HEADER_SIZE),
				boost::bind(
					&TcpConnection::ocp1HeaderRead,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void TcpConnection::ocp1HeaderRead(const boost::system::error_code& error, size_t bytesTransferred)
		{
			processor->Ocp1HeaderReceived(
				&dataBuffer[0],
				identifier,
				error,
				bytesTransferred,
				boost::bind(&TcpConnection::readOcp1Data, shared_from_this(), _1)
		    );
		}

		void TcpConnection::readOcp1Data(uint32_t dataSize)
		{
			boost::asio::async_read(socket, boost::asio::buffer(&dataBuffer[0], (size_t)dataSize),
				boost::bind(
					&TcpConnection::ocp1DataRead,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void TcpConnection::ocp1DataRead(const boost::system::error_code& error, size_t bytesTransferred)
		{
			processor->Ocp1DataReceived(&dataBuffer[0], identifier, error, bytesTransferred);


			// Now that we've read all the way through the message, we go wait
			// for the next one.
			readSyncValue();
		}

	}
}
