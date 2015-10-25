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
  #ifndef __TCPSERVER_HXX__
  #define __TCPSERVER_HXX__

    // C++ Standard Headers


    // C Standard Headers


    // Boost Headers
    #include <boost/function.hpp>
    #include <boost/bind.hpp>
    #include <boost/shared_ptr.hpp>
	#include <boost/asio.hpp>

    // 3rd Party Headers


    // GTK Headers

// Our Headers

#include "TcpConnectionFactory.hxx"
#include "ITcpConnection.hxx"

namespace oca
{
	namespace net
	{

		class TcpServer
		{
		public:
			typedef boost::function< void(boost::shared_ptr<ITcpConnection>) > ConnectionEventHandler;

            TcpServer(
                boost::shared_ptr<oca::net::TcpConnectionFactory> connectionFactory,
                boost::shared_ptr<boost::asio::io_service> ioService,
                uint16_t port,
                ConnectionEventHandler handler);
            void Start();
            void Stop();
			virtual ~TcpServer();


		private:
			void accept();
			void accepted(boost::shared_ptr<ITcpConnection> connection, const boost::system::error_code& error);


			boost::asio::ip::tcp::acceptor acceptor;
            boost::shared_ptr<TcpConnectionFactory> connectionFactory;
            boost::shared_ptr<boost::asio::io_service> ioService;
            uint16_t port;
            bool isRunning;
            ConnectionEventHandler handler;

		};
	}
}

  #endif // __TCPSERVER_HXX__
