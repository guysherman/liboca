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
#ifndef __MockOcpSessionFactory_HXX__
#define __MockOcpSessionFactory_HXX__


// C++ Standard Headers
#include <vector>

// C Standard Headers


// Boost Headers
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers
#include <IOcpSession.hxx>
#include <OcpSession.hxx>
#include <OcpSessionFactory.hxx>
#include "MockOcpConnection.hxx"

namespace oca
{
	class OcpMessageReader;

	namespace net
	{
		class TcpConnection;

		class MockOcpSessionFactory : public OcpSessionFactory
		{
		public:
			MockOcpSessionFactory(boost::shared_ptr<boost::asio::io_service> ioService, int maxConns)
                :   OcpSessionFactory(ioService), countConns(0), maxConns(maxConns)
            {

            }

			virtual ~MockOcpSessionFactory()
            {

            }

			virtual IOcpSession::pointer CreateSession()
            {
                if (countConns++ == maxConns)
				{
					return boost::shared_ptr<IOcpSession>();
				}
				boost::shared_ptr<MockOcpSession> conn = MockOcpSession::Create(*ioService);
				connections.push_back(conn);
				return conn;

            }

			std::vector< boost::shared_ptr<MockOcpSession> > connections;

		private:
			int countConns;
			int maxConns;
		};
	}
}


#endif // __MockOcpSessionFactory_HXX__
