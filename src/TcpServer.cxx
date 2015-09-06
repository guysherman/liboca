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
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#include "TcpServer.hxx"
#include "TcpConnectionFactory.hxx"
#include "ITcpConnection.hxx"


namespace oca
{
    namespace net
    {
        TcpServer::TcpServer(boost::shared_ptr<TcpConnectionFactory> connectionFactory, boost::shared_ptr<boost::asio::io_service> ioService, uint16_t port)
            : acceptor(*ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), connectionFactory(connectionFactory), ioService(ioService), port(port)
        {
            accept();
        }

        void TcpServer::accept()
        {
            ITcpConnection::pointer connection = connectionFactory->CreateConnection();
            if (connection == NULL)
            {
                return;
            }

            acceptor.async_accept(
                connection->GetSocket(),
                boost::bind(
                    &TcpServer::accepted,
                    this,
                    connection,
                    boost::asio::placeholders::error
                )
            );
        }

        void TcpServer::accepted(boost::shared_ptr<ITcpConnection> connection, const boost::system::error_code &error)
        {
            if (!error)
            {
                connection->Start();
            }

            accept();
        }

        void TcpServer::Start()
        {
            ioService->run();
        }

        void TcpServer::Stop()
        {
            ioService->stop();
        }

        TcpServer::~TcpServer()
        {

        }
    }
}
