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
#ifndef __CHANGEME_HXX__
#define __CHANGEME_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers
#include <ITcpConnection.hxx>
#include <IOcpSession.hxx>


namespace oca
{
	class OcpMessageReader;

  namespace net
	{

      class MockTcpConnection : public ITcpConnection
		{
		public:


			static boost::shared_ptr<MockTcpConnection> Create(boost::asio::io_service& ioService)

			{
				return boost::shared_ptr<MockTcpConnection>(new MockTcpConnection(ioService));
			}

			boost::asio::ip::tcp::socket& GetSocket()
			{
				return socket;
			}

			void Start()
			{
				started = true;
			}

			void SetOcpSession(IOcpSession::pointer session)
			{
				this->session = session;
				sessionSet = true;
			}

			boost::system::error_code Send(boost::asio::const_buffer& buffer, size_t bytesToTransfer)
			{
				return boost::asio::error::eof;
			}

			virtual ~MockTcpConnection()
			{

			}

			bool started;
			bool sessionSet;
			IOcpSession::pointer session;

		private:
			// We make it non-constructible, and non-copyable so that the
			// only way to get an instance is through the Create member function
			// which forces the consumer to get a shared_ptr. Nice encapsulation!
			MockTcpConnection(boost::asio::io_service& ioService)
				: started(false), sessionSet(false), socket(ioService) {}
			MockTcpConnection(const MockTcpConnection& rhs);
			MockTcpConnection& operator=(const MockTcpConnection& rhs);

			boost::asio::ip::tcp::socket socket;
		};
	}
}


#endif // __CHANGEME_HXX__
