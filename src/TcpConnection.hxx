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
#ifndef __TCPCONNECTION_HXX__
#define __TCPCONNECTION_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers
#include "ITcpConnection.hxx"
#include "OcpSession.hxx"

#define OCP1_DATA_BUFFER_SIZE 1024

namespace oca
{
	class OcpMessageReader;

    namespace net
	{

        class TcpConnection : public ITcpConnection, public boost::enable_shared_from_this<TcpConnection>
		{
		public:

			static ITcpConnection::pointer Create(boost::asio::io_service& ioService, OcpSession::pointer processor);
			boost::asio::ip::tcp::socket& GetSocket();

			void Start();

			// The buffer contains the full data for the message, including the sync value and the
			boost::system::error_code Send(boost::asio::const_buffer& buffer, size_t bytesToTransfer);

			virtual ~TcpConnection();
		private:
			// We make it non-constructible, and non-copyable so that the
			// only way to get an instance is through the Create member function
			// which forces the consumer to get a shared_ptr. Nice encapsulation!
			TcpConnection(boost::asio::io_service& ioService, OcpSession::pointer processor);
			TcpConnection(const TcpConnection& rhs);
            TcpConnection& operator=(const TcpConnection& rhs);

			// The first step of reading any OCP.1 message is the sync value (0x3B)
			// if we don't get it, we must close the connection.
			void readSyncValue();
            void syncValueRead(const boost::system::error_code& error, size_t bytesTransferred);

			// The header is a fixed size and tells us how much data to expect
			void readOcp1Header();
			void ocp1HeaderRead(const boost::system::error_code& error, size_t bytesTransferred);

			// // The data could be 1 or more messages (commands, responses, notifications)
			// // The header tells us what to expect, and how many. It seems you don't
			// // mix message types in a single sync unit.
			void readOcp1Data(uint32_t dataSize);
			void ocp1DataRead(const boost::system::error_code& error, size_t bytesTransferred);


			boost::function<void(void)> readOcp1HeaderDelegate;
			boost::function<void(uint32_t)> readOcp1DataDelegate;

			uint8_t dataBuffer[OCP1_DATA_BUFFER_SIZE];
			boost::asio::ip::tcp::socket socket;
            OcpSession::pointer processor;
			uint64_t identifier;
		};
	}
}


#endif // __TCPCONNECTION_HXX__
