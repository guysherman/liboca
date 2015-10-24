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
#ifndef __MOCKTCPCONNECTIONFACTORY_HXX__
#define __MOCKTCPCONNECTIONFACTORY_HXX__


// C++ Standard Headers
#include <vector>

// C Standard Headers


// Boost Headers
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers
#include <ITcpConnection.hxx>
#include <TcpConnectionFactory.hxx>
#include <OcpSession.hxx>
#include "MockTcpConnection.hxx"

namespace oca
{
	class OcpMessageReader;

	namespace net
	{
		class TcpConnection;

		class MockTcpConnectionFactory : public TcpConnectionFactory
		{
		public:
			MockTcpConnectionFactory(boost::shared_ptr<boost::asio::io_service> ioService, int maxConns)
                :   TcpConnectionFactory(ioService, OcpSession::pointer()), countConns(0), maxConns(maxConns)
            {

            }

			virtual ~MockTcpConnectionFactory()
            {

            }

			virtual ITcpConnection::pointer CreateConnection()
            {
                if (countConns++ == maxConns)
				{
					return boost::shared_ptr<ITcpConnection>();
				}
				boost::shared_ptr<MockTcpConnection> conn = MockTcpConnection::Create(*ioService);
				connections.push_back(conn);
				return conn;

            }

			std::vector< boost::shared_ptr<MockTcpConnection> > connections;

		private:
			int countConns;
			int maxConns;
		};
	}
}


#endif // __MOCKTCPCONNECTIONFACTORY_HXX__
