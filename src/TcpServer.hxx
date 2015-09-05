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
	#include <boost/shared_ptr.hpp>
	#include <boost/asio.hpp>

    // 3rd Party Headers


    // GTK Headers



namespace oca
{
	namespace net
	{
		class TcpConnection;

		class TcpServer
		{
		public:
			TcpServer();
			TcpServer(uint16_t port);

			virtual ~TcpServer();


		private:
			void accept();
			void accepted(boost::shared_ptr<TcpConnection> connection, const boost::system::error_code& error);


			boost::asio::ip::tcp::acceptor acceptor;
		};
	}
}

  #endif // __TCPSERVER_HXX__